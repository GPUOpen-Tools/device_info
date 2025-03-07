//==============================================================================
/// Copyright (c) 2010-2025 Advanced Micro Devices, Inc. All rights reserved.
/// @author AMD Developer Tools Team
/// @file
/// @brief Device info table.
//==============================================================================

#ifndef DEVICE_INFO_DEVICE_INFO_H_
#define DEVICE_INFO_DEVICE_INFO_H_

#ifndef _WIN32
#include <cstddef>  // Required by GCC size_t
#endif

#define REVISION_ID_ANY 0xFFFFFFFF  ///< Ignore revision id when looking up device Id.

enum GDT_HW_ASIC_TYPE
{
    GDT_ASIC_TYPE_NONE = -1,  ///< undefined asic
    GDT_TAHITI_PRO     = 0,   ///< TAHITI GPU PRO
    GDT_TAHITI_XT,            ///< TAHITI GPU XT
    GDT_PITCAIRN_PRO,         ///< PITCAIRN GPU PRO
    GDT_PITCAIRN_XT,          ///< PITCAIRN GPU XT
    GDT_CAPEVERDE_PRO,        ///< CAPE VERDE PRO GPU
    GDT_CAPEVERDE_XT,         ///< CAPE VERDE XT GPU
    GDT_OLAND,                ///< OLAND GPU (mobile is MARS)
    GDT_HAINAN,               ///< HAINAN GPU
    GDT_BONAIRE,              ///< BONAIRE GPU (mobile is SATURN)
    GDT_HAWAII,               ///< HAWAII GPU
    GDT_KALINDI,              ///< KB APU
    GDT_SPECTRE,              ///< KV APU SPECTRE
    GDT_SPECTRE_SL,           ///< KV APU SPECTRE SL
    GDT_SPECTRE_LITE,         ///< KV APU SPECTRE LITE
    GDT_SPOOKY,               ///< KV APU SPOOKY
    GDT_ICELAND,              ///< ICELAND GPU
    GDT_TONGA,                ///< TONGA GPU
    GDT_CARRIZO,              ///< CZ APU
    GDT_CARRIZO_EMB,          ///< CZ APU EMBEDDED
    GDT_FIJI,                 ///< FIJI GPU
    GDT_STONEY,               ///< STONEY APU
    GDT_ELLESMERE,            ///< ELLESMERE GPU
    GDT_BAFFIN,               ///< BAFFIN GPU
    GDT_GFX8_0_4,             ///< GFX8_0_4 GPU
    GDT_VEGAM1,               ///< VegaM GPU
    GDT_VEGAM2,               ///< VegaM GPU
    GDT_GFX9_0_0,             ///< GFX9_0_0 GPU
    GDT_GFX9_0_2,             ///< GFX9_0_2 APU
    GDT_GFX9_0_4,             ///< GFX9_0_4 GPU
    GDT_GFX9_0_6,             ///< GFX9_0_6 GPU
    GDT_GFX9_0_9,             ///< GFX9_0_9 GPU
    GDT_GFX9_0_A,             ///< GFX9_0_A GPU
    GDT_GFX9_0_C,             ///< GfX9_0_C GPU
    GDT_GFX9_4_2,             ///< GfX9_4_2 GPU
    GDT_GFX10_1_0,            ///< GFX10_1_0 GPU
    GDT_GFX10_1_0_XL,         ///< GFX10_1_0_XL GPU
    GDT_GFX10_1_2,            ///< GFX10_1_2 GPU
    GDT_GFX10_1_1,            ///< GFX10_1_1 GPU
    GDT_GFX10_3_0,            ///< GFX10_3_0 GPU
    GDT_GFX10_3_1,            ///< GFX10_3_1 GPU
    GDT_GFX10_3_2,            ///< GFX10_3_2 GPU
    GDT_GFX10_3_3,            ///< GFX10_3_3 APU
    GDT_GFX10_3_4,            ///< GFX10_3_4 GPU
    GDT_GFX10_3_5,            ///< GFX10_3_5 APU
    GDT_GFX11_0_0,            ///< GFX11_0_0 GPU
    GDT_GFX11_0_1,            ///< GFX11_0_1 GPU
    GDT_GFX11_0_2,            ///< GFX11_0_2 GPU
    GDT_GFX11_0_3,            ///< GFX11_0_3 APU
    GDT_GFX11_0_3B,           ///< GFX11_0_3B APU
    GDT_GFX11_5_0,            ///< GFX11_5_0 APU
    GDT_GFX11_5_1,            ///< GFX11_5_1 APU
    GDT_GFX11_5_2,            ///< GFX11_5_2 APU
    GDT_GFX12_0_1,            ///< GFX12_0_1 GPU
    GDT_LAST                  ///< last
};

/// Specifies the hardware vendor or generation.
enum GDT_HW_GENERATION
{
    GDT_HW_GENERATION_NONE,                                          ///< undefined hw generation
    GDT_HW_GENERATION_NVIDIA,                                        ///< nvidia GPU
    GDT_HW_GENERATION_INTEL,                                         ///< intel GPU
    GDT_HW_GENERATION_SOUTHERNISLAND,                                ///< GFX IP 6
    GDT_HW_GENERATION_FIRST_AMD = GDT_HW_GENERATION_SOUTHERNISLAND,  ///< first AMD generation
    GDT_HW_GENERATION_SEAISLAND,                                     ///< GFX IP 7
    GDT_HW_GENERATION_VOLCANICISLAND,                                ///< GFX IP 8
    GDT_HW_GENERATION_GFX9,                                          ///< GFX IP 9
    GDT_HW_GENERATION_GFX10,                                         ///< GFX IP 10
    GDT_HW_GENERATION_GFX103,                                        ///< GFX IP 10.3
    GDT_HW_GENERATION_GFX11,                                         ///< GFX IP 11
    GDT_HW_GENERATION_GFX115,                                        ///< GFX IP 11.5
    GDT_HW_GENERATION_CDNA,                                          ///< MI-100
    GDT_HW_GENERATION_CDNA2,                                         ///< MI-200
    GDT_HW_GENERATION_CDNA3,                                         ///< MI-300
    GDT_HW_GENERATION_GFX12,                                         ///< GFX IP 12
    GDT_HW_GENERATION_LAST
};

/// Card info.
struct GDT_GfxCardInfo
{
    GDT_HW_ASIC_TYPE  m_asicType;         ///< ASIC type, which is also the index to gs_deviceInfo table.
    size_t            m_deviceID;         ///< Numeric device id.
    size_t            m_revID;            ///< Numeric revision id.
    GDT_HW_GENERATION m_generation;       ///< Hardware generation.
    bool              m_bAPU;             ///< Flag indicating whether or not the device is an APU.
    const char*       m_szCALName;        ///< CAL name.
    const char*       m_szMarketingName;  ///< Marketing style device name.
};

/// Device info.
struct GDT_DeviceInfo
{
    size_t m_nNumShaderEngines;  ///< Number of shader engines.
    size_t m_nMaxWavePerSIMD;    ///< Number of wave slots per SIMD.
    size_t m_suClocksPrim;       ///< Number of clocks it takes to process a primitive.
    size_t m_nNumSQMaxCounters;  ///< Max number of SQ counters.
    size_t m_nNumPrimPipes;      ///< Number of primitive pipes.
    size_t m_nWaveSize;          ///< Wavefront size.
    size_t m_nNumSHPerSE;        ///< Number of shader array per Shader Engine.
    size_t m_nNumCUPerSH;        ///< Number of compute unit per Shader Array.
    size_t m_nNumSIMDPerCU;      ///< Number of SIMDs per Compute unit.

    bool m_deviceInfoValid;  ///< Flag indicating whether or not this device info is valid (as opposed to placeholder data).

    /// Get total number of SIMDs.
    size_t numberSIMDs() const
    {
        return m_nNumSIMDPerCU * numberCUs();
    }

    /// Get total number of shader arrays.
    size_t numberSHs() const
    {
        return m_nNumSHPerSE * m_nNumShaderEngines;
    }

    /// Get total number of compute units.
    size_t numberCUs() const
    {
        return numberSHs() * m_nNumCUPerSH;
    }
};

extern GDT_GfxCardInfo gs_cardInfo[];
extern GDT_DeviceInfo  gs_deviceInfo[];
extern size_t          gs_cardInfoSize;
extern size_t          gs_deviceInfoSize;

#endif  // DEVICE_INFO_DEVICE_INFO_H_
