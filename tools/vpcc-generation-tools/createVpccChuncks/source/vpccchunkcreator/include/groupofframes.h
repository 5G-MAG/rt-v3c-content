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
#include "PCCContext.h"
#include "PCCFrameContext.h"

class GroupOfFrames
{
public:
    GroupOfFrames();
    ~GroupOfFrames();

    void addV3CUnit(pcc::V3CUnit &v3cUnit) { ssvu_.getV3CUnit().push_back(v3cUnit); }
    void trace();

    pcc::SampleStreamV3CUnit &getSampleStreamV3CUnit() { return ssvu_; }
    size_t getNumFrames() { return numFrames_; }
    size_t getPts() { return pts_; }
    size_t getDts() { return dts_; }
    size_t getOcmWidth() { return ocmWidth_; }
    size_t getGeoWidth() { return geoWidth_; }
    size_t getTxtWidth() { return txtWidth_; }
    size_t getOcmHeight() { return ocmHeight_; }
    size_t getGeoHeight() { return geoHeight_; }
    size_t getTxtHeight() { return txtHeight_; }

    void setOcmWidth(size_t value) { ocmWidth_ = value; }
    void setGeoWidth(size_t value) { geoWidth_ = value; }
    void setTxtWidth(size_t value) { txtWidth_ = value; }
    void setOcmHeight(size_t value) { ocmHeight_ = value; }
    void setGeoHeight(size_t value) { geoHeight_ = value; }
    void setTxtHeight(size_t value) { txtHeight_ = value; }
    void setNumFrames(size_t numFrames) { numFrames_ = numFrames; }
    void setPts(size_t pts) { pts_ = pts; }
    void setDts(size_t dts) { dts_ = dts; }

private:
    size_t numFrames_;
    size_t dts_;
    size_t pts_;
    size_t ocmWidth_;
    size_t ocmHeight_;
    size_t geoWidth_;
    size_t geoHeight_;
    size_t txtWidth_;
    size_t txtHeight_;
    pcc::SampleStreamV3CUnit ssvu_;
    static std::list<std::string> chunk_;
};
