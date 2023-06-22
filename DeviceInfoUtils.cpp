//==============================================================================
// Copyright (c) 2010-2022 Advanced Micro Devices, Inc. All rights reserved.
/// @author AMD Developer Tools Team
/// @file
/// @brief Device info utils class.
//==============================================================================

#ifdef _WIN32
#include <Windows.h>
#endif
#ifdef _LINUX
#include <dlfcn.h>
#endif

#include <cassert>
#include "DeviceInfoUtils.h"

using namespace std;

AMDTDeviceInfoUtils* AMDTDeviceInfoUtils::ms_pInstance = nullptr;
AMDTDeviceInfoManager* AMDTDeviceInfoManager::ms_pInstance = nullptr;

AMDTDeviceInfoManager::AMDTDeviceInfoManager()
{
    AMDTDeviceInfoUtils* pDeviceInfoUtils = AMDTDeviceInfoUtils::Instance();

    for (size_t i = 0; i < gs_cardInfoSize; ++i)
    {
        pDeviceInfoUtils->AddDevice(gs_cardInfo[i]);
    }

    for (size_t i = 0; i < gs_deviceInfoSize; ++i)
    {
        pDeviceInfoUtils->AddDeviceInfo(static_cast<GDT_HW_ASIC_TYPE>(i), gs_deviceInfo[i]);
    }
}

AMDTDeviceInfoUtils* AMDTDeviceInfoUtils::Instance()
{
    if (nullptr == ms_pInstance)
    {
        ms_pInstance = new AMDTDeviceInfoUtils();
        AMDTDeviceInfoManager::Instance();
    }
    return ms_pInstance;
}

void AMDTDeviceInfoUtils::DeleteInstance()
{
    delete ms_pInstance;
    ms_pInstance = nullptr;

    AMDTDeviceInfoManager::DeleteInstance();
}

bool AMDTDeviceInfoUtils::GetDeviceInfo(size_t deviceID, size_t revisionID, GDT_DeviceInfo& deviceInfo) const
{
    bool found = false;

    auto matches = m_deviceIDMap.equal_range(deviceID);
    for (auto it = matches.first; it != matches.second && !found; ++it)
    {
        size_t thisRevId = (*it).second.m_revID;

        if (REVISION_ID_ANY == revisionID || thisRevId == revisionID)
        {
            for (auto itr = m_asicTypeDeviceInfoMap.find((*it).second.m_asicType); itr != m_asicTypeDeviceInfoMap.end() && !found; ++itr)
            {
                deviceInfo = itr->second;

                if (deviceInfo.m_deviceInfoValid)
                {
                    found = true;
                }
            }
        }
    }

    return found;
}

/// NOTE: this might not return the correct GDT_DeviceInfo instance, since some devices with the same CAL name might have different GDT_DeviceInfo instances
bool AMDTDeviceInfoUtils::GetDeviceInfo(const char* szCALDeviceName, GDT_DeviceInfo& deviceInfo) const
{
    std::string strDeviceName = TranslateDeviceName(szCALDeviceName);
    auto matches = m_deviceNameMap.equal_range(strDeviceName.c_str());

    if (matches.first != matches.second)
    {
        auto deviceIt = m_asicTypeDeviceInfoMap.find(matches.first->second.m_asicType);

        if (m_asicTypeDeviceInfoMap.end() != deviceIt)
        {
            deviceInfo = deviceIt->second;

            if (deviceInfo.m_deviceInfoValid)
            {
                return true;
            }
        }
    }

    return false;
}

bool AMDTDeviceInfoUtils::GetDeviceInfo(size_t deviceID, size_t revisionID, GDT_GfxCardInfo& cardInfo) const
{
    bool found = false;

    assert(m_deviceIDMap.size() != 0);

    auto matches = m_deviceIDMap.equal_range(deviceID);
    for (auto it = matches.first; it != matches.second && !found; ++it)
    {
        size_t thisRevId = (*it).second.m_revID;

        if (REVISION_ID_ANY == revisionID || thisRevId == revisionID)
        {
            cardInfo = (*it).second;
            found = true;
        }
    }

    return found;
}

bool AMDTDeviceInfoUtils::GetDeviceInfo(const char* szCALDeviceName, vector<GDT_GfxCardInfo>& cardList) const
{
    std::string strDeviceName = TranslateDeviceName(szCALDeviceName);

    cardList.clear();
    auto matches = m_deviceNameMap.equal_range(strDeviceName.c_str());

    for (auto it = matches.first; it != matches.second; ++it)
    {
        cardList.push_back((*it).second);
    }

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetDeviceInfoMarketingName(const char* szMarketingDeviceName, vector<GDT_GfxCardInfo>& cardList) const
{
    cardList.clear();
    auto matches = m_deviceMarketingNameMap.equal_range(szMarketingDeviceName);

    for (auto it = matches.first; it != matches.second; ++it)
    {
        cardList.push_back((*it).second);
    }

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::IsAPU(const char* szCALDeviceName, bool& bIsAPU) const
{
    std::string strDeviceName = TranslateDeviceName(szCALDeviceName);
    auto matches = m_deviceNameMap.equal_range(strDeviceName.c_str());

    if (matches.first != matches.second)
    {
        bIsAPU = matches.first->second.m_bAPU;
        return true;
    }
    else
    {
        return false;
    }
}

bool AMDTDeviceInfoUtils::IsAPU(size_t deviceID, bool& isAPU) const
{
    auto matches = m_deviceIDMap.equal_range(deviceID);

    if (matches.first != matches.second)
    {
        isAPU = matches.first->second.m_bAPU;
        return true;
    }
    else
    {
        return false;
    }
}

bool AMDTDeviceInfoUtils::IsXFamily(size_t deviceID, GDT_HW_GENERATION generation, bool& isXFamily) const
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

bool AMDTDeviceInfoUtils::IsGfx11Family(size_t deviceID, bool& isGfx11) const
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_GFX11, isGfx11);
}

bool AMDTDeviceInfoUtils::IsGfx10Family(size_t deviceID, bool& isGfx10) const
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_GFX10, isGfx10);
}

bool AMDTDeviceInfoUtils::IsGfx9Family(size_t deviceID, bool& isGfx9) const
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_GFX9, isGfx9);
}

bool AMDTDeviceInfoUtils::IsVIFamily(size_t deviceID, bool& isVI) const
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_VOLCANICISLAND, isVI);
}

bool AMDTDeviceInfoUtils::IsCIFamily(size_t deviceID, bool& isCI) const
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_SEAISLAND, isCI);
}

bool AMDTDeviceInfoUtils::IsSIFamily(size_t deviceID, bool& isSI) const
{
    return IsXFamily(deviceID, GDT_HW_GENERATION_SOUTHERNISLAND, isSI);
}

bool AMDTDeviceInfoUtils::GetHardwareGeneration(size_t deviceID, GDT_HW_GENERATION& gen) const
{
    // revId not needed here, since all revs will have the same hardware family
    auto matches = m_deviceIDMap.equal_range(deviceID);

    if (matches.first != matches.second)
    {
        gen = matches.first->second.m_generation;
        return true;
    }
    else
    {
        return false;
    }
}

bool AMDTDeviceInfoUtils::GetHardwareGeneration(const char* szCALDeviceName, GDT_HW_GENERATION& gen) const
{
    std::string strDeviceName = TranslateDeviceName(szCALDeviceName);
    auto matches = m_deviceNameMap.equal_range(strDeviceName.c_str());

    if (matches.first != matches.second)
    {
        gen = matches.first->second.m_generation;
        return true;
    }
    else
    {
        return false;
    }
}

void AMDTDeviceInfoUtils::GetAllCards(std::vector<GDT_GfxCardInfo>& cardList) const
{
    cardList.clear();
    cardList.reserve(gs_cardInfoSize);

    for(size_t i = 0ULL; i < gs_cardInfoSize; ++i)
    {
        cardList.push_back(gs_cardInfo[i]);
    }
}

bool AMDTDeviceInfoUtils::GetAllCardsWithName(const char* szCALDeviceName, std::vector<GDT_GfxCardInfo>& cardList) const
{
    return GetDeviceInfo(szCALDeviceName, cardList);
}

bool AMDTDeviceInfoUtils::GetAllCardsInHardwareGeneration(GDT_HW_GENERATION gen, std::vector<GDT_GfxCardInfo>& cardList) const
{
    cardList.clear();
    auto matches = m_deviceHwGenerationMap.equal_range(gen);

    for (auto it = matches.first; it != matches.second; ++it)
    {
        cardList.push_back((*it).second);
    }

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetAllCardsWithDeviceId(size_t deviceID, std::vector<GDT_GfxCardInfo>& cardList) const
{
    cardList.clear();
    auto matches = m_deviceIDMap.equal_range(deviceID);

    for (auto it = matches.first; it != matches.second; ++it)
    {
        cardList.push_back((*it).second);
    }

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetAllCardsWithAsicType(GDT_HW_ASIC_TYPE asicType, std::vector<GDT_GfxCardInfo>& cardList) const
{
    cardList.clear();
    auto matches = m_asicTypeCardInfoMap.equal_range(asicType);

    for (auto it = matches.first; it != matches.second; ++it)
    {
        cardList.push_back((*it).second);
    }

    return !cardList.empty();
}

bool AMDTDeviceInfoUtils::GetHardwareGenerationDisplayName(GDT_HW_GENERATION gen, std::string& strGenerationDisplayName) const
{
    static const std::string s_SI_FAMILY_NAME    = "Graphics IP v6";
    static const std::string s_CI_FAMILY_NAME    = "Graphics IP v7";
    static const std::string s_VI_FAMILY_NAME    = "Graphics IP v8";
    static const std::string s_GFX9_FAMILY_NAME  = "Vega";
    static const std::string s_RDNA_FAMILY_NAME  = "RDNA";
    static const std::string s_RDNA2_FAMILY_NAME = "RDNA2";
    static const std::string s_RDNA3_FAMILY_NAME = "RDNA3";

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

        default:
            strGenerationDisplayName.clear();
            assert(false);
            retVal = false;
            break;
    }

    return retVal;
}

std::string AMDTDeviceInfoUtils::TranslateDeviceName(const char* strDeviceName) const
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

    if (nullptr != m_pDeviceNameTranslatorFunction)
    {
        retVal = m_pDeviceNameTranslatorFunction(retVal.c_str());
    }

    return retVal;
}

bool AMDTDeviceInfoUtils::GfxIPVerToHwGeneration(size_t gfxIPVer, GDT_HW_GENERATION& hwGen) const
{
    hwGen = static_cast<GDT_HW_GENERATION>(gfxIPVer - ms_gfxToGdtHwGenConversionFactor);

    bool retVal = hwGen >= GDT_HW_GENERATION_FIRST_AMD && hwGen < GDT_HW_GENERATION_LAST;

    if (!retVal)
    {
        hwGen = GDT_HW_GENERATION_NONE;
    }

    return retVal;
}

bool AMDTDeviceInfoUtils::HwGenerationToGfxIPVer(GDT_HW_GENERATION hwGen, size_t& gfxIPVer) const
{
    gfxIPVer = 0;

    bool retVal = hwGen >= GDT_HW_GENERATION_FIRST_AMD && hwGen < GDT_HW_GENERATION_LAST;

    if (retVal)
    {
        gfxIPVer = static_cast<size_t>(hwGen) + ms_gfxToGdtHwGenConversionFactor;
    }

    return retVal;
}

void AMDTDeviceInfoUtils::AddDeviceInfo(GDT_HW_ASIC_TYPE asicType, const GDT_DeviceInfo& deviceInfo)
{
    if (m_asicTypeDeviceInfoMap.end() == m_asicTypeDeviceInfoMap.find(asicType))
    {
        m_asicTypeDeviceInfoMap.insert(ASICTypeDeviceInfoMapPair(asicType, deviceInfo));
    }
    else
    {
        m_asicTypeDeviceInfoMap[asicType] = deviceInfo;
    }
}

void AMDTDeviceInfoUtils::AddDevice(const GDT_GfxCardInfo& cardInfo)
{
    m_deviceIDMap.insert(DeviceIDMapPair(cardInfo.m_deviceID, cardInfo));
    m_asicTypeCardInfoMap.insert(ASICTypeCardInfoMapPair(cardInfo.m_asicType, cardInfo));
    m_deviceNameMap.insert(DeviceNameMapPair(cardInfo.m_szCALName, cardInfo));
    m_deviceMarketingNameMap.insert(DeviceNameMapPair(cardInfo.m_szMarketingName, cardInfo));
    m_deviceHwGenerationMap.insert(DeviceHWGenerationMapPair(cardInfo.m_generation, cardInfo));
}

void AMDTDeviceInfoUtils::RemoveDevice(const GDT_GfxCardInfo& cardInfo)
{
    for (auto it = m_deviceIDMap.begin(); it != m_deviceIDMap.end(); ++it)
    {
        if (it->first == cardInfo.m_deviceID && it->second.m_revID == cardInfo.m_revID)
        {
            m_deviceIDMap.erase(it);
            break;
        }
    }

    for (auto it = m_asicTypeCardInfoMap.begin(); it != m_asicTypeCardInfoMap.end(); ++it)
    {
        if (it->second.m_deviceID == cardInfo.m_deviceID && it->second.m_revID == cardInfo.m_revID)
        {
            m_asicTypeCardInfoMap.erase(it);
            break;
        }
    }

    for (auto it = m_deviceNameMap.begin(); it != m_deviceNameMap.end(); ++it)
    {
        if (it->second.m_deviceID == cardInfo.m_deviceID && it->second.m_revID == cardInfo.m_revID)
        {
            m_deviceNameMap.erase(it);
            break;
        }
    }

    for (auto it = m_deviceMarketingNameMap.begin(); it != m_deviceMarketingNameMap.end(); ++it)
    {
        if (it->second.m_deviceID == cardInfo.m_deviceID && it->second.m_revID == cardInfo.m_revID)
        {
            m_deviceMarketingNameMap.erase(it);
            break;
        }
    }

    for (auto it = m_deviceHwGenerationMap.begin(); it != m_deviceHwGenerationMap.end(); ++it)
    {
        if (it->first == cardInfo.m_generation && it->second.m_deviceID == cardInfo.m_deviceID && it->second.m_revID == cardInfo.m_revID)
        {
            m_deviceHwGenerationMap.erase(it);
            break;
        }
    }
}

void AMDTDeviceInfoUtils::SetDeviceNameTranslator(DeviceNameTranslatorFunction deviceNametranslatorFunction)
{
    m_pDeviceNameTranslatorFunction = deviceNametranslatorFunction;
}
