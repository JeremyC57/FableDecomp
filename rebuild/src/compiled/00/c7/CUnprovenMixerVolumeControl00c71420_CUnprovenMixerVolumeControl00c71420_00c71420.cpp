// CUnprovenMixerVolumeControl_00c71420::CUnprovenMixerVolumeControl_00c71420() -- Initialize the observed default state.
// Leaf: ??0CUnprovenMixerVolumeControl_00c71420@@QAE@XZ
// Identity: descriptive. Actual caller00c55d00 initializes contained object+110. Helper00c71430 uses mixerOpen out at0, MIXERCONTROLA output at+4 with SDK cbmxctrl148, saved unsigned volume+152 from mixerGetControlDetailsA; close00c715d0 tests Control.szName[0] at+40 before restoring volume. This is not navigation. Original owner/field names descriptive, SDK types/layout proven.
// Layout: own exact writes; no vptr or base stores observed. SDK scalar widths portable in host tests; unused union alternatives omitted while preserving original reserved arrays and extents. No engine owner header exists for this unproven identity.
#ifdef _MSC_VER
typedef unsigned long DWORD;
#else
typedef unsigned int DWORD;
#endif
struct HMIXER__;typedef HMIXER__* HMIXER;
struct tagMIXERCONTROLA{DWORD cbStruct,dwControlID,dwControlType,fdwControl,cMultipleItems;char szShortName[16];char szName[64];union{DWORD dwReserved[6];}Bounds;union{DWORD dwReserved[6];}Metrics;};typedef tagMIXERCONTROLA MIXERCONTROLA;
struct tMIXERCONTROLDETAILS_UNSIGNED{DWORD dwValue;};typedef tMIXERCONTROLDETAILS_UNSIGNED MIXERCONTROLDETAILS_UNSIGNED;
class CUnprovenMixerVolumeControl_00c71420{public:HMIXER Handle;MIXERCONTROLA Control;MIXERCONTROLDETAILS_UNSIGNED SavedVolume;CUnprovenMixerVolumeControl_00c71420();};CUnprovenMixerVolumeControl_00c71420::CUnprovenMixerVolumeControl_00c71420(){Handle=0;SavedVolume.dwValue=0;Control.szName[0]=0;}
#ifdef _MSC_VER
typedef char ExactOwnerSize[sizeof(CUnprovenMixerVolumeControl_00c71420)==156?1:-1];
#endif