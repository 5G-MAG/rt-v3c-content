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
#include <iostream>
#include "PCCBitstream.h"
#include "sequence.h"
#include "vpccinputparser.h"
#include <filesystem>

static const std::string usage = R"(
NAME
VpccChunkGenerator - Application to create V-PCC (local) chunk

USAGE
VpccChunkGenerator -i vpcc.bin -fps 30 -o chunkdir

OPTIONS
--help
    Display this help message.
)";

int main(int argc, char **argv)
{
    InputParser parser(argc, argv);

    if (parser.cmdOptionExists("-h"))
    {
        std::cerr << "\033[31m" << usage << std::endl;
        return -1;
    }

    if ( !parser.cmdOptionExists("-i") || !parser.cmdOptionExists("-fps") || !parser.cmdOptionExists("-o"))
    {
        std::cerr << "\033[31m" << usage << std::endl;
        return -1;
    }

    const std::string& inputFilePath = parser.getCmdOption("-i");
    const size_t fps = stoi(parser.getCmdOption("-fps"));
    const std::string& outputDir = parser.getCmdOption("-o");

    std::ifstream inputFile(inputFilePath);
    if (!inputFile.is_open())
    {
        std::cerr << "\033[31m" << "Encoded file not found:" << inputFilePath << std::endl;
        return -1;
    }

    Sequence sequence;
    const std::filesystem::path inPath = std::filesystem::absolute(inputFilePath);
    const std::filesystem::path outPath = std::filesystem::absolute(outputDir);
    auto num = sequence.writeChunk(inPath, fps, outPath);
    
    return num;
}


