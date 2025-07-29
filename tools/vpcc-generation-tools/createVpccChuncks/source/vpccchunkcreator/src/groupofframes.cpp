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
#include "groupofframes.h"
#include "PCCBitstream.h"
#include "PCCBitstreamReader.h"
#include "PCCBitstreamWriter.h"
#include "PCCCommon.h"
#include "PCCContext.h"
#include "PCCDecoder.h"
#include "PCCFrameContext.h"

GroupOfFrames::GroupOfFrames()
    : numFrames_(0),
      dts_(0),
      pts_(0),
      ocmWidth_(0),
      ocmHeight_(0),
      geoWidth_(0),
      geoHeight_(0),
      txtWidth_(0),
      txtHeight_(0)
{
}
GroupOfFrames::~GroupOfFrames() {}

void GroupOfFrames::trace()
{
    size_t element = 0;
    for (auto &v3cUnit : ssvu_.getV3CUnit())
    {
        printf("  %2zu / %2zu : type = %d %-12s size = %9zu  dts = %9zu  pts = %9zu ( %4zux%4zu %4zux%4zu %4zux%4zu )\n",
               element,
               ssvu_.getV3CUnitCount(),
               v3cUnit.getType(),
               toString(v3cUnit.getType()).c_str(),
               v3cUnit.getSize(),
               dts_,
               pts_,
               ocmWidth_,
               ocmHeight_,
               geoWidth_,
               geoHeight_,
               txtWidth_,
               txtHeight_);
        element++;
    }
}
