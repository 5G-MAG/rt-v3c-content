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
#pragma once

#include "PCCCommon.h"
#include "groupofframes.h"
#include <filesystem>

class Sequence
{
public:
    Sequence();
    ~Sequence();
    void clear();
    size_t writeChunk(const std::filesystem::path &inputFilePath, const size_t frameRate, const std::filesystem::path outputDir);

private:
    bool read(const std::filesystem::path &filename, const size_t frameRate);
    void traceSsvu(pcc::SampleStreamV3CUnit ssvu);
    void trace();
    size_t getNumFrames() { return numFrames_; }
    std::vector<GroupOfFrames> &getGroupOfFrameList() { return groupOfFramesList_; }
    size_t getDuration() { return numFrames_ / frameRate_; }
    //GroupOfFrames &getGof(size_t current);
    size_t getOcmWidth();
    size_t getOcmHeight();
    size_t getGeoWidth();
    size_t getGeoHeight();
    size_t getTxtWidth();
    size_t getTxtHeight();

    size_t numFrames_;
    size_t frameRate_;
    std::vector<GroupOfFrames> groupOfFramesList_;
    std::filesystem::path filename_;
};