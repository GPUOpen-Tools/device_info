//==============================================================================
// Copyright (c) 2010-2026 Advanced Micro Devices, Inc. All rights reserved.
/// @author AMD Developer Tools Team
/// @file
/// @brief Helper functions for common device info operations.
//==============================================================================

#ifndef DEVICE_INFO_DEVICE_INFO_UTILS_H_
#define DEVICE_INFO_DEVICE_INFO_UTILS_H_

#include <optional>
#include <string>
#include <vector>
#include "DeviceInfo.h"

//------------------------------------------------------------------------------------
/// Device info utils
//------------------------------------------------------------------------------------
namespace AMDTDeviceInfoUtils
{
    constexpr uint32_t kRevisionIdAny = 0xFFFFFFFF; ///< Ignore revision id when looking up device Id.

    /// Function pointer type for a function that will translate device names
    using DeviceNameTranslatorFunction = std::string (*)(const char *strDeviceName);

    /// Sets the Device name translator function
    /// \param func the function to use to translate device names
    void SetDeviceNameTranslator(DeviceNameTranslatorFunction func);

    /// Get device info from device ID
    /// \param[in] deviceID Device ID
    /// \param[in] revisionID RevisionID, pass kRevisionIdAny if revision ID is not important.
    /// \param[out] deviceInfo Output device info if device id is found.
    /// \return True if device info is found
    [[nodiscard]] bool GetDeviceInfo(uint32_t deviceID, uint32_t revisionID, GDT_DeviceInfo &deviceInfo);

    /// Get device info from CAL name string
    /// NOTE: this might not return the correct GDT_DeviceInfo instance, since some devices with the same CAL name might have different GDT_DeviceInfo instances
    /// \param[in] szCALDeviceName CAL device name string
    /// \param[out] deviceInfo Output device info if device id is found.
    /// \return True if device info is found
    [[nodiscard]] bool GetDeviceInfo(const char *szCALDeviceName, GDT_DeviceInfo &deviceInfo);

    /// Get total LDS size in bytes.
    /// \param[in] gen Hardware generation
    /// \return Total LDS size in bytes if found.
    [[nodiscard]] std::optional<uint32_t> GetTotalLdsSizeInBytes(const GDT_HW_GENERATION gen, GDT_DeviceInfo const &info);

    /// Get Graphics Card Info.
    /// \param[in] deviceID Device ID
    /// \param[in] revisionID Revision ID, pass kRevisionIdAny if revision ID is not important.
    /// \param[out] cardInfo Output graphics card info if device id is found.
    /// \return True if info for deviceID is found
    [[nodiscard]] bool GetDeviceInfo(uint32_t deviceID, uint32_t revisionID, GDT_GfxCardInfo &cardInfo);

    /// Get a vector of Graphics Card Info.
    /// \param[in] szCALDeviceName CAL device name string
    /// \param[out] cardList Output vector of graphics card info.
    /// \return True if any graphics card info is found for CAL device name.
    [[nodiscard]] bool GetDeviceInfo(const char *szCALDeviceName, std::vector<GDT_GfxCardInfo> &cardList);

    /// Get a vector of Graphics Card Info.
    /// \param[in]  szMarketingDeviceName Marketing device name string
    /// \param[out] cardList Output vector of graphics card info.
    /// \return True if any graphics card info is found for Marketing device name.
    [[nodiscard]] bool GetDeviceInfoMarketingName(const char *szMarketingDeviceName, std::vector<GDT_GfxCardInfo> &cardList);

    /// Query whether or not input device is APU or not
    /// \param[in] szCALDeviceName CAL device name string
    /// \param[out] bIsAPU flag indicating whether or not the specified device is an APU
    /// \return True if device info is found
    [[nodiscard]] bool IsAPU(const char *szCALDeviceName, bool &bIsAPU);

    /// Query whether or not input device is APU or not
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isAPU flag indicating whether or not the specified device is an APU
    /// \return True if device info is found
    [[nodiscard]] bool IsAPU(uint32_t deviceID, bool &isAPU);

    /// Get hardware generation from device ID
    /// \param[in] deviceID Device id
    /// \param[out] gen Hardware generation
    /// \return True if device info is found
    [[nodiscard]] bool GetHardwareGeneration(uint32_t deviceID, GDT_HW_GENERATION &gen);

    /// Get hardware generation from device name
    /// \param[in] szCALDeviceName Device name
    /// \param[out] gen Hardware generation
    /// \return True if device info is found
    [[nodiscard]] bool GetHardwareGeneration(const char *szCALDeviceName, GDT_HW_GENERATION &gen);

    /// Get all cards in all hardware generations.
    /// \param[out] cardList Output vector of all graphics card info.
    void GetAllCards(std::vector<GDT_GfxCardInfo> &cardList);

    /// Get all cards with the specified CAL device name -- this a wrapper around one of the GetDeviceInfo overloads
    /// \param[in] szCALDeviceName CAL device name string
    /// \param[out] cardList Output vector of graphics card info.
    /// \return True if any graphics card info is found for CAL device name.
    [[nodiscard]] bool GetAllCardsWithName(const char *szCALDeviceName, std::vector<GDT_GfxCardInfo> &cardList);

    /// Get all cards from the specified hardware generation
    /// \param[in] gen Hardware generation
    /// \param[out] cardList Output vector of graphics card info.
    /// \return true if successful, false otherwise
    [[nodiscard]] bool GetAllCardsInHardwareGeneration(GDT_HW_GENERATION gen, std::vector<GDT_GfxCardInfo> &cardList);

    /// Get all cards with the specified device id
    /// \param[in] deviceID DeviceId
    /// \param[out] cardList Output vector of graphics card info.
    /// \return true if successful, false otherwise
    [[nodiscard]] bool GetAllCardsWithDeviceId(uint32_t deviceID, std::vector<GDT_GfxCardInfo> &cardList);

    /// Get all cards with the specified ASIC type
    /// \param[in] asicType ASICType
    /// \param[out] cardList Output vector of graphics card info.
    /// \return true if successful, false otherwise
    [[nodiscard]] bool GetAllCardsWithAsicType(GDT_HW_ASIC_TYPE asicType, std::vector<GDT_GfxCardInfo> &cardList);

    /// Get hardware generation display name
    /// \param[in] gen Hardware generation
    /// \param[out] strGenerationDisplayName the display name for the specified hardware generation
    /// \return true if successful, false otherwise
    [[nodiscard]] bool GetHardwareGenerationDisplayName(GDT_HW_GENERATION gen, std::string &strGenerationDisplayName);

    /// Determine if the specified device is a member of the specified family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[in] generation Generation enum
    /// \param[out] bRes Set to true if input device name is a specified family card
    /// \return false if device name is not found
    [[nodiscard]] inline bool IsXFamily(const char *szCALDeviceName, GDT_HW_GENERATION generation, bool &bRes)
    {
        GDT_HW_GENERATION gen = GDT_HW_GENERATION_NONE;

        if (GetHardwareGeneration(szCALDeviceName, gen))
        {
            bRes = gen == generation;
            return true;
        }
        else
        {
            return false;
        }
    }

    /// Determine if the specified device is a member of the specified family
    /// \param[in] deviceID the PCIE device ID
    /// \param[in] generation Generation enum
    /// \param[out] isXFamily Set to true if input device name is a specified family card
    /// \return false if device ID is not found
    [[nodiscard]] bool IsXFamily(uint32_t deviceID, GDT_HW_GENERATION generation, bool &isXFamily);

    /// Determine if the specified device is a member of the Gfx12 family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsGfx12 Set to true if input device name is a member of the Gfx12 family
    /// \return false if device name is not found
    [[nodiscard]] inline bool IsGfx12Family(const char *szCALDeviceName, bool &bIsGfx12)
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_GFX12, bIsGfx12);
    }

    /// Determine if the specified device is a member of the Gfx12 family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isGfx12 Set to true if input device name is a member of the Gfx12 family
    /// \return false if device ID is not found
    [[nodiscard]] bool IsGfx12Family(uint32_t deviceID, bool &isGfx12);

    /// Determine if the specified device is a member of the Gfx11 family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsGfx11 Set to true if input device name is a member of the Gfx11 family
    /// \return false if device name is not found
    [[nodiscard]] inline bool IsGfx11Family(const char *szCALDeviceName, bool &bIsGfx11)
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_GFX11, bIsGfx11);
    }

    /// Determine if the specified device is a member of the Gfx11 family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isGfx11 Set to true if input device name is a member of the Gfx11 family
    /// \return false if device ID is not found
    [[nodiscard]] bool IsGfx11Family(uint32_t deviceID, bool &isGfx11);

    /// Determine if the specified device is a member of the Gfx10 family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsGfx10 Set to true if input device name is a member of the Gfx10 family
    /// \return false if device name is not found
    [[nodiscard]] inline bool IsGfx10Family(const char *szCALDeviceName, bool &bIsGfx10)
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_GFX10, bIsGfx10);
    }

    /// Determine if the specified device is a member of the Gfx10 family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isGfx10 Set to true if input device name is a member of the Gfx10 family
    /// \return false if device ID is not found
    [[nodiscard]] bool IsGfx10Family(uint32_t deviceID, bool &isGfx10);

    /// Determine if the specified device is a member of the Gfx9 family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsGfx9 Set to true if input device name is a member of the Gfx9 family
    /// \return false if device name is not found
    [[nodiscard]] inline bool IsGfx9Family(const char *szCALDeviceName, bool &bIsGfx9)
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_GFX9, bIsGfx9);
    }

    /// Determine if the specified device is a member of the Gfx9 family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isGfx9 Set to true if input device name is a member of the Gfx9 family
    /// \return false if device ID is not found
    [[nodiscard]] bool IsGfx9Family(uint32_t deviceID, bool &isGfx9);

    /// Determine if the specified device is a member of the VI family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsVI Set to true if input device name is a member of the VI family
    /// \return false if device name is not found
    [[nodiscard]] inline bool IsVIFamily(const char *szCALDeviceName, bool &bIsVI)
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_VOLCANICISLAND, bIsVI);
    }

    /// Determine if the specified device is a member of the VI family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isVI Set to true if input device name is a member of the VI family
    /// \return false if device ID is not found
    [[nodiscard]] bool IsVIFamily(uint32_t deviceID, bool &isVI);

    /// Determine if the specified device is a member of the CI family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsCI Set to true if input device name is a member of the CI family
    /// \return false if device name is not found
    [[nodiscard]] inline bool IsCIFamily(const char *szCALDeviceName, bool &bIsCI)
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_SEAISLAND, bIsCI);
    }

    /// Determine if the specified device is a member of the CI family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isCI Set to true if input device name is a member of the CI family
    /// \return false if device ID is not found
    [[nodiscard]] bool IsCIFamily(uint32_t deviceID, bool &isCI);

    /// Determine if the specified device is a member of the SI family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsSI Set to true if input device name is a member of the SI family
    /// \return false if device name is not found
    [[nodiscard]] inline bool IsSIFamily(const char *szCALDeviceName, bool &bIsSI)
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_SOUTHERNISLAND, bIsSI);
    }

    /// Determine if the specified device is a member of the SI family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isSI Set to true if input device name is a member of the SI family
    /// \return false if device ID is not found
    [[nodiscard]] bool IsSIFamily(uint32_t deviceID, bool &isSI);

    /// Translates the reported device name to the true device name exposed in the DeviceInfo table.
    /// \param strDeviceName the device name reported by the runtime.
    /// \return the true device name as exposed by the device info table.
    [[nodiscard]] std::string TranslateDeviceName(const char *strDeviceName);

    /// Converts gfxIPVersion to GDT_HW_GENERATION
    /// \param gfxIPVer the graphics IP version whose corresponding GDT_HW_GENERATION is needed
    /// \param[out] hwGen the GDT_HW_GENERATION that corresponds to the specified graphics IP version
    /// \return true on success, false if there is no equivalent GDT_HW_GENERATION
    [[nodiscard]] bool GfxIPVerToHwGeneration(uint32_t gfxIPVer, GDT_HW_GENERATION &hwGen);

    /// Converts GDT_HW_GENERATION to gfxIPVersion
    /// \param hwGen the GDT_HW_GENERATION whose corresponding graphics IP version is needed
    /// \param[out] gfxIPVer the graphics IP version that corresponds to the specified GDT_HW_GENERATION
    /// \return true on success, false if there is no equivalent graphics IP version
    [[nodiscard]] bool HwGenerationToGfxIPVer(GDT_HW_GENERATION hwGen, uint32_t &gfxIPVer);
} // namespace AMDTDeviceInfoUtils

#endif
