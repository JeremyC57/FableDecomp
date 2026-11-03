#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifndef _MSC_VER
#define __cdecl
#endif
// Fixture uses fixed-width int for native long, and public visibility for state checks.
#ifndef FABLE_EAX40_TYPES_H
#define FABLE_EAX40_TYPES_H
// Creative EAX 4.0 API, revision 1.6.3, 21 February 2003.
// Original public SDK tags and members; all fields naturally align to four bytes.
#define EAX_MAX_ACTIVE_FXSLOTS 2
extern "C" {
typedef struct _GUID
        {
            unsigned int Data1;
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
    int          lDirect;                 // direct path level (at low and mid frequencies)
    int          lDirectHF;               // relative direct path level at high frequencies
    int          lRoom;                   // room effect level (at low and mid frequencies)
    int          lRoomHF;                 // relative room effect level at high frequencies
    int          lObstruction;            // main obstruction control (attenuation at high frequencies)
    float         flObstructionLFRatio;    // obstruction low-frequency level re. main control
    int          lOcclusion;              // main occlusion control (attenuation at high frequencies)
    float         flOcclusionLFRatio;      // occlusion low-frequency level re. main control
    float         flOcclusionRoomRatio;    // relative occlusion control for room effect
    float         flOcclusionDirectRatio;  // relative occlusion control for direct path
    int          lExclusion;              // main exlusion control (attenuation at high frequencies)
    float         flExclusionLFRatio;      // exclusion low-frequency level re. main control
    int          lOutsideVolumeHF;        // outside sound cone level at high frequencies
    float         flDopplerFactor;         // like DS3D flDopplerFactor but per source
    float         flRolloffFactor;         // like DS3D flRolloffFactor but per source
    float         flRoomRolloffFactor;     // like DS3D flRolloffFactor but for room effect
    float         flAirAbsorptionFactor;   // multiplies EAXREVERB_AIRABSORPTIONHF
    unsigned int ulFlags;                 // modifies the behavior of properties
} EAXSOURCEPROPERTIES, *LPEAXSOURCEPROPERTIES;

typedef struct _EAXSOURCEALLSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	int          lSend;                   // send level (at low and mid frequencies)
	int          lSendHF;                 // relative send level at high frequencies
	int          lOcclusion;
	float         flOcclusionLFRatio;
	float         flOcclusionRoomRatio;
	float         flOcclusionDirectRatio;
	int          lExclusion;
	float         flExclusionLFRatio;
} EAXSOURCEALLSENDPROPERTIES, *LPEAXSOURCEALLSENDPROPERTIES;

typedef struct _EAXACTIVEFXSLOTS
{
    GUID          guidActiveFXSlots[EAX_MAX_ACTIVE_FXSLOTS];
} EAXACTIVEFXSLOTS, *LPEAXACTIVEFXSLOTS;

typedef struct _EAXOBSTRUCTIONPROPERTIES
{
    int          lObstruction;
    float         flObstructionLFRatio;
} EAXOBSTRUCTIONPROPERTIES, *LPEAXOBSTRUCTIONPROPERTIES;

typedef struct _EAXOCCLUSIONPROPERTIES
{
    int          lOcclusion;
    float         flOcclusionLFRatio;
    float         flOcclusionRoomRatio;
    float         flOcclusionDirectRatio;
} EAXOCCLUSIONPROPERTIES, *LPEAXOCCLUSIONPROPERTIES;

typedef struct _EAXEXCLUSIONPROPERTIES
{
    int          lExclusion;
    float         flExclusionLFRatio;
} EAXEXCLUSIONPROPERTIES, *LPEAXEXCLUSIONPROPERTIES;

typedef struct _EAXSOURCESENDPROPERTIES
{
    GUID          guidReceivingFXSlotID;
    int          lSend;
    int          lSendHF;
} EAXSOURCESENDPROPERTIES, *LPEAXSOURCESENDPROPERTIES;

typedef struct _EAXSOURCEOCCLUSIONSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	int          lOcclusion;
	float         flOcclusionLFRatio;
	float         flOcclusionRoomRatio;
	float         flOcclusionDirectRatio;
} EAXSOURCEOCCLUSIONSENDPROPERTIES, *LPEAXSOURCEOCCLUSIONSENDPROPERTIES;

typedef struct _EAXSOURCEEXCLUSIONSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	int          lExclusion;
	float         flExclusionLFRatio;
} EAXSOURCEEXCLUSIONSENDPROPERTIES, *LPEAXSOURCEEXCLUSIONSENDPROPERTIES;

typedef struct _EAXFXSLOTPROPERTIES
{
	GUID          guidLoadEffect;
	int          lVolume;
	int          lLock;
	unsigned int ulFlags;
} EAXFXSLOTPROPERTIES, *LPEAXFXSLOTPROPERTIES;

typedef struct _EAXVECTOR {
    float x;
    float y;
    float z;
} EAXVECTOR;

typedef struct _EAXREVERBPROPERTIES
{
    unsigned int ulEnvironment;   // sets all reverb properties
    float flEnvironmentSize;       // environment size in meters
    float flEnvironmentDiffusion;  // environment diffusion
    int lRoom;                    // room effect level (at mid frequencies)
    int lRoomHF;                  // relative room effect level at high frequencies
    int lRoomLF;                  // relative room effect level at low frequencies
    float flDecayTime;             // reverberation decay time at mid frequencies
    float flDecayHFRatio;          // high-frequency to mid-frequency decay time ratio
    float flDecayLFRatio;          // low-frequency to mid-frequency decay time ratio
    int lReflections;             // early reflections level relative to room effect
    float flReflectionsDelay;      // initial reflection delay time
    EAXVECTOR vReflectionsPan;     // early reflections panning vector
    int lReverb;                  // late reverberation level relative to room effect
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
    unsigned int ulFlags;         // modifies the behavior of properties
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
    EAXSOURCE_ACTIVEFXSLOTID
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
    EAXREVERB_FLAGS
} EAXREVERB_PROPERTY;
extern const GUID EAX_REVERB_EFFECT;
extern const GUID EAXPROPERTYID_EAX40_FXSlot0;
extern const GUID EAXPROPERTYID_EAX40_FXSlot1;
extern const GUID EAXPROPERTYID_EAX40_FXSlot2;
extern const GUID EAXPROPERTYID_EAX40_FXSlot3;

extern const GUID EAX_PrimaryFXSlotID;
}
#endif

#ifndef FABLE_DSOUND3D_H
#define FABLE_DSOUND3D_H
// Public DirectSound SDK POD types; optional D3D_OVERLOADS are disabled.
typedef float D3DVALUE;
typedef struct _D3DVECTOR {
 union { D3DVALUE x; D3DVALUE dvX; };
 union { D3DVALUE y; D3DVALUE dvY; };
 union { D3DVALUE z; D3DVALUE dvZ; };
} D3DVECTOR, *LPD3DVECTOR;
typedef struct _DS3DBUFFER {
 unsigned int dwSize;
 D3DVECTOR vPosition,vVelocity;
 unsigned int dwInsideConeAngle,dwOutsideConeAngle;
 D3DVECTOR vConeOrientation;
 int lConeOutsideVolume;
 D3DVALUE flMinDistance,flMaxDistance;
 unsigned int dwMode;
} DS3DBUFFER,*LPDS3DBUFFER;
#endif

#ifndef FABLE_CODA_ENVIRONMENT_H
#define FABLE_CODA_ENVIRONMENT_H
// GameCODA2.10 public structures and original SDK COFF types.
typedef unsigned int CUINT32;
typedef struct _CVECTOR                             /* Cartesian vector structure for listener and  */
{                                                   /* 3D source parameters                         */
    float   x;                                      /* X, Y, Z co-ordinates                         */
    float   y;
    float   z;
}
CVECTOR;
typedef struct _CENVIRONMENT                        /* Environment parameters                       */
{                                       
    CVECTOR vReflectionsPan;                        /* Reflections position control vector          */
    CVECTOR vReverbPan;                             /* Reverb position control vector               */
    int     iLevel;                                 /* in mB                                        */
    int     iLevelHF;                               /* in mB                                        */
    int     iLevelLF;                               /* in mB                                        */
    float   fDecayTime;                             /* -60 dB decay time in seconds                 */
    float   fDecayHFRatio;                          /* Fraction of time for HF to decay             */
    float   fDecayLFRatio;                          /* Fraction of time for LF to decay             */
    int     iReflections;                           /* Early reflections level in mB                */
    float   fReflectionsDelay;                      /* Early reflections delay in seconds           */
    int     iReverb;                                /* Late reverberation level in mB               */
    float   fReverbDelay;                           /* Late reverberation delay in seconds          */
    float   fSize;                                  /* Room size in metres                          */                      
    float   fDiffusion;                             /* 0 - 1                                        */
    float   fDensity;                               /* 0 - 1                                        */
    float   fEchoTime;                              /* Time lag between echoes in seconds           */
    float   fEchoDepth;                             /* Echo effect strength                         */
    float   fModulationTime;                        /* Pitch modulation period in seconds           */         
    float   fModulationDepth;                       /* Pitch modulation strength                    */
    CUINT32 uiHFReference;                          /* Frequency for HF effects                     */
    CUINT32 uiLFReference;                          /* Frequency for LF effects                     */
    float   fRolloff;                               /* Rolloff with distance                        */
    int     iAirAbsorptionHF;                       /* Air absorption in mB/unit                    */
}
CENVIRONMENT;
class CCodaObjectBase {
public:
 virtual ~CCodaObjectBase();
};
class CCodaObject:public CCodaObjectBase {
public:
 virtual ~CCodaObject();
public:
 CCodaObject* m_pPrev;
 CCodaObject* m_pNext;
};
class CSalReverb;
class CCodaEnvironment:public CCodaObject {
public:
 virtual ~CCodaEnvironment();
 const CENVIRONMENT& GetParams()const{return m_Params;}
 unsigned int GetCommit()const{return m_ffCommit;}
public:
 CENVIRONMENT m_Params;
 unsigned int m_ffCommit;
 CSalReverb* m_pReverb;
};
#endif

#ifndef FABLE_EAX_NATIVE_TYPES_H
#define FABLE_EAX_NATIVE_TYPES_H
// Original SDK COFF type and field names. Native omits older EAX1/2 conversion state.
typedef struct _EAX30LISTENERPROPERTIES
{
    unsigned int ulEnvironment;   // sets all reverb properties
    float flEnvironmentSize;       // environment size in meters
    float flEnvironmentDiffusion;  // environment diffusion
    int lRoom;                    // room effect level (at mid frequencies)
    int lRoomHF;                  // relative room effect level at high frequencies
    int lRoomLF;                  // relative room effect level at low frequencies
    float flDecayTime;             // reverberation decay time at mid frequencies
    float flDecayHFRatio;          // high-frequency to mid-frequency decay time ratio
    float flDecayLFRatio;          // low-frequency to mid-frequency decay time ratio
    int lReflections;             // early reflections level relative to room effect
    float flReflectionsDelay;      // initial reflection delay time
    EAXVECTOR vReflectionsPan;     // early reflections panning vector
    int lReverb;                  // late reverberation level relative to room effect
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
    unsigned int ulFlags;         // modifies the behavior of properties
} EAX30LISTENERPROPERTIES, *LPEAX30LISTENERPROPERTIES;
typedef struct _EAX30BUFFERPROPERTIES
{
    int          lDirect;                 // direct path level (at low and mid frequencies)
    int          lDirectHF;               // relative direct path level at high frequencies
    int          lRoom;                   // room effect level (at low and mid frequencies)
    int          lRoomHF;                 // relative room effect level at high frequencies
    int          lObstruction;            // main obstruction control (attenuation at high frequencies)
    float         flObstructionLFRatio;    // obstruction low-frequency level re. main control
    int          lOcclusion;              // main occlusion control (attenuation at high frequencies)
    float         flOcclusionLFRatio;      // occlusion low-frequency level re. main control
    float         flOcclusionRoomRatio;    // relative occlusion control for room effect
    float         flOcclusionDirectRatio;  // relative occlusion control for direct path
    int          lExclusion;              // main exlusion control (attenuation at high frequencies)
    float         flExclusionLFRatio;      // exclusion low-frequency level re. main control
    int          lOutsideVolumeHF;        // outside sound cone level at high frequencies
    float         flDopplerFactor;         // like DS3D flDopplerFactor but per source
    float         flRolloffFactor;         // like DS3D flRolloffFactor but per source
    float         flRoomRolloffFactor;     // like DS3D flRolloffFactor but for room effect
    float         flAirAbsorptionFactor;   // multiplies EAXREVERB_AIRABSORPTIONHF
    unsigned int ulFlags;                 // modifies the behavior of properties
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
 int lVolume;
 float vPosition[3],vVelocity[3];
 unsigned int ulInsideConeAngle,ulOutsideConeAngle;
 float vConeOrientation[3];
 int lConeOutsideVolume;
 float flMinDistance,flMaxDistance;
 unsigned int ulMode;
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
 int volume;
 D3DVECTOR vPosition,vVelocity;
 unsigned int dwInsideConeAngle,dwOutsideConeAngle;
 D3DVECTOR vConeOrientation;
 int lConeOutsideVolume;
 float flMinDistance,flMaxDistance;
 unsigned int dwMode;
};
#endif

#ifndef FABLE_CODA_MEMORY_H
#define FABLE_CODA_MEMORY_H
// Original SDK Memory.obj matches both native callbacks and default allocation bodies.
extern "C" void* __cdecl CodaMalloc(unsigned int);
extern "C" void __cdecl CodaFree(void*);
#endif

#ifndef FABLE_CEAXLISTENER_H
#define FABLE_CEAXLISTENER_H
class CEAXBuffer;
class CEAXBufferList:public CCodaObjectBase {
public:
 virtual ~CEAXBufferList();
 CEAXBufferList* lpNext;
 CEAXBuffer* lpEAXBuffer;
};
// Layout: native ctor00c604e0, allocation0x4f0, vtable012b9340.
// Original SDK public/private names, with EAX1/2 conversion fields absent natively.
class CEAXListener:public CCodaObjectBase {
public:
 virtual ~CEAXListener();
 int GetEAX4FXSlot(unsigned int,unsigned int,void*,unsigned int,unsigned int*);
 void AddToChangesList(IDENTIFIER,unsigned int,void*,int);
 void SetEAX4ErrorCode(int code){m_lEAX4ErrorCode=code;}
 unsigned int m_ListenerSlotID;
 CEAXBufferList* m_lpBufferList;
 LISTENER m_Listener,m_Listener_Defer;
 SOURCE m_Source;
 DESTINATION m_Dest;
 unsigned int m_dwFlags,m_dwFlags_Defer,m_dwEAX3Changes,m_dwEAX3Changes_Defer;
 _CHANGES* lpChanges;
 float m_flDecayHFFactor;
 bool m_bEnsoniq;
 EAXVERSION m_EAXVersion;
public:
 int m_lEAX4ErrorCode;
 float m_flEAX4UnsizedDecayTime[4],m_flEAX4UnsizedDecayTimeSize[4];
 float m_flEAX4UnsizedReflections[4],m_flEAX4UnsizedReflectionsSize[4];
 float m_flEAX4UnsizedReflectionsDelay[4],m_flEAX4UnsizedReflectionsDelaySize[4];
 float m_flEAX4UnsizedReverb[4],m_flEAX4UnsizedReverbSize[4];
 float m_flEAX4UnsizedReverbDelay[4],m_flEAX4UnsizedReverbDelaySize[4];
 float m_flEAX4UnsizedEchoTime[4],m_flEAX4UnsizedEchoTimeSize[4];
 float m_flEAX4UnsizedModulationTime[4],m_flEAX4UnsizedModulationTimeSize[4];
 float m_flEAX3UnsizedDecayTime,m_flEAX3UnsizedDecayTimeSize;
 float m_flEAX3UnsizedReflections,m_flEAX3UnsizedReflectionsSize;
 float m_flEAX3UnsizedReflectionsDelay,m_flEAX3UnsizedReflectionsDelaySize;
 float m_flEAX3UnsizedReverb,m_flEAX3UnsizedReverbSize;
 float m_flEAX3UnsizedReverbDelay,m_flEAX3UnsizedReverbDelaySize;
 float m_flEAX3UnsizedEchoTime,m_flEAX3UnsizedEchoTimeSize;
 float m_flEAX3UnsizedModulationTime,m_flEAX3UnsizedModulationTimeSize;
};
#endif

#ifndef FABLE_CEAXBUFFER_H
#define FABLE_CEAXBUFFER_H
// Layout: native ctor00c61610, allocation0x3c0, vtable012b9344.
// SDK names and exact access/signatures; native state excludes EAX1/2 conversions.
class CEAXBuffer:public CCodaObjectBase {
public:
 virtual ~CEAXBuffer();
 int GetEAX4Source(unsigned int,void*,unsigned int,unsigned int*);
 int CommitEAX3();
 unsigned int m_dwEAX3Changes,m_dwEAX3Changes_Defer;
 bool m_bEAX4Active;
 EAXVERSION m_EAXVersion;
public:
 int Set3DBuffer(unsigned int,void*,unsigned int);
 CEAXListener* m_lpEAXListener;
 SOURCEBP m_Source;
 DESTINATIONBP m_Dest;
 BUFFER m_Buffer,m_Buffer_Defer;
 unsigned int m_dwFlags,m_dwFlags_Defer;
 int m_BufferID;
 // Remaining native0x4c storage differs from later SDK, and is untouched by these rows.
 unsigned char Opaque374[0x4c];
};
#endif

CCodaObjectBase::~CCodaObjectBase(){}
CEAXListener::~CEAXListener(){}
CEAXBuffer::~CEAXBuffer(){}
extern "C" const GUID EAX_REVERB_EFFECT={0xcf95c8f,0xa3cc,0x4849,{0xb0,0xb6,0x83,0x2e,0xcc,0x18,0x22,0xdf}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot0={0xc4d79f1e,0xf1ac,0x436b,{0xa8,0x1d,0xa7,0x38,0xe7,0x4,0x54,0x69}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot1={0x8c00e96,0x74be,0x4491,{0x93,0xaa,0xe8,0xad,0x35,0xa4,0x91,0x17}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot2={0x1d433b88,0xf0f6,0x4637,{0x91,0x9f,0x60,0xe7,0xe0,0x6b,0x5e,0xdd}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot3={0xefff08ea,0xc7d8,0x44ab,{0x93,0xad,0x6d,0xbd,0x5f,0x91,0x0,0x64}};
static void fill_words(void* p,unsigned n,unsigned seed){for(unsigned i=0;i<n/4;++i)((unsigned*)p)[i]=0x3f000000u+seed+i;}
// CEAXBuffer::GetEAX4Source(unsigned int,void*,unsigned int,unsigned int*) -- Read EAX source properties.
// Leaf: ?GetEAX4Source@CEAXBuffer@@QAEHKPAXKPAK@Z
#include <string.h>
int CEAXBuffer::GetEAX4Source(unsigned int property,void* data,unsigned int size,unsigned int* returned)
{
 int result=0;
 switch(property & 0x7fffffff) {
 case EAXSOURCE_NONE: *returned=0; break;
 case EAXSOURCE_ALLPARAMETERS:
  if(data && size >= 72) { *(EAXSOURCEPROPERTIES*)data=*(const EAXSOURCEPROPERTIES*)&m_Source.eax4BP; *returned=72; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_OBSTRUCTIONPARAMETERS:
  if(data && size >= 8) { ((EAXOBSTRUCTIONPROPERTIES*)data)->lObstruction=m_Source.eax4BP.lObstruction;((EAXOBSTRUCTIONPROPERTIES*)data)->flObstructionLFRatio=m_Source.eax4BP.flObstructionLFRatio; *returned=8; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_OCCLUSIONPARAMETERS:
  if(data && size >= 16) { ((EAXOCCLUSIONPROPERTIES*)data)->lOcclusion=m_Source.eax4BP.lOcclusion;((EAXOCCLUSIONPROPERTIES*)data)->flOcclusionLFRatio=m_Source.eax4BP.flOcclusionLFRatio;((EAXOCCLUSIONPROPERTIES*)data)->flOcclusionRoomRatio=m_Source.eax4BP.flOcclusionRoomRatio;((EAXOCCLUSIONPROPERTIES*)data)->flOcclusionDirectRatio=m_Source.eax4BP.flOcclusionDirectRatio; *returned=16; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_EXCLUSIONPARAMETERS:
  if(data && size >= 8) { ((EAXEXCLUSIONPROPERTIES*)data)->lExclusion=m_Source.eax4BP.lExclusion;((EAXEXCLUSIONPROPERTIES*)data)->flExclusionLFRatio=m_Source.eax4BP.flExclusionLFRatio; *returned=8; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 5:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lDirect,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 6:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lDirectHF,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 7:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lRoom,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 8:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lRoomHF,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 9:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lObstruction,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 10:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flObstructionLFRatio,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 11:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lOcclusion,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 12:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flOcclusionLFRatio,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 13:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flOcclusionRoomRatio,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 14:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flOcclusionDirectRatio,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 15:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lExclusion,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 16:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flExclusionLFRatio,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 17:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.lOutsideVolumeHF,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 18:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flDopplerFactor,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 19:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flRolloffFactor,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 20:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flRoomRolloffFactor,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 21:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.flAirAbsorptionFactor,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case 22:
  if(data && size >= 4) { memcpy(data,&m_Source.eax4BP.ulFlags,4); *returned=4; }
  else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_SENDPARAMETERS:
  if(data && size >= 24) {
   unsigned int count=size/24; if(count > 4) count=4;
   memcpy(data,&m_Source.eax4BPSend,count*24);
   *returned=count*24;
  } else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_ALLSENDPARAMETERS:
  if(data && size >= 48) {
   unsigned int count=size/48; if(count > 4) count=4;
   EAXSOURCEALLSENDPROPERTIES* sends=(EAXSOURCEALLSENDPROPERTIES*)data;
   for(unsigned int i=0;i<count;++i) {
    switch(i) {
     case 0: sends[0].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot0; break;
     case 1: sends[1].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot1; break;
     case 2: sends[2].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot2; break;
     case 3: sends[3].guidReceivingFXSlotID=EAXPROPERTYID_EAX40_FXSlot3; break;
    }
    sends[i].lOcclusion=m_Source.eax4BPOcclusionSend[i].lOcclusion;sends[i].flOcclusionLFRatio=m_Source.eax4BPOcclusionSend[i].flOcclusionLFRatio;sends[i].flOcclusionRoomRatio=m_Source.eax4BPOcclusionSend[i].flOcclusionRoomRatio;sends[i].flOcclusionDirectRatio=m_Source.eax4BPOcclusionSend[i].flOcclusionDirectRatio;
    sends[i].lExclusion=m_Source.eax4BPExclusionSend[i].lExclusion;sends[i].flExclusionLFRatio=m_Source.eax4BPExclusionSend[i].flExclusionLFRatio;
    sends[i].lSend=m_Source.eax4BPSend[i].lSend; sends[i].lSendHF=m_Source.eax4BPSend[i].lSendHF;
   }
   *returned=count*48;
  } else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_OCCLUSIONSENDPARAMETERS:
  if(data && size >= 32) {
   unsigned int count=size/32; if(count > 4) count=4;
   memcpy(data,&m_Source.eax4BPOcclusionSend,count*32);
   *returned=count*32;
  } else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_EXCLUSIONSENDPARAMETERS:
  if(data && size >= 24) {
   unsigned int count=size/24; if(count > 4) count=4;
   memcpy(data,&m_Source.eax4BPExclusionSend,count*24);
   *returned=count*24;
  } else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 case EAXSOURCE_ACTIVEFXSLOTID:
  if(data && size >= 16) {
   unsigned int count=size/16; if(count > 2) count=2;
   memcpy(data,&m_Source.eax4BPActiveFXSlotID,count*16);
   *returned=count*16;
  } else { m_lpEAXListener->SetEAX4ErrorCode(-2); result=-3; }
  break;
 default: return -3;
 }
 return result;
}
int main() {
#ifdef _MSC_VER
 assert(sizeof(CEAXListener)==0x4f0);assert(sizeof(CEAXBuffer)==0x3c0);
 assert(sizeof(_CHANGES)==0x98);assert(sizeof(BUFFER)==0x8c);
 assert(offsetof(CEAXListener,m_Source)==0x84);assert(offsetof(CEAXListener,m_lEAX4ErrorCode)==0x3d4);
 assert(offsetof(CEAXBuffer,m_Source)==0x18);assert(offsetof(CEAXBuffer,m_Buffer)==0x250);
 assert(offsetof(CEAXBuffer,m_dwFlags)==0x368);assert(offsetof(CEAXBuffer,m_BufferID)==0x370);
#endif

 CEAXBuffer source; CEAXListener context; source.m_lpEAXListener=&context;
 fill_words(&source.m_Source.eax4BP,sizeof(source.m_Source.eax4BP),1);fill_words(source.m_Source.eax4BPSend,sizeof(source.m_Source.eax4BPSend),41);
 fill_words(source.m_Source.eax4BPOcclusionSend,sizeof(source.m_Source.eax4BPOcclusionSend),71);fill_words(source.m_Source.eax4BPExclusionSend,sizeof(source.m_Source.eax4BPExclusionSend),101);
 fill_words(&source.m_Source.eax4BPActiveFXSlotID,sizeof(source.m_Source.eax4BPActiveFXSlotID),131);
 const GUID* ids[4]={&EAXPROPERTYID_EAX40_FXSlot0,&EAXPROPERTYID_EAX40_FXSlot1,&EAXPROPERTYID_EAX40_FXSlot2,&EAXPROPERTYID_EAX40_FXSlot3};
 EAXSOURCEALLSENDPROPERTIES all[4];
 for(unsigned i=0;i<4;++i) {
  all[i].guidReceivingFXSlotID=*ids[i];all[i].lSend=source.m_Source.eax4BPSend[i].lSend;all[i].lSendHF=source.m_Source.eax4BPSend[i].lSendHF;
  all[i].lOcclusion=source.m_Source.eax4BPOcclusionSend[i].lOcclusion;all[i].flOcclusionLFRatio=source.m_Source.eax4BPOcclusionSend[i].flOcclusionLFRatio;
  all[i].flOcclusionRoomRatio=source.m_Source.eax4BPOcclusionSend[i].flOcclusionRoomRatio;all[i].flOcclusionDirectRatio=source.m_Source.eax4BPOcclusionSend[i].flOcclusionDirectRatio;
  all[i].lExclusion=source.m_Source.eax4BPExclusionSend[i].lExclusion;all[i].flExclusionLFRatio=source.m_Source.eax4BPExclusionSend[i].flExclusionLFRatio;
 }
 for(unsigned id=0;id<=28;++id) for(unsigned high=0;high<2;++high) for(unsigned size=0;size<=210;++size) for(unsigned nullData=0;nullData<2;++nullData) {
  unsigned char out[224],expect[224];memset(out,0x79,sizeof(out));memset(expect,0x79,sizeof(expect));unsigned returned=0x7bb7;context.m_lEAX4ErrorCode=91;
  unsigned required=id==1?72:id==2||id==4?8:id==3?16:id>=5&&id<=22?4:id==23||id==26?24:id==24?48:id==25?32:id==27?16:0;
  bool okay=id<28 && (id==0 || (!nullData && size>=required));unsigned count=1;
  if(id>=23) { count=required?size/required:0;unsigned cap=id==27?2:4;if(count>cap)count=cap; }
  unsigned bytes=id==0?0:required*count;const void* expected=0;
  if(id==1)expected=&source.m_Source.eax4BP;else if(id==2)expected=&source.m_Source.eax4BP.lObstruction;else if(id==3)expected=&source.m_Source.eax4BP.lOcclusion;else if(id==4)expected=&source.m_Source.eax4BP.lExclusion;
  else if(id>=5&&id<=22)expected=(const unsigned char*)&source.m_Source.eax4BP+(id-5)*4;
  else if(id==23)expected=source.m_Source.eax4BPSend;else if(id==24)expected=all;else if(id==25)expected=source.m_Source.eax4BPOcclusionSend;else if(id==26)expected=source.m_Source.eax4BPExclusionSend;else if(id==27)expected=&source.m_Source.eax4BPActiveFXSlotID;
  if(okay && bytes)memcpy(expect,expected,bytes);
  int result=source.GetEAX4Source(id|(high?0x80000000u:0),nullData?0:out,size,&returned);
  assert(result==(okay?0:-3));assert(returned==(okay?bytes:0x7bb7));assert(context.m_lEAX4ErrorCode==((id>0&&id<28&&!okay)?-2:91));assert(memcmp(out,expect,sizeof(out))==0);
 }
 puts("00c66c90_TEST PASS"); return 0;
}