/*
* Copyright (c) 2025 InterDigital CE Patent Holdings SASU
* Licensed under the License terms of 5GMAG software (the "License").
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at https://www.5g-mag.com/license .
* Unless required by applicable law or agreed to in writing, software distributed under the License is
* distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and limitations under the License.
*/
#include "PCCBitstream.h"
#include "PCCBitstreamReader.h"
#include "PCCBitstreamWriter.h"
#include "PCCCommon.h"
#include "PCCContext.h"
#include "PCCDecoder.h"
#include "PCCFrameContext.h"
#include "PccGroupOfFrames.h"
#include "PccHevcParser.h"

#include <filesystem>
#include "sequence.h"

Sequence::Sequence(): numFrames_(0), frameRate_(0)
{   
}
Sequence::~Sequence() { groupOfFramesList_.clear(); }

void Sequence::clear()
{
    numFrames_ = 0;
    frameRate_ = 0;
    groupOfFramesList_.clear();
}

size_t Sequence::writeChunk(const std::filesystem::path &inputFile, const size_t fps, const std::filesystem::path outputDir)
{
    std::ifstream inputStream(inputFile);
    if (!inputStream.is_open())
    {
        std::cerr << "\033[31m" << "File not found" << std::endl;
        return -1;
    }

    std::ifstream input(inputFile, std::ios::in | std::ios::binary);
    if (!input.is_open())
    {
        std::cerr << "\033[31m" << "Can't open input file: " << inputFile << std::endl;
        return -1;
    }

    std::filesystem::path destDir = outputDir / inputFile.stem();
    if (!std::filesystem::is_directory(destDir))
    {
        if (!std::filesystem::create_directories(destDir)) {
            std::cerr << "\033[31m" << "Output Directory not created: " << destDir << std::endl;
            return -1;
        }
    }
    else
    {
        std::cerr << "\033[31m" << "Chunk generation is aborted since OutputDir already exist : " << destDir << std::endl;
        return -1;
    }
    bool ret = read(inputFile, fps);

    size_t index = 0;
    if (ret)
    {
        for (auto &gof : groupOfFramesList_)
        {
            int digit = 4;
            std::ostringstream indexStr;
            indexStr << std::setw(digit) << std::setfill('0') << index;

            std::string tmp = inputFile.stem().generic_string() + std::string("_") + indexStr.str() + std::string(".bin");
            std::filesystem::path outPath = destDir / tmp;
            std::cout << "outputFile: " << outPath << std::endl;

            std::ofstream output(outPath, std::ios::out | std::ios::binary);
            if (!output.is_open())
            {
                std::cerr << "\033[31m" << "writeChunk can't open output file: " << outPath << std::endl ;
                return false;
            }

            pcc::PCCBitstream bitstream;
            pcc::PCCBitstreamWriter bitstreamWriter;
            auto &ssvu = gof.getSampleStreamV3CUnit();

            bitstreamWriter.write(ssvu, bitstream); // Write ssvu with v3c_header + v3c_unit list
            bitstream.write(outPath.generic_string());
            index++;
        }
        input.close();
    }
    else
    {
        std::cerr << "\033[31m" << "Error while read input file:" << inputFile << std::endl;
    }
    std::cout << index << " chunks created in " << destDir << std::endl;

    return 0;
}

bool Sequence::read(const std::filesystem::path &inputFilePath, const size_t frameRate)
{
    std::cout << "read: " << inputFilePath << std::endl;
    clear();

    filename_ = inputFilePath;
    frameRate_ = frameRate;
    pcc::SampleStreamV3CUnit ssvu;
    pcc::PCCBitstream bitstream;
    pcc::PCCBitstreamStat bitstreamStat;
    pcc::PCCLogger logger;
    logger.initilalize(pcc::removeFileExtension(filename_.generic_string()) + "_createLocalChunks_", true);
#ifdef BITSTREAM_TRACE
    bitstream.setLogger(logger);
    bitstream.setTrace(true);
#endif
    if (!bitstream.initialize(inputFilePath.generic_string()))
    {
        std::cerr << "\033[31m" << "initilization error : cat = %s \n" << inputFilePath <<std::endl;
        return false;
    }

    //bitstreamStat.setHeader(bitstream.size());
    ssvu.getV3CUnit().clear();
    pcc::PCCBitstreamReader bitstreamReader;
#ifdef BITSTREAM_TRACE
    bitstreamReader.setLogger(logger);
#endif
    size_t headerSize = bitstreamReader.read(bitstream, ssvu);
    std::cout << "read : headerSize = " << headerSize << std::endl;
    if (!headerSize)
    {
        std::cout << "Error: headerSize = " << headerSize << std::endl;
        return 0;
    }

    std::cout << "read: " <<
             inputFilePath <<
             " " <<
             ssvu.getV3CUnitCount() <<
             " ssvh_unit_size_precision_bytes_minus1 = " <<
             ssvu.getSsvhUnitSizePrecisionBytesMinus1() << std::endl;

    bool bMoreData = true;
    while (bMoreData)
    {
        groupOfFramesList_.resize(groupOfFramesList_.size() + 1);
        auto &groupOfFrames = groupOfFramesList_.back();
        groupOfFrames.getSampleStreamV3CUnit().setSsvhUnitSizePrecisionBytesMinus1(
            ssvu.getSsvhUnitSizePrecisionBytesMinus1());

        // get first 5 nalus;
        groupOfFrames.addV3CUnit(ssvu.getV3CUnit()[0]);
        groupOfFrames.addV3CUnit(ssvu.getV3CUnit()[1]);
        groupOfFrames.addV3CUnit(ssvu.getV3CUnit()[2]);
        groupOfFrames.addV3CUnit(ssvu.getV3CUnit()[3]);
        groupOfFrames.addV3CUnit(ssvu.getV3CUnit()[4]);

        traceSsvu(ssvu);

        pcc::PCCContext context;
        context.setBitstreamStat(bitstreamStat);
        if (!bitstreamReader.decode(ssvu, context))
        {
            return false;
        }
        //bitstreamStat.trace();

        auto &vps = context.getVps();
        context.resizeAtlas(vps.getAtlasCountMinus1() + 1);

        //printf( "profile = %u AtlasCountMinus1 = %u \n", vps.getProfileTierLevel().getProfileReconstructionIdc(),
        //       vps.getAtlasCountMinus1() ); fflush( stdout );

        pcc::PCCDecoder decoder;
#ifdef BITSTREAM_TRACE
        decoder.setLogger(logger);
#endif
        for (uint32_t atlId = 0; atlId < vps.getAtlasCountMinus1() + 1; atlId++)
        {
            context.getAtlas(atlId).allocateVideoFrames(context, 0);
            // first allocating the structures, frames will be added as the V3C
            // units are being decoded ???
            context.setAtlasIndex(atlId);
            decoder.createPatchFrameDataStructure(context);
        }
        // decoder.createPatchFrameDataStructure( context );
        auto &asps = context.getAtlasSequenceParameterSet(0);

        // size_t resolution = = size_t( 1 ) << asps.getLog2PatchPackingBlockSize();
        size_t width = 0, height = 0, bitDepth = 0;
        bool is444 = false;
        pcc_hevc::PccHevcParser hevcParser;

        auto &occupancyVideoBitstream = context.getVideoBitstream(pcc::VIDEO_OCCUPANCY);               
        occupancyVideoBitstream.sampleStreamToByteStream();
        hevcParser.getVideoSize(occupancyVideoBitstream.vector(), width, height, bitDepth, is444);

        std::cout << "video size OCM = " << width << " " << height << std::endl;
        size_t resolution = vps.getFrameWidth(0) / width;
        groupOfFrames.setOcmWidth(vps.getFrameWidth(0) / resolution);
        groupOfFrames.setOcmHeight(vps.getFrameHeight(0) / resolution);
        groupOfFrames.setGeoWidth(vps.getFrameWidth(0));
        groupOfFrames.setGeoHeight(vps.getFrameHeight(0));
        groupOfFrames.setTxtWidth(vps.getFrameWidth(0));
        groupOfFrames.setTxtHeight(vps.getFrameHeight(0));
        groupOfFrames.setNumFrames(context.getFrames().size());
        groupOfFrames.setDts(1000 * numFrames_ / frameRate_);
        numFrames_ += context.getFrames().size();
        groupOfFrames.setPts(1000 * numFrames_ / frameRate_);
        bMoreData = (ssvu.getV3CUnitCount() > 0);
    }

    trace();
    return true;
}

// GroupOfFrames &Sequence::getGof(size_t current)
//{
//    for (auto &gof : groupOfFramesList_)
//    {
//        if (gof.getDts() <= current && current < gof.getPts())
//        {
//            return gof;
//        }
//    }
//    return groupOfFramesList_[0];
//}

void Sequence::trace()
{
    size_t element = 0;
std:
    std::cout << "Sequence: " << numFrames_ << " duration = " << getDuration() << "s : " << filename_.filename() << std::endl;
    for (auto &gof : groupOfFramesList_)
    {
        printf("%2zu / %2zu : NumFrames = %zu / %zu \n",
               element,
               groupOfFramesList_.size(),
               gof.getNumFrames(),
               numFrames_);
        gof.trace();
        element++;
    }
}

void Sequence::traceSsvu(pcc::SampleStreamV3CUnit ssvu)
{
    size_t element = 0;

    std::cout << "traceSsvu :"<< std::endl;

    for (auto &v3cUnit : ssvu.getV3CUnit())
    {
        printf("%2zu / %2zu : type = %d %-12s size = %9zu  \n",
               element,
               ssvu.getV3CUnitCount(),
               v3cUnit.getType(),
               toString(v3cUnit.getType()).c_str(),
               v3cUnit.getSize());
        element++;
    }
}

size_t Sequence::getOcmWidth()
{
    size_t value = 0;
    for (auto &gof : groupOfFramesList_)
    {
        if (value < gof.getOcmWidth())
        {
            value = gof.getOcmWidth();
        }
    }
    return value;
}
size_t Sequence::getOcmHeight()
{
    size_t value = 0;
    for (auto &gof : groupOfFramesList_)
    {
        if (value < gof.getOcmHeight())
        {
            value = gof.getOcmHeight();
        }
    }
    return value;
}
size_t Sequence::getGeoWidth()
{
    size_t value = 0;
    for (auto &gof : groupOfFramesList_)
    {
        if (value < gof.getGeoWidth())
        {
            value = gof.getGeoWidth();
        }
    }
    return value;
}
size_t Sequence::getGeoHeight()
{
    size_t value = 0;
    for (auto &gof : groupOfFramesList_)
    {
        if (value < gof.getGeoHeight())
        {
            value = gof.getGeoHeight();
        }
    }
    return value;
}
size_t Sequence::getTxtWidth()
{
    size_t value = 0;
    for (auto &gof : groupOfFramesList_)
    {
        if (value < gof.getTxtWidth())
        {
            value = gof.getTxtWidth();
        }
    }
    return value;
}
size_t Sequence::getTxtHeight()
{
    size_t value = 0;
    for (auto &gof : groupOfFramesList_)
    {
        if (value < gof.getTxtHeight())
        {
            value = gof.getTxtHeight();
        }
    }
    return value;
}