#ifndef FABLE_EAX40_TYPES_H
#define FABLE_EAX40_TYPES_H
// Creative EAX 4.0 API, revision 1.6.3, 21 February 2003.
// Original public SDK tags and members; all fields naturally align to four bytes.
#define EAX_MAX_ACTIVE_FXSLOTS 2
extern "C" {
typedef struct _GUID
        {
            unsigned long Data1;
            unsigned short Data2;
            unsigned short Data3;
            unsigned char Data4[8];
        } GUID;

typedef struct _EAXCONTEXTPROPERTIES
{
    GUID          guidPrimaryFXSlotID;
    float         flDistanceFactor;
    float         flAirAbsorptionHF;
    float         flHFReference;
} EAXCONTEXTPROPERTIES, *LPEAXCONTEXTPROPERTIES;

typedef struct _EAXSOURCEPROPERTIES
{
    long          lDirect;                 // direct path level (at low and mid frequencies)
    long          lDirectHF;               // relative direct path level at high frequencies
    long          lRoom;                   // room effect level (at low and mid frequencies)
    long          lRoomHF;                 // relative room effect level at high frequencies
    long          lObstruction;            // main obstruction control (attenuation at high frequencies)
    float         flObstructionLFRatio;    // obstruction low-frequency level re. main control
    long          lOcclusion;              // main occlusion control (attenuation at high frequencies)
    float         flOcclusionLFRatio;      // occlusion low-frequency level re. main control
    float         flOcclusionRoomRatio;    // relative occlusion control for room effect
    float         flOcclusionDirectRatio;  // relative occlusion control for direct path
    long          lExclusion;              // main exlusion control (attenuation at high frequencies)
    float         flExclusionLFRatio;      // exclusion low-frequency level re. main control
    long          lOutsideVolumeHF;        // outside sound cone level at high frequencies
    float         flDopplerFactor;         // like DS3D flDopplerFactor but per source
    float         flRolloffFactor;         // like DS3D flRolloffFactor but per source
    float         flRoomRolloffFactor;     // like DS3D flRolloffFactor but for room effect
    float         flAirAbsorptionFactor;   // multiplies EAXREVERB_AIRABSORPTIONHF
    unsigned long ulFlags;                 // modifies the behavior of properties
} EAXSOURCEPROPERTIES, *LPEAXSOURCEPROPERTIES;

typedef struct _EAXSOURCEALLSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	long          lSend;                   // send level (at low and mid frequencies)
	long          lSendHF;                 // relative send level at high frequencies
	long          lOcclusion;
	float         flOcclusionLFRatio;
	float         flOcclusionRoomRatio;
	float         flOcclusionDirectRatio;
	long          lExclusion;
	float         flExclusionLFRatio;
} EAXSOURCEALLSENDPROPERTIES, *LPEAXSOURCEALLSENDPROPERTIES;

typedef struct _EAXACTIVEFXSLOTS
{
    GUID          guidActiveFXSlots[EAX_MAX_ACTIVE_FXSLOTS];
} EAXACTIVEFXSLOTS, *LPEAXACTIVEFXSLOTS;

typedef struct _EAXOBSTRUCTIONPROPERTIES
{
    long          lObstruction;
    float         flObstructionLFRatio;
} EAXOBSTRUCTIONPROPERTIES, *LPEAXOBSTRUCTIONPROPERTIES;

typedef struct _EAXOCCLUSIONPROPERTIES
{
    long          lOcclusion;
    float         flOcclusionLFRatio;
    float         flOcclusionRoomRatio;
    float         flOcclusionDirectRatio;
} EAXOCCLUSIONPROPERTIES, *LPEAXOCCLUSIONPROPERTIES;

typedef struct _EAXEXCLUSIONPROPERTIES
{
    long          lExclusion;
    float         flExclusionLFRatio;
} EAXEXCLUSIONPROPERTIES, *LPEAXEXCLUSIONPROPERTIES;

typedef struct _EAXSOURCESENDPROPERTIES
{
    GUID          guidReceivingFXSlotID;
    long          lSend;
    long          lSendHF;
} EAXSOURCESENDPROPERTIES, *LPEAXSOURCESENDPROPERTIES;

typedef struct _EAXSOURCEOCCLUSIONSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	long          lOcclusion;
	float         flOcclusionLFRatio;
	float         flOcclusionRoomRatio;
	float         flOcclusionDirectRatio;
} EAXSOURCEOCCLUSIONSENDPROPERTIES, *LPEAXSOURCEOCCLUSIONSENDPROPERTIES;

typedef struct _EAXSOURCEEXCLUSIONSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	long          lExclusion;
	float         flExclusionLFRatio;
} EAXSOURCEEXCLUSIONSENDPROPERTIES, *LPEAXSOURCEEXCLUSIONSENDPROPERTIES;

typedef struct _EAXFXSLOTPROPERTIES
{
	GUID          guidLoadEffect;
	long          lVolume;
	long          lLock;
	unsigned long ulFlags;
} EAXFXSLOTPROPERTIES, *LPEAXFXSLOTPROPERTIES;

typedef struct _EAXVECTOR {
    float x;
    float y;
    float z;
} EAXVECTOR;

typedef struct _EAXREVERBPROPERTIES
{
    unsigned long ulEnvironment;   // sets all reverb properties
    float flEnvironmentSize;       // environment size in meters
    float flEnvironmentDiffusion;  // environment diffusion
    long lRoom;                    // room effect level (at mid frequencies)
    long lRoomHF;                  // relative room effect level at high frequencies
    long lRoomLF;                  // relative room effect level at low frequencies
    float flDecayTime;             // reverberation decay time at mid frequencies
    float flDecayHFRatio;          // high-frequency to mid-frequency decay time ratio
    float flDecayLFRatio;          // low-frequency to mid-frequency decay time ratio
    long lReflections;             // early reflections level relative to room effect
    float flReflectionsDelay;      // initial reflection delay time
    EAXVECTOR vReflectionsPan;     // early reflections panning vector
    long lReverb;                  // late reverberation level relative to room effect
    float flReverbDelay;           // late reverberation delay time relative to initial reflection
    EAXVECTOR vReverbPan;          // late reverberation panning vector
    float flEchoTime;              // echo time
    float flEchoDepth;             // echo depth
    float flModulationTime;        // modulation time
    float flModulationDepth;       // modulation depth
    float flAirAbsorptionHF;       // change in level per meter at high frequencies
    float flHFReference;           // reference high frequency
    float flLFReference;           // reference low frequency
    float flRoomRolloffFactor;     // like DS3D flRolloffFactor but for room effect
    unsigned long ulFlags;         // modifies the behavior of properties
} EAXREVERBPROPERTIES, *LPEAXREVERBPROPERTIES;

typedef enum
{
    EAXSOURCE_NONE,
    EAXSOURCE_ALLPARAMETERS,
    EAXSOURCE_OBSTRUCTIONPARAMETERS,
    EAXSOURCE_OCCLUSIONPARAMETERS,
    EAXSOURCE_EXCLUSIONPARAMETERS,
    EAXSOURCE_DIRECT,
    EAXSOURCE_DIRECTHF,
    EAXSOURCE_ROOM,
    EAXSOURCE_ROOMHF,
    EAXSOURCE_OBSTRUCTION,
    EAXSOURCE_OBSTRUCTIONLFRATIO,
    EAXSOURCE_OCCLUSION,
    EAXSOURCE_OCCLUSIONLFRATIO,
    EAXSOURCE_OCCLUSIONROOMRATIO,
    EAXSOURCE_OCCLUSIONDIRECTRATIO,
    EAXSOURCE_EXCLUSION,
    EAXSOURCE_EXCLUSIONLFRATIO,
    EAXSOURCE_OUTSIDEVOLUMEHF,
    EAXSOURCE_DOPPLERFACTOR,
    EAXSOURCE_ROLLOFFFACTOR,
    EAXSOURCE_ROOMROLLOFFFACTOR,
    EAXSOURCE_AIRABSORPTIONFACTOR,
    EAXSOURCE_FLAGS,
    EAXSOURCE_SENDPARAMETERS,
    EAXSOURCE_ALLSENDPARAMETERS,
    EAXSOURCE_OCCLUSIONSENDPARAMETERS,
    EAXSOURCE_EXCLUSIONSENDPARAMETERS,
    EAXSOURCE_ACTIVEFXSLOTID,
} EAXSOURCE_PROPERTY;

typedef enum
{
    EAXFXSLOT_PARAMETER = 0, // range 0-0x40 reserved for loaded effect parameters
    EAXFXSLOT_NONE = 0x10000,
	EAXFXSLOT_ALLPARAMETERS,
    EAXFXSLOT_LOADEFFECT,
    EAXFXSLOT_VOLUME,
	EAXFXSLOT_LOCK,
	EAXFXSLOT_FLAGS
} EAXFXSLOT_PROPERTY;

typedef enum
{
    EAXREVERB_NONE,
    EAXREVERB_ALLPARAMETERS,
    EAXREVERB_ENVIRONMENT,
    EAXREVERB_ENVIRONMENTSIZE,
    EAXREVERB_ENVIRONMENTDIFFUSION,
    EAXREVERB_ROOM,
    EAXREVERB_ROOMHF,
    EAXREVERB_ROOMLF,
    EAXREVERB_DECAYTIME,
    EAXREVERB_DECAYHFRATIO,
    EAXREVERB_DECAYLFRATIO,
    EAXREVERB_REFLECTIONS,
    EAXREVERB_REFLECTIONSDELAY,
    EAXREVERB_REFLECTIONSPAN,
    EAXREVERB_REVERB,
    EAXREVERB_REVERBDELAY,
    EAXREVERB_REVERBPAN,
    EAXREVERB_ECHOTIME,
    EAXREVERB_ECHODEPTH,
    EAXREVERB_MODULATIONTIME,
    EAXREVERB_MODULATIONDEPTH,
    EAXREVERB_AIRABSORPTIONHF,
    EAXREVERB_HFREFERENCE,
    EAXREVERB_LFREFERENCE,
    EAXREVERB_ROOMROLLOFFFACTOR,
    EAXREVERB_FLAGS,
} EAXREVERB_PROPERTY;
extern const GUID EAX_REVERB_EFFECT;
extern const GUID EAX_PrimaryFXSlotID;
}
#endif
