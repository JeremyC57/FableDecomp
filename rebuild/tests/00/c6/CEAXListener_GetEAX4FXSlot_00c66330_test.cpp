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
// CEAXListener::GetEAX4FXSlot(unsigned int,unsigned int,void*,unsigned int,unsigned int*) -- Read EAX effect-slot and reverb properties.
// Leaf: ?GetEAX4FXSlot@CEAXListener@@QAEHKKPAXKPAK@Z
#include <string.h>
int CEAXListener::GetEAX4FXSlot(unsigned int slot,unsigned int property,void* data,unsigned int size,unsigned int* returned)
{
 int result=0;
 if(slot>=4) return -3;
 unsigned int parameter=property&0x7fffffff;
 if(parameter<0x10000) {
  if(memcmp(&m_Source.eax4FXSlot[slot].guidLoadEffect,&EAX_REVERB_EFFECT,sizeof(GUID))!=0) { m_lEAX4ErrorCode=-3; return -3; }
  switch(parameter) {
  case EAXREVERB_NONE: *returned=0; break;
  case 1:
   if(data && size >= 112) { *(EAXREVERBPROPERTIES*)data=m_Source.eax4LPSlot[slot]; *returned=112; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 2:
   if(data && size >= 4) { *(unsigned int*)data=m_Source.eax4LPSlot[slot].ulEnvironment; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 3:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flEnvironmentSize; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 4:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flEnvironmentDiffusion; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 5:
   if(data && size >= 4) { *(int*)data=m_Source.eax4LPSlot[slot].lRoom; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 6:
   if(data && size >= 4) { *(int*)data=m_Source.eax4LPSlot[slot].lRoomHF; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 7:
   if(data && size >= 4) { *(int*)data=m_Source.eax4LPSlot[slot].lRoomLF; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 8:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flDecayTime; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 9:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flDecayHFRatio; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 10:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flDecayLFRatio; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 11:
   if(data && size >= 4) { *(int*)data=m_Source.eax4LPSlot[slot].lReflections; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 12:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flReflectionsDelay; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 13:
   if(data && size >= 12) { *(EAXVECTOR*)data=m_Source.eax4LPSlot[slot].vReflectionsPan; *returned=12; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 14:
   if(data && size >= 4) { *(int*)data=m_Source.eax4LPSlot[slot].lReverb; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 15:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flReverbDelay; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 16:
   if(data && size >= 12) { *(EAXVECTOR*)data=m_Source.eax4LPSlot[slot].vReverbPan; *returned=12; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 17:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flEchoTime; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 18:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flEchoDepth; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 19:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flModulationTime; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 20:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flModulationDepth; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 21:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flAirAbsorptionHF; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 22:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flHFReference; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 23:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flLFReference; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 24:
   if(data && size >= 4) { *(float*)data=m_Source.eax4LPSlot[slot].flRoomRolloffFactor; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case 25:
   if(data && size >= 4) { *(unsigned int*)data=m_Source.eax4LPSlot[slot].ulFlags; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  default: return -3;
  }
 } else {
  switch(property) {
  case EAXFXSLOT_NONE: *returned=0; break;
  case EAXFXSLOT_ALLPARAMETERS:
   if(data && size >= 28) { *(EAXFXSLOTPROPERTIES*)data=m_Source.eax4FXSlot[slot]; *returned=28; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case EAXFXSLOT_LOADEFFECT:
   if(data && size >= 16) { *(GUID*)data=m_Source.eax4FXSlot[slot].guidLoadEffect; *returned=16; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case EAXFXSLOT_VOLUME:
   if(data && size >= 4) { *(int*)data=m_Source.eax4FXSlot[slot].lVolume; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case EAXFXSLOT_LOCK:
   if(data && size >= 4) { *(int*)data=m_Source.eax4FXSlot[slot].lLock; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  case EAXFXSLOT_FLAGS:
   if(data && size >= 4) { *(unsigned int*)data=m_Source.eax4FXSlot[slot].ulFlags; *returned=4; }
   else { m_lEAX4ErrorCode=-2; result=-3; }
   break;
  default: return -3;
  }
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

 CEAXListener context;
 for(unsigned i=0;i<4;++i) { fill_words(&context.m_Source.eax4LPSlot[i],112,100*i);fill_words(&context.m_Source.eax4FXSlot[i],28,500+i);context.m_Source.eax4FXSlot[i].guidLoadEffect=EAX_REVERB_EFFECT; }
 unsigned offsets[26]={0,0,0,4,8,12,16,20,24,28,32,36,40,44,56,60,64,76,80,84,88,92,96,100,104,108};
 for(unsigned slot=0;slot<=4;++slot)for(unsigned selector=0;selector<33;++selector)for(unsigned high=0;high<2;++high)for(unsigned size=0;size<=113;++size)for(unsigned nullData=0;nullData<2;++nullData) {
  unsigned id=selector<26?selector:0x10000+selector-26;unsigned property=id|(high?0x80000000u:0);
  unsigned required=id==0||id==0x10000?0:id==1?112:id==13||id==16?12:id==0x10001?28:id==0x10002?16:4;
  bool valid=slot<4&&(id<26||(id<=0x10005&&!high));bool okay=valid&&(required==0||(!nullData&&size>=required));
  unsigned char out[128],expect[128];memset(out,0x79,sizeof(out));memset(expect,0x79,sizeof(expect));unsigned returned=0x7bb7;context.m_lEAX4ErrorCode=91;
  if(okay && required) {const void* from=id<26?(void*)((char*)&context.m_Source.eax4LPSlot[slot]+offsets[id]):id==0x10001?(void*)&context.m_Source.eax4FXSlot[slot]:id==0x10002?(void*)&context.m_Source.eax4FXSlot[slot].guidLoadEffect:(void*)((char*)&context.m_Source.eax4FXSlot[slot]+16+(id-0x10003)*4);memcpy(expect,from,required);}
  int result=context.GetEAX4FXSlot(slot,property,nullData?0:out,size,&returned);
  assert(result==(okay?0:-3));assert(returned==(okay?required:0x7bb7));assert(context.m_lEAX4ErrorCode==(valid&&!okay?-2:91));assert(memcmp(out,expect,sizeof(out))==0);
 }
 context.m_Source.eax4FXSlot[1].guidLoadEffect.Data1^=1;unsigned returned=71;unsigned out=123;context.m_lEAX4ErrorCode=91;
 assert(context.GetEAX4FXSlot(1,2,&out,4,&returned)==-3);assert(out==123&&returned==71&&context.m_lEAX4ErrorCode==-3);
 assert(context.GetEAX4FXSlot(1,0x10003,&out,4,&returned)==0);
 puts("00c66330_TEST PASS");return 0;
}