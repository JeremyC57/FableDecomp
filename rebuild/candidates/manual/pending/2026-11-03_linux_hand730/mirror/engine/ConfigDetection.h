#ifndef FABLE_ENGINE_CONFIGDETECTION_H
#define FABLE_ENGINE_CONFIGDETECTION_H
// Complete original data members, access and array element types from Ego_r TPI.
// Native DisplayErrorDialog uses VendorName+196,DeviceName+200,DeviceID+204.
struct _GUID { unsigned long Data1;unsigned short Data2,Data3;unsigned char Data4[8]; };
struct SOUND_DEVICE {
public:
    char name[64];
    _GUID Guid;
    unsigned long DriverVersionLowPart;
    unsigned long DriverVersionHighPart;
    unsigned long VendorID;
    unsigned long DeviceID;
    unsigned long SubSysID;
    unsigned long Revision;
};
struct CONFIGTABLE {
public:
    const char *Property[50];
};
class ConfigDetection {
public:
    unsigned long cf_ForceShader;
    unsigned long cf_MaximumResolution;
    unsigned long cf_LinearTextureAddressing;
    unsigned long cf_LinearTextureAddressingZoom;
    unsigned long cf_LinearTextureAddressingSun;
    unsigned long cf_UseFixedFunction;
    unsigned long cf_DisableDriverManagement;
    unsigned long cf_DisableSpecular;
    unsigned long cf_PrototypeCard;
    unsigned long cf_UnsupportedVideoCard;
    unsigned long cf_UnsupportedAudioCard;
    unsigned long cf_DisableRenderTargets;
    unsigned long cf_DisableAlphaRenderTargets;
    unsigned long cf_UseAlternateConvolveMask;
    unsigned long cf_MinMaxBlendOpIsBroken;
    unsigned long cf_OldVideoDriver;
    unsigned long cf_EnableStopStart;
    unsigned long cf_HeadRelativeSpeech;
    unsigned long cf_OldSoundDriver;
    unsigned long cf_DisableBuffering;
    unsigned long cf_InvalidDriver;
    unsigned long cf_InvalidSoundDriver;
    unsigned long cf_SafeMode;
    unsigned long cf_UseAnisotropicFilter;
    float cf_DecalZBiasValue;
    float cf_DecalSlopeZBiasValue;
    float cf_TransparentDecalZBiasValue;
    float cf_TransparentDecalSlopeZBiasValue;
    unsigned long cf_UseAlternateRenderTargetClear;
    unsigned long cf_UseAlternateOutlineEffect;
    unsigned long cf_DisableStretchRect;
    unsigned long cf_UseAlternateSkyPixelShader;
    unsigned long cf_DisableDisplacementEffect;
    unsigned long cf_DisableReflectiveWater;
    unsigned long cf_DisableFilteredScreenGUIBackground;
    unsigned long cf_DisableTextureTransforms;
    unsigned long cf_MaxEffectsDetail;
    unsigned long cf_MaxShadowDetail;
    unsigned long cf_MaxMeshDetail;
    unsigned long cf_MaxTextureDetail;
    unsigned long cf_DefaultEffectsDetail;
    unsigned long cf_DefaultShadowDetail;
    unsigned long cf_DefaultMeshDetail;
    unsigned long cf_DefaultTextureDetail;
    unsigned long cf_MaxAntiAliasLevel;
    unsigned long cf_HardStop;
    unsigned long cf_GenericFlag0;
    unsigned long cf_GenericFlag1;
    unsigned long cf_GenericFlag2;
    char * VendorName;
    char * DeviceName;
    unsigned long DeviceID;
    unsigned long VendorID;
    unsigned long DriverVersionLowPart;
    unsigned long DriverVersionHighPart;
    unsigned long m_SysMemory;
    unsigned long m_CpuSpeed;
    unsigned long m_VideoMemory;
    char * AppendText;
    int optionNoVideo;
    int optionSafe;
    SOUND_DEVICE Sound_devices[10];
    unsigned long Sound_currentDevice;
    unsigned long Sound_maxDevices;
    CONFIGTABLE ConfigTable[50];
    unsigned long req_CPUSpeed;
    unsigned long req_Memory;
    unsigned long req_VideoMemory;
    unsigned long req_DirectXVersion;
    unsigned long req_DirectXRevision;
    unsigned long req_DiskSpace;
private:
    bool m_ErrorOccurred;
    char * m_ErrorString;
};
#endif
