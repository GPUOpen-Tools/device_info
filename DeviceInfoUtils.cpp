//==============================================================================
// Copyright (c) 2010-2026 Advanced Micro Devices, Inc. All rights reserved.
/// @author AMD Developer Tools Team
/// @file
/// @brief  Device info utils class.
//==============================================================================

#include <algorithm>
#include <cassert>
#include <string_view>
#include <ranges>

#include "DeviceInfoUtils.h"

namespace
{
    AMDTDeviceInfoUtils::DeviceNameTranslatorFunction deviceNameTranslatorFunction = nullptr; ///< The function to call to translate device names.

    constexpr unsigned int kGfxToGdtHwGenConversionFactor = 3; ///< Factor to apply when converting between GFX IP version and GDT_HW_GENERATION.
}

bool AMDTDeviceInfoUtils::GetDeviceInfo(uint32_t deviceID, uint32_t revisionID, GDT_DeviceInfo &deviceInfo)
{
    auto find_device = [&](GDT_GfxCardInfo const &info)
    {
        if (info.m_deviceID != deviceID)
        {
            return false;
        }

        return (kRevisionIdAny == revisionID || info.m_revID == revisionID);
    };

    const auto it = std::ranges::find_if(gs_cardInfo, find_device);
    const bool found = it != gs_cardInfo.end();
    if (found)
    {
        deviceInfo = GetDeviceInfoForAsicType(it->m_asicType);
    }
    return found;
}

/// NOTE: this might not return the correct GDT_DeviceInfo instance, since some devices with the same CAL name might have different GDT_DeviceInfo instances
bool AMDTDeviceInfoUtils::GetDeviceInfo(const char *szCALDeviceName, GDT_DeviceInfo &deviceInfo)
{
    const std::string strDeviceName = TranslateDeviceName(szCALDeviceName);

    auto same_name = [&strDeviceName](GDT_GfxCardInfo const &info)
    { return strDeviceName == info.m_szCALName; };

    const auto it = std::ranges::find_if(gs_cardInfo, same_name);
    const bool found = it != gs_cardInfo.end();
    if (found)
    {
        deviceInfo = GetDeviceInfoForAsicType(it->m_asicType);
    }
    return found;
}

[[nodiscard]] std::optional<uint32_t> AMDTDeviceInfoUtils::GetTotalLdsSizeInBytes(const GDT_HW_GENERATION gen, GDT_DeviceInfo const &info)
{
    static_assert(GDT_HW_GENERATION_LAST == 15, "Update this function!");

    // Anything less than GFX9 is not supported.
    if (gen < GDT_HW_GENERATION_GFX9)
    {
        return std::nullopt;
    }
    // GFX9 to GFX12 all have the same amount of LDS bytes per CU.
    if (gen < GDT_HW_GENERATION_CDNA4)
    {
        constexpr uint32_t kLdsBytesPerCu = 64 * 1024;
        return info.numberCUs() * kLdsBytesPerCu;
    }

    if(gen == GDT_HW_GENERATION_CDNA4)
    {
        constexpr uint32_t kLdsBytesPerCu = 160 * 1024;
        return info.numberCUs() * kLdsBytesPerCu;
    }

    return std::nullopt;
}

bool AMDTDeviceInfoUtils::GetDeviceInfo(uint32_t deviceID, uint32_t revisionID, GDT_GfxCardInfo &cardInfo)
{
    auto find_device = [&](GDT_GfxCardInfo const &info)
    {
        if (info.m_deviceID != deviceID)
        {
            return false;
        }

        return (kRevisionIdAny == revisionID || info.m_revID == revisionID);
    };

    const auto it = std::ranges::find_if(gs_cardInfo, find_device);
    const bool found = it != gs_cardInfo.end();
    if (found)
    {
        cardInfo = *it;
    }
    return found;
}

bool AMDTDeviceInfoUtils::GetDeviceInfo(const char *szCALDeviceName, std::vector<GDT_GfxCardInfo> &cardList)
{
    cardList.clear();

    const std::string strDeviceName = TranslateDeviceName(szCALDeviceName);

    auto same_name = [&strDeviceName](GDT_GfxCardInfo const &info)
    { return strDeviceName == info.m_szMarketingName; };

    std::ranges::copy_if(gs_cardInfo, std::back_inserter(cardList),
                         same_name);

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetDeviceInfoMarketingName(const char *szMarketingDeviceName, std::vector<GDT_GfxCardInfo> &cardList)
{
    cardList.clear();

    const std::string_view marketing_name = szMarketingDeviceName;

    auto same_name = [&marketing_name](GDT_GfxCardInfo const &info)
    { return marketing_name == info.m_szMarketingName; };

    std::ranges::copy_if(gs_cardInfo, std::back_inserter(cardList),
                         same_name);

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::IsAPU(const char *szCALDeviceName, bool &bIsAPU)
{
    const std::string strDeviceName = TranslateDeviceName(szCALDeviceName);

    auto same_name = [&strDeviceName](GDT_GfxCardInfo const &info)
    { return strDeviceName == info.m_szCALName; };

    const auto it = std::ranges::find_if(gs_cardInfo, same_name);
    const bool found = it != gs_cardInfo.end();
    if (found)
    {
        bIsAPU = it->m_bAPU;
    }
    return found;
}

bool AMDTDeviceInfoUtils::IsAPU(uint32_t deviceID, bool &isAPU)
{
    auto find_device = [&](GDT_GfxCardInfo const &info)
    {
        return info.m_deviceID != deviceID;
    };

    const auto it = std::ranges::find_if(gs_cardInfo, find_device);
    const bool found = it != gs_cardInfo.end();
    if (found)
    {
        isAPU = it->m_bAPU;
    }
    return found;
}

bool AMDTDeviceInfoUtils::IsXFamily(uint32_t deviceID, GDT_HW_GENERATION generation, bool &isXFamily)
{
    GDT_HW_GENERATION gen = GDT_HW_GENERATION_NONE;

    if (GetHardwareGeneration(deviceID, gen))
    {
        isXFamily = gen == generation;
        return true;
    }
    else
    {
        return false;
    }
}

bool AMDTDeviceInfoUtils::IsGfx12Family(uint32_t deviceID, bool &isGfx12)
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_GFX12, isGfx12);
}

bool AMDTDeviceInfoUtils::IsGfx11Family(uint32_t deviceID, bool &isGfx11)
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_GFX11, isGfx11);
}

bool AMDTDeviceInfoUtils::IsGfx10Family(uint32_t deviceID, bool &isGfx10)
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_GFX10, isGfx10);
}

bool AMDTDeviceInfoUtils::IsGfx9Family(uint32_t deviceID, bool &isGfx9)
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_GFX9, isGfx9);
}

bool AMDTDeviceInfoUtils::IsVIFamily(uint32_t deviceID, bool &isVI)
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_VOLCANICISLAND, isVI);
}

bool AMDTDeviceInfoUtils::IsCIFamily(uint32_t deviceID, bool &isCI)
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_SEAISLAND, isCI);
}

bool AMDTDeviceInfoUtils::IsSIFamily(uint32_t deviceID, bool &isSI)
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_SOUTHERNISLAND, isSI);
}

bool AMDTDeviceInfoUtils::GetHardwareGeneration(uint32_t deviceID, GDT_HW_GENERATION &gen)
{
    // revId not needed here, since all revs will have the same hardware family
    auto find_device = [&](GDT_GfxCardInfo const &info)
    {
        return info.m_deviceID != deviceID;
    };

    const auto it = std::ranges::find_if(gs_cardInfo, find_device);
    const bool found = it != gs_cardInfo.end();
    if (found)
    {
        gen = it->m_generation;
    }
    return found;
}

bool AMDTDeviceInfoUtils::GetHardwareGeneration(const char *szCALDeviceName, GDT_HW_GENERATION &gen)
{
    const std::string strDeviceName = TranslateDeviceName(szCALDeviceName);

    auto same_name = [&strDeviceName](GDT_GfxCardInfo const &info)
    { return strDeviceName == info.m_szCALName; };

    const auto it = std::ranges::find_if(gs_cardInfo, same_name);
    const bool found = it != gs_cardInfo.end();
    if (found)
    {
        gen = it->m_generation;
    }
    return found;
}

void AMDTDeviceInfoUtils::GetAllCards(std::vector<GDT_GfxCardInfo> &cardList)
{
    cardList.clear();
    cardList.reserve(gs_cardInfo.size());
    std::ranges::copy(gs_cardInfo, std::back_inserter(cardList));
}

bool AMDTDeviceInfoUtils::GetAllCardsWithName(const char *szCALDeviceName, std::vector<GDT_GfxCardInfo> &cardList)
{
    return GetDeviceInfo(szCALDeviceName, cardList);
}

bool AMDTDeviceInfoUtils::GetAllCardsInHardwareGeneration(GDT_HW_GENERATION gen, std::vector<GDT_GfxCardInfo> &cardList)
{
    cardList.clear();

    auto same_gen = [&gen](GDT_GfxCardInfo const &info)
    { return gen == info.m_generation; };

    std::ranges::copy_if(gs_cardInfo, std::back_inserter(cardList),
                         same_gen);

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetAllCardsWithDeviceId(uint32_t deviceID, std::vector<GDT_GfxCardInfo> &cardList)
{
    cardList.clear();

    auto same_device_id = [&deviceID](GDT_GfxCardInfo const &info)
    { return deviceID == info.m_deviceID; };

    std::ranges::copy_if(gs_cardInfo, std::back_inserter(cardList),
                         same_device_id);

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetAllCardsWithAsicType(GDT_HW_ASIC_TYPE asicType, std::vector<GDT_GfxCardInfo> &cardList)
{
    cardList.clear();

    auto same_asic = [&asicType](GDT_GfxCardInfo const &info)
    { return asicType == info.m_asicType; };

    std::ranges::copy_if(gs_cardInfo, std::back_inserter(cardList),
                         same_asic);

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetHardwareGenerationDisplayName(GDT_HW_GENERATION gen, std::string &strGenerationDisplayName)
{
    static constexpr std::string_view s_SI_FAMILY_NAME    = "Graphics IP v6";
    static constexpr std::string_view s_CI_FAMILY_NAME    = "Graphics IP v7";
    static constexpr std::string_view s_VI_FAMILY_NAME    = "Graphics IP v8";
    static constexpr std::string_view s_GFX9_FAMILY_NAME  = "Vega";
    static constexpr std::string_view s_RDNA_FAMILY_NAME  = "RDNA";
    static constexpr std::string_view s_RDNA2_FAMILY_NAME = "RDNA2";
    static constexpr std::string_view s_RDNA3_FAMILY_NAME = "RDNA3";
    static constexpr std::string_view s_RDNA4_FAMILY_NAME = "RDNA4";
    static constexpr std::string_view s_CDNA_FAMILY_NAME  = "CDNA";
    static constexpr std::string_view s_CDNA2_FAMILY_NAME = "CDNA2";
    static constexpr std::string_view s_CDNA3_FAMILY_NAME = "CDNA3";
    static constexpr std::string_view s_CDNA4_FAMILY_NAME = "CDNA4";

    bool retVal = true;

    switch (gen)
    {
        case GDT_HW_GENERATION_SOUTHERNISLAND:
            strGenerationDisplayName = s_SI_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_SEAISLAND:
            strGenerationDisplayName = s_CI_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_VOLCANICISLAND:
            strGenerationDisplayName = s_VI_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_GFX9:
            strGenerationDisplayName = s_GFX9_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_GFX10:
            strGenerationDisplayName = s_RDNA_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_GFX103:
            strGenerationDisplayName = s_RDNA2_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_GFX11:
            strGenerationDisplayName = s_RDNA3_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_GFX12:
            strGenerationDisplayName = s_RDNA4_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_CDNA:
            strGenerationDisplayName = s_CDNA_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_CDNA2:
            strGenerationDisplayName = s_CDNA2_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_CDNA3:
            strGenerationDisplayName = s_CDNA3_FAMILY_NAME;
            break;

        case GDT_HW_GENERATION_CDNA4:
            strGenerationDisplayName = s_CDNA4_FAMILY_NAME;
            break;

        default:
            strGenerationDisplayName.clear();
            assert(false);
            retVal = false;
            break;
    }

    return retVal;
}

std::string AMDTDeviceInfoUtils::TranslateDeviceName(const char *strDeviceName)
{
    std::string retVal(strDeviceName);

    if (retVal.compare("gfx901") == 0) // some gfx900 boards are identified as gfx901 by some drivers
    {
        retVal = "gfx900";
    }

    if (retVal.compare("gfx903") == 0) // some gfx902 APUs are identified as gfx903 by some drivers
    {
        retVal = "gfx902";
    }

    if (retVal.compare("gfx905") == 0)  // some gfx904 boards are identified as gfx905
    {
        retVal = "gfx904";
    }

    if (retVal.compare("gfx907") == 0)  // some gfx906 boards are identified as gfx907
    {
        retVal = "gfx906";
    }

    if (nullptr != deviceNameTranslatorFunction)
    {
        retVal = deviceNameTranslatorFunction(retVal.c_str());
    }

    return retVal;
}

bool AMDTDeviceInfoUtils::GfxIPVerToHwGeneration(uint32_t gfxIPVer, GDT_HW_GENERATION &hwGen)
{
    hwGen = static_cast<GDT_HW_GENERATION>(gfxIPVer - kGfxToGdtHwGenConversionFactor);

    bool retVal = hwGen >= GDT_HW_GENERATION_FIRST_AMD && hwGen < GDT_HW_GENERATION_LAST;

    if (!retVal)
    {
        hwGen = GDT_HW_GENERATION_NONE;
    }

    return retVal;
}

bool AMDTDeviceInfoUtils::HwGenerationToGfxIPVer(GDT_HW_GENERATION hwGen, uint32_t &gfxIPVer)
{
    gfxIPVer = 0;

    bool retVal = hwGen >= GDT_HW_GENERATION_FIRST_AMD && hwGen < GDT_HW_GENERATION_LAST;

    if (retVal)
    {
        gfxIPVer = static_cast<uint32_t>(hwGen) + kGfxToGdtHwGenConversionFactor;
    }

    return retVal;
}

void AMDTDeviceInfoUtils::SetDeviceNameTranslator(DeviceNameTranslatorFunction func)
{
    deviceNameTranslatorFunction = func;
}
