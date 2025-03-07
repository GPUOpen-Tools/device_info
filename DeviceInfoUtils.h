//==============================================================================
// Copyright (c) 2010-2025 Advanced Micro Devices, Inc. All rights reserved.
/// @author AMD Developer Tools Team
/// @file
/// @brief Device info utils class.
//==============================================================================

#ifndef DEVICE_INFO_DEVICE_INFO_UTILS_H_
#define DEVICE_INFO_DEVICE_INFO_UTILS_H_

#include <string>
#include <cstring>
#include <map>
#include <vector>
#include "DeviceInfo.h"

//------------------------------------------------------------------------------------
/// Device info utils
//------------------------------------------------------------------------------------
class AMDTDeviceInfoUtils
{
public:

    /// Get AMDTDeviceInfoUtils instance
    /// \return the dynamically created AMDTDeviceInfoUtils instance
    static AMDTDeviceInfoUtils* Instance();

    /// Deletes the AMDTDeviceInfoUtils instance
    static void DeleteInstance();

    /// Adds the specified device info
    /// \param asicType the asic type of the device info being added
    /// \param deviceInfo the device info being added
    void AddDeviceInfo(GDT_HW_ASIC_TYPE asicType, const GDT_DeviceInfo& deviceInfo);

    /// Adds the specified card info
    /// \param cardInfo the card being added
    void AddDevice(const GDT_GfxCardInfo& cardInfo);

    /// Removes the specified card info
    /// \param cardInfo the card being removed
    void RemoveDevice(const GDT_GfxCardInfo& cardInfo);

    /// Function pointer type for a function that will translate device names
    typedef std::string(*DeviceNameTranslatorFunction)(const char* strDeviceName);

    /// Sets the Device name translator function
    /// \param deviceNametranslatorFunction the function to use to translate device names
    void SetDeviceNameTranslator(DeviceNameTranslatorFunction deviceNametranslatorFunction);

    /// Get device info from device ID
    /// \param[in] deviceID Device ID - usually queried from ADL
    /// \param[in] revisionID RevisionID - usually queried from ADL
    /// \param[out] deviceInfo Output device info if device id is found.
    /// \return True if device info is found
    bool GetDeviceInfo(size_t deviceID, size_t revisionID, GDT_DeviceInfo& deviceInfo) const;

    /// Get device info from CAL name string
    /// NOTE: this might not return the correct GDT_DeviceInfo instance, since some devices with the same CAL name might have different GDT_DeviceInfo instances
    /// \param[in] szCALDeviceName CAL device name string
    /// \param[out] deviceInfo Output device info if device id is found.
    /// \return True if device info is found
    bool GetDeviceInfo(const char* szCALDeviceName, GDT_DeviceInfo& deviceInfo) const;

    /// Get Graphics Card Info.
    /// \param[in] deviceID Device ID - usually queried from ADL
    /// \param[in] revisionID Revision ID - usually queried from ADL
    /// \param[out] cardInfo Output graphics card info if device id is found.
    /// \return True if info for deviceID is found
    bool GetDeviceInfo(size_t deviceID, size_t revisionID, GDT_GfxCardInfo& cardInfo) const;

    /// Get a vector of Graphics Card Info.
    /// \param[in] szCALDeviceName CAL device name string
    /// \param[out] cardList Output vector of graphics card info.
    /// \return True if any graphics card info is found for CAL device name.
    bool GetDeviceInfo(const char* szCALDeviceName, std::vector<GDT_GfxCardInfo>& cardList) const;

    /// Get a vector of Graphics Card Info.
    /// \param[in]  szMarketingDeviceName Marketing device name string
    /// \param[out] cardList Output vector of graphics card info.
    /// \return True if any graphics card info is found for Marketing device name.
    bool GetDeviceInfoMarketingName(const char* szMarketingDeviceName, std::vector<GDT_GfxCardInfo>& cardList) const;

    /// Query whether or not input device is APU or not
    /// \param[in] szCALDeviceName CAL device name string
    /// \param[out] bIsAPU flag indicating whether or not the specified device is an APU
    /// \return True if device info is found
    bool IsAPU(const char* szCALDeviceName, bool& bIsAPU) const;

    /// Query whether or not input device is APU or not
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isAPU flag indicating whether or not the specified device is an APU
    /// \return True if device info is found
    bool IsAPU(size_t deviceID, bool& isAPU) const;

    /// Get hardware generation from device ID
    /// \param[in] deviceID Device id
    /// \param[out] gen Hardware generation
    /// \return True if device info is found
    bool GetHardwareGeneration(size_t deviceID, GDT_HW_GENERATION& gen) const;

    /// Get hardware generation from device name
    /// \param[in] szCALDeviceName Device name
    /// \param[out] gen Hardware generation
    /// \return True if device info is found
    bool GetHardwareGeneration(const char* szCALDeviceName, GDT_HW_GENERATION& gen) const;

    /// Get all cards in all hardware generations.
    /// \param[out] cardList Output vector of all graphics card info.
    void GetAllCards(std::vector<GDT_GfxCardInfo>& cardList) const;

    /// Get all cards with the specified CAL device name -- this a wrapper around one of the GetDeviceInfo overloads
    /// \param[in] szCALDeviceName CAL device name string
    /// \param[out] cardList Output vector of graphics card info.
    /// \return True if any graphics card info is found for CAL device name.
    bool GetAllCardsWithName(const char* szCALDeviceName, std::vector<GDT_GfxCardInfo>& cardList) const;

    /// Get all cards from the specified hardware generation
    /// \param[in] gen Hardware generation
    /// \param[out] cardList Output vector of graphics card info.
    /// \return true if successful, false otherwise
    bool GetAllCardsInHardwareGeneration(GDT_HW_GENERATION gen, std::vector<GDT_GfxCardInfo>& cardList) const;

    /// Get all cards with the specified device id
    /// \param[in] deviceID DeviceId
    /// \param[out] cardList Output vector of graphics card info.
    /// \return true if successful, false otherwise
    bool GetAllCardsWithDeviceId(size_t deviceID, std::vector<GDT_GfxCardInfo>& cardList) const;

    /// Get all cards with the specified ASIC type
    /// \param[in] asicType ASICType
    /// \param[out] cardList Output vector of graphics card info.
    /// \return true if successful, false otherwise
    bool GetAllCardsWithAsicType(GDT_HW_ASIC_TYPE asicType, std::vector<GDT_GfxCardInfo>& cardList) const;

    /// Get hardware generation display name
    /// \param[in] gen Hardware generation
    /// \param[out] strGenerationDisplayName the display name for the specified hardware generation
    /// \return true if successful, false otherwise
    bool GetHardwareGenerationDisplayName(GDT_HW_GENERATION gen, std::string& strGenerationDisplayName) const;

    /// Determine if the specified device is a member of the specified family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[in] generation Generation enum
    /// \param[out] bRes Set to true if input device name is a specified family card
    /// \return false if device name is not found
    bool IsXFamily(const char* szCALDeviceName, GDT_HW_GENERATION generation, bool& bRes) const
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
    bool IsXFamily(size_t deviceID, GDT_HW_GENERATION generation, bool& isXFamily) const;

    /// Determine if the specified device is a member of the Gfx12 family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsGfx12 Set to true if input device name is a member of the Gfx12 family
    /// \return false if device name is not found
    bool IsGfx12Family(const char* szCALDeviceName, bool& bIsGfx12) const
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_GFX12, bIsGfx12);
    }

    /// Determine if the specified device is a member of the Gfx12 family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isGfx12 Set to true if input device name is a member of the Gfx12 family
    /// \return false if device ID is not found
    bool IsGfx12Family(size_t deviceID, bool& isGfx12) const;
	
	/// Determine if the specified device is a member of the Gfx115 family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsGfx115 Set to true if input device name is a member of the Gfx115 family
    /// \return false if device name is not found
    bool IsGfx115Family(const char* szCALDeviceName, bool& bIsGfx115) const
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_GFX115, bIsGfx115);
    }

    /// Determine if the specified device is a member of the Gfx115 family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] bIsGfx115 Set to true if input device name is a member of the Gfx115 family
    /// \return false if device ID is not found
    bool IsGfx115Family(size_t deviceID, bool& isGfx12) const;

    /// Determine if the specified device is a member of the Gfx11 family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsGfx11 Set to true if input device name is a member of the Gfx11 family
    /// \return false if device name is not found
    bool IsGfx11Family(const char* szCALDeviceName, bool& bIsGfx11) const
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_GFX11, bIsGfx11);
    }

    /// Determine if the specified device is a member of the Gfx11 family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isGfx11 Set to true if input device name is a member of the Gfx11 family
    /// \return false if device ID is not found
    bool IsGfx11Family(size_t deviceID, bool& isGfx11) const;

    /// Determine if the specified device is a member of the Gfx10 family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsGfx10 Set to true if input device name is a member of the Gfx10 family
    /// \return false if device name is not found
    bool IsGfx10Family(const char* szCALDeviceName, bool& bIsGfx10) const
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_GFX10, bIsGfx10);
    }

    /// Determine if the specified device is a member of the Gfx10 family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isGfx10 Set to true if input device name is a member of the Gfx10 family
    /// \return false if device ID is not found
    bool IsGfx10Family(size_t deviceID, bool& isGfx10) const;

    /// Determine if the specified device is a member of the Gfx9 family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsGfx9 Set to true if input device name is a member of the Gfx9 family
    /// \return false if device name is not found
    bool IsGfx9Family(const char* szCALDeviceName, bool& bIsGfx9) const
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_GFX9, bIsGfx9);
    }

    /// Determine if the specified device is a member of the Gfx9 family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isGfx9 Set to true if input device name is a member of the Gfx9 family
    /// \return false if device ID is not found
    bool IsGfx9Family(size_t deviceID, bool& isGfx9) const;

    /// Determine if the specified device is a member of the VI family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsVI Set to true if input device name is a member of the VI family
    /// \return false if device name is not found
    bool IsVIFamily(const char* szCALDeviceName, bool& bIsVI) const
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_VOLCANICISLAND, bIsVI);
    }

    /// Determine if the specified device is a member of the VI family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isVI Set to true if input device name is a member of the VI family
    /// \return false if device ID is not found
    bool IsVIFamily(size_t deviceID, bool& isVI) const;

    /// Determine if the specified device is a member of the CI family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsCI Set to true if input device name is a member of the CI family
    /// \return false if device name is not found
    bool IsCIFamily(const char* szCALDeviceName, bool& bIsCI) const
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_SEAISLAND, bIsCI);
    }

    /// Determine if the specified device is a member of the CI family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isCI Set to true if input device name is a member of the CI family
    /// \return false if device ID is not found
    bool IsCIFamily(size_t deviceID, bool& isCI) const;

    /// Determine if the specified device is a member of the SI family
    /// \param[in] szCALDeviceName CAL device name
    /// \param[out] bIsSI Set to true if input device name is a member of the SI family
    /// \return false if device name is not found
    bool IsSIFamily(const char* szCALDeviceName, bool& bIsSI) const
    {
        return IsXFamily(szCALDeviceName, GDT_HW_GENERATION_SOUTHERNISLAND, bIsSI);
    }

    /// Determine if the specified device is a member of the SI family
    /// \param[in] deviceID the PCIE device ID
    /// \param[out] isSI Set to true if input device name is a member of the SI family
    /// \return false if device ID is not found
    bool IsSIFamily(size_t deviceID, bool& isSI) const;

    /// Translates the reported device name to the true device name exposed in the DeviceInfo table.
    /// \param strDeviceName the device name reported by the runtime.
    /// \return the true device name as exposed by the device info table.
    std::string TranslateDeviceName(const char* strDeviceName) const;

    /// Converts gfxIPVersion to GDT_HW_GENERATION
    /// \param gfxIPVer the graphics IP version whose corresponding GDT_HW_GENERATION is needed
    /// \param[out] hwGen the GDT_HW_GENERATION that corresponds to the specified graphics IP version
    /// \return true on success, false if there is no equivalent GDT_HW_GENERATION
    bool GfxIPVerToHwGeneration(size_t gfxIPVer, GDT_HW_GENERATION& hwGen) const;

    /// Converts GDT_HW_GENERATION to gfxIPVersion
    /// \param hwGen the GDT_HW_GENERATION whose corresponding graphics IP version is needed
    /// \param[out] gfxIPVer the graphics IP version that corresponds to the specified GDT_HW_GENERATION
    /// \return true on success, false if there is no equivalent graphics IP version
    bool HwGenerationToGfxIPVer(GDT_HW_GENERATION hwGen, size_t& gfxIPVer) const;

private:
    /// private constructor
    AMDTDeviceInfoUtils() : m_pDeviceNameTranslatorFunction(nullptr) {}

    /// disable copy constructor
    AMDTDeviceInfoUtils(const AMDTDeviceInfoUtils&) = delete;

    /// disable move constructor
    AMDTDeviceInfoUtils(AMDTDeviceInfoUtils&&) = delete;

    /// disable assignment operator
    /// \return reference to object
    AMDTDeviceInfoUtils& operator=(AMDTDeviceInfoUtils&) = delete;

    /// disable move operator
    /// \return reference to object
    AMDTDeviceInfoUtils& operator=(AMDTDeviceInfoUtils&&) = delete;

    /// private destructor
    virtual ~AMDTDeviceInfoUtils() = default;

    //------------------------------------------------------------------------------------
    /// const char* comparer used in the DeviceNameMap below
    //------------------------------------------------------------------------------------
    struct cmp_str
    {
        /// Operator () overload function
        /// \param a left operand
        /// \param b right operand
        /// \return true if a is less than b
        bool operator()(const char* a, const char* b) const
        {
            return std::strcmp(a, b) < 0;
        }
    };

    typedef std::multimap<size_t, GDT_GfxCardInfo> DeviceIDMap;                        ///< typedef for map from device id to card info
    typedef std::pair<size_t, GDT_GfxCardInfo> DeviceIDMapPair;                        ///< typedef for device id / card info pair

    typedef std::multimap<GDT_HW_ASIC_TYPE, GDT_GfxCardInfo> ASICTypeCardInfoMap;      ///< typedef for map from asic type to card info
    typedef std::pair<GDT_HW_ASIC_TYPE, GDT_GfxCardInfo> ASICTypeCardInfoMapPair;      ///< typedef for asic type / card info pair

    typedef std::multimap<const char*, GDT_GfxCardInfo, cmp_str> DeviceNameMap;        ///< typedef for map from CAL device name to card info (with custom comparer)
    typedef std::pair<const char*, GDT_GfxCardInfo> DeviceNameMapPair;                 ///< typedef for device name / card info pair

    typedef std::multimap<GDT_HW_GENERATION, GDT_GfxCardInfo> DeviceHWGenerationMap;   ///< typedef for map from hardware generation to card info
    typedef std::pair<GDT_HW_GENERATION, GDT_GfxCardInfo> DeviceHWGenerationMapPair;   ///< typedef for hardware generation / card info pair

    typedef std::map<GDT_HW_ASIC_TYPE, GDT_DeviceInfo> ASICTypeDeviceInfoMap;          ///< typedef for map from asic type to device info
    typedef std::pair<GDT_HW_ASIC_TYPE, GDT_DeviceInfo> ASICTypeDeviceInfoMapPair;     ///< typedef for asic type / device info pair

    DeviceIDMap           m_deviceIDMap;                                               ///< device ID to card info map.
    ASICTypeCardInfoMap   m_asicTypeCardInfoMap;                                       ///< ASIC type to card info map.
    DeviceNameMap         m_deviceNameMap;                                             ///< cal device name to card info map.
    DeviceNameMap         m_deviceMarketingNameMap;                                    ///< marketing device name to card info map.
    DeviceHWGenerationMap m_deviceHwGenerationMap;                                     ///< hardware generation to card info map.
    ASICTypeDeviceInfoMap m_asicTypeDeviceInfoMap;                                     ///< ASIC type to device info map.

    DeviceNameTranslatorFunction m_pDeviceNameTranslatorFunction;                      ///< the function to call to translate device names

    static AMDTDeviceInfoUtils* ms_pInstance;                                          ///< singleton instance

    static const unsigned int ms_gfxToGdtHwGenConversionFactor = 3;                    /// factor to apply when converting between GFX IP version and GDT_HW_GENERATION
};

//------------------------------------------------------------------------------------
/// Device info manager
//------------------------------------------------------------------------------------
class AMDTDeviceInfoManager
{
    friend class AMDTDeviceInfoUtils;

    /// Get AMDTDeviceInfoManager instance
    /// \return the dynamically created AMDTDeviceInfoManager instance
    static AMDTDeviceInfoManager* Instance()
    {
        if (nullptr == ms_pInstance)
        {
            ms_pInstance = new AMDTDeviceInfoManager();
        }

        return ms_pInstance;
    }

    /// Deletes the AMDTDeviceInfoManager instance
    static void DeleteInstance()
    {
        delete ms_pInstance;
        ms_pInstance = nullptr;
    }

private:

    /// Constructor
    AMDTDeviceInfoManager();

    /// disable copy constructor
    AMDTDeviceInfoManager(const AMDTDeviceInfoManager&) = delete;

    /// disable move constructor
    AMDTDeviceInfoManager(AMDTDeviceInfoManager&&) = delete;

    /// disable assignment operator
    /// \return reference to object
    AMDTDeviceInfoManager& operator=(AMDTDeviceInfoManager&) = delete;

    /// disable move operator
    /// \return reference to object
    AMDTDeviceInfoManager& operator=(AMDTDeviceInfoManager&&) = delete;

    /// Destructor
    ~AMDTDeviceInfoManager() = default;

    static AMDTDeviceInfoManager* ms_pInstance; ///< the singleton AMDTDeviceInfoManager instance
};

#endif // DEVICE_INFO_DEVICE_INFO_UTILS_H_
