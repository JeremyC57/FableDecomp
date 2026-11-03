#ifndef FABLE_EAX_NATIVE_TYPES_H
#define FABLE_EAX_NATIVE_TYPES_H
#include "EAX40.h"
#include "DSound3D.h"
// Original SDK COFF type and field names. Native omits older EAX1/2 conversion state.
typedef struct _EAX30LISTENERPROPERTIES
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
} EAX30LISTENERPROPERTIES, *LPEAX30LISTENERPROPERTIES;
typedef struct _EAX30BUFFERPROPERTIES
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
} EAX30BUFFERPROPERTIES, *LPEAX30BUFFERPROPERTIES;
enum IDENTIFIER {EAX3LISTENER=0,EAX3BUFFER=1,LISTENERUPDATE=4,BUFFERUPDATE=5};
// Only the four native-used identifiers are exposed; later SDK numbers differ.
enum EAXVERSION {EAX_NONE=0,EAX1=1,EAX2=2,EAX25=3,EAX3=4,EAX4=5};
struct LISTENER {
 D3DVECTOR vPosition,vVelocity,vOrientFront,vOrientTop;
 float flDistanceFactor,flRolloffFactor,flDopplerFactor;
};
struct LISTENERPROPERTIES {
 float vPosition[3],vVelocity[3],vOrientFront[3],vOrientTop[3];
 float flDistanceFactor,flRolloffFactor,flDopplerFactor;
};
struct BUFFERPROPERTIES {
 unsigned int BufferID;
 long lVolume;
 float vPosition[3],vVelocity[3];
 unsigned long ulInsideConeAngle,ulOutsideConeAngle;
 float vConeOrientation[3];
 long lConeOutsideVolume;
 float flMinDistance,flMaxDistance;
 unsigned long ulMode;
 EAX30BUFFERPROPERTIES eax30BP;
};
struct _CHANGES {
 IDENTIFIER id;
 int property;
 union { EAX30LISTENERPROPERTIES eax30LP; LISTENERPROPERTIES Listener; BUFFERPROPERTIES Buffer; };
 _CHANGES* next;
};
struct SOURCE {
 EAXFXSLOTPROPERTIES eax4FXSlot[4];
 EAXREVERBPROPERTIES eax4LPSlot[4];
 EAXCONTEXTPROPERTIES eax4Context;
 EAX30LISTENERPROPERTIES eax3LP;
};
struct DESTINATION { EAX30LISTENERPROPERTIES eax3LP; float flDistanceFactor; };
struct SOURCEBP {
 EAXSOURCEPROPERTIES eax4BP;
 EAXSOURCESENDPROPERTIES eax4BPSend[4];
 EAXSOURCEOCCLUSIONSENDPROPERTIES eax4BPOcclusionSend[4];
 EAXSOURCEEXCLUSIONSENDPROPERTIES eax4BPExclusionSend[4];
 GUID eax4BPActiveFXSlotID[2];
 EAX30BUFFERPROPERTIES eax3BP;
};
struct DESTINATIONBP { EAX30BUFFERPROPERTIES eax3BP; };
struct BUFFER {
 unsigned int BufferID;
 EAX30BUFFERPROPERTIES eax3BP;
 long volume;
 D3DVECTOR vPosition,vVelocity;
 unsigned long dwInsideConeAngle,dwOutsideConeAngle;
 D3DVECTOR vConeOrientation;
 long lConeOutsideVolume;
 float flMinDistance,flMaxDistance;
 unsigned long dwMode;
};
#endif
