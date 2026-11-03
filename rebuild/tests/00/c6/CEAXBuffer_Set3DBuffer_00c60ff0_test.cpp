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
static unsigned queueCalls,commitCalls,recordID;static int recordVolume;static CEAXListener* expectedContext;
void CEAXListener::AddToChangesList(IDENTIFIER group,unsigned int property,void* value,int sourceID){assert(this==expectedContext&&group==BUFFERUPDATE&&property==32);++queueCalls;memcpy(&recordVolume,value,4);recordID=sourceID;}
int CEAXBuffer::CommitEAX3(){++commitCalls;return 17;}
bool IsNaN(const D3DVECTOR& v){return v.x!=v.x||v.y!=v.y||v.z!=v.z;}
// int CEAXBuffer::Set3DBuffer(unsigned int,void*,unsigned int) -- update immediate/deferred DirectSound 3D source properties.
// Leaf: ?Set3DBuffer@CEAXBuffer@@AAEHKPAXK@Z
bool IsNaN(const D3DVECTOR&);
int CEAXBuffer::Set3DBuffer(unsigned int property,void* value,unsigned int deferred)
{
 int result=0;
 switch(property){
 case 1:{
  const DS3DBUFFER* parameters=(const DS3DBUFFER*)value;
  if(IsNaN((const D3DVECTOR&)parameters->vPosition)||IsNaN((const D3DVECTOR&)parameters->vVelocity)){result=-3;break;}
  if(deferred!=1){
   m_Buffer.vPosition=parameters->vPosition;
   m_Buffer.vVelocity=parameters->vVelocity;
   m_Buffer.dwInsideConeAngle=parameters->dwInsideConeAngle;
   m_Buffer.dwOutsideConeAngle=parameters->dwOutsideConeAngle;
   m_Buffer.vConeOrientation=parameters->vConeOrientation;
   m_Buffer.lConeOutsideVolume=parameters->lConeOutsideVolume;
   m_Buffer.flMinDistance=parameters->flMinDistance;
   m_Buffer.flMaxDistance=parameters->flMaxDistance;
   m_Buffer.dwMode=parameters->dwMode;
   m_dwFlags|=0x754;
  }else m_dwFlags_Defer|=0x754;
  m_Buffer_Defer.vPosition=parameters->vPosition;
  m_Buffer_Defer.vVelocity=parameters->vVelocity;
  m_Buffer_Defer.dwInsideConeAngle=parameters->dwInsideConeAngle;
  m_Buffer_Defer.dwOutsideConeAngle=parameters->dwOutsideConeAngle;
  m_Buffer_Defer.vConeOrientation=parameters->vConeOrientation;
  m_Buffer_Defer.lConeOutsideVolume=parameters->lConeOutsideVolume;
  m_Buffer_Defer.flMinDistance=parameters->flMinDistance;
  m_Buffer_Defer.flMaxDistance=parameters->flMaxDistance;
  m_Buffer_Defer.dwMode=parameters->dwMode;
  int volume=m_Buffer_Defer.lConeOutsideVolume;
  m_lpEAXListener->AddToChangesList(BUFFERUPDATE,0x20,&volume,m_BufferID);
  break;
 }
 case 2:{
  const D3DVECTOR* vector=(const D3DVECTOR*)value;
  if(IsNaN((const D3DVECTOR&)*vector)){result=-3;break;}
  if(deferred!=1){m_Buffer.vPosition=*vector;m_dwFlags|=0x4;}
  else m_dwFlags_Defer|=0x4;
  m_Buffer_Defer.vPosition=*vector;
  break;
 }
 case 4:{
  const D3DVECTOR* vector=(const D3DVECTOR*)value;
  if(IsNaN((const D3DVECTOR&)*vector)){result=-3;break;}
  if(deferred!=1){m_Buffer.vVelocity=*vector;m_dwFlags|=0x10;}
  else m_dwFlags_Defer|=0x10;
  m_Buffer_Defer.vVelocity=*vector;
  break;
 }
 case 8:
  if(deferred!=1){m_Buffer.dwInsideConeAngle=((const unsigned int*)value)[0];m_Buffer.dwOutsideConeAngle=((const unsigned int*)value)[1];m_dwFlags|=0x400;}
  else m_dwFlags_Defer|=0x400;
  m_Buffer_Defer.dwInsideConeAngle=((const unsigned int*)value)[0];m_Buffer_Defer.dwOutsideConeAngle=((const unsigned int*)value)[1];
  break;
 case 16:
  if(deferred!=1){m_Buffer.vConeOrientation=*(const D3DVECTOR*)value;m_dwFlags|=0x40;}
  else m_dwFlags_Defer|=0x40;
  m_Buffer_Defer.vConeOrientation=*(const D3DVECTOR*)value;
  break;
 case 32:{
  int volume=*(const int*)value;
  if(m_Buffer_Defer.lConeOutsideVolume==volume)break;
  m_Buffer_Defer.lConeOutsideVolume=volume;
  if(deferred!=1){m_Buffer.lConeOutsideVolume=volume;m_dwFlags|=0x400;}
  else m_dwFlags_Defer|=0x400;
  break;
 }
 case 64:
  if(deferred!=1){m_Buffer.flMinDistance=*(const float*)value;m_dwFlags|=0x104;}
  else m_dwFlags_Defer|=0x104;
  m_Buffer_Defer.flMinDistance=*(const float*)value;
  break;
 case 128:
  if(deferred!=1){m_Buffer.flMaxDistance=*(const float*)value;m_dwFlags|=0x4;}
  else m_dwFlags_Defer|=0x4;
  m_Buffer_Defer.flMaxDistance=*(const float*)value;
  break;
 case 256:
  if(deferred!=1){m_Buffer.dwMode=*(const unsigned int*)value;m_dwFlags|=0x754;}
  else m_dwFlags_Defer|=0x754;
  m_Buffer_Defer.dwMode=*(const unsigned int*)value;
  break;
 default:result=-3;break;
 }
 if(deferred==1)m_dwFlags|=1;
 else m_dwFlags&=~1;
 if(result>=0){
  if(m_dwFlags&1)return result;
  return CommitEAX3();
 }
 return result;
}
int main(){
#ifdef _MSC_VER
 assert(sizeof(CEAXListener)==0x4f0);assert(sizeof(CEAXBuffer)==0x3c0);
 assert(sizeof(_CHANGES)==0x98);assert(sizeof(BUFFER)==0x8c);
 assert(offsetof(CEAXListener,m_Source)==0x84);assert(offsetof(CEAXListener,m_lEAX4ErrorCode)==0x3d4);
 assert(offsetof(CEAXBuffer,m_Source)==0x18);assert(offsetof(CEAXBuffer,m_Buffer)==0x250);
 assert(offsetof(CEAXBuffer,m_dwFlags)==0x368);assert(offsetof(CEAXBuffer,m_BufferID)==0x370);
#endif

 CEAXListener context;CEAXBuffer source;expectedContext=&context;source.m_lpEAXListener=&context;source.m_BufferID=199;
 unsigned properties[]={0,1,2,3,4,8,16,32,64,128,256,512,0xffffffffu};
 int modes[]={-1,0,1,2};
 for(unsigned selector=0;selector<sizeof(properties)/sizeof(properties[0]);++selector)for(unsigned mode=0;mode<4;++mode)for(unsigned variant=0;variant<9;++variant){
  unsigned property=properties[selector];int deferred=modes[mode];
  DS3DBUFFER input;memset(&input,0,sizeof(input));input.dwSize=sizeof(input);
  input.vPosition.x=1.0f;input.vPosition.y=2.0f;input.vPosition.z=4.0f;
  input.vVelocity.x=-1.0f;input.vVelocity.y=-2.0f;input.vVelocity.z=-4.0f;
  input.dwInsideConeAngle=80;input.dwOutsideConeAngle=160;input.vConeOrientation.x=0.5f;input.vConeOrientation.y=0.25f;input.vConeOrientation.z=0.125f;
  input.lConeOutsideVolume=-50;input.flMinDistance=2;input.flMaxDistance=32;input.dwMode=2;
  unsigned scalar=7;float distance=8.0f;unsigned angles[2]={45,90};D3DVECTOR vector={};vector.x=1;vector.y=2;vector.z=4;
  if(variant>=1&&variant<=6){unsigned invalid=0x7fc00000u;memcpy((char*)&input.vPosition+(variant-1)*4,&invalid,4);if(variant<=3)memcpy((char*)&vector+(variant-1)*4,&invalid,4);}
  void* data=property==1?(void*)&input:property==2||property==4||property==16?(void*)&vector:property==8?(void*)angles:property==64||property==128?(void*)&distance:(void*)&scalar;
  memset(&source.m_Buffer,0x79,sizeof(source.m_Buffer));memset(&source.m_Buffer_Defer,0x61,sizeof(source.m_Buffer_Defer));
  if(variant==7)source.m_Buffer_Defer.lConeOutsideVolume=(int)scalar;
  source.m_dwFlags=variant==8?0:0xaaaa0031u;source.m_dwFlags_Defer=0x44440002u;
  unsigned expectDirty=source.m_dwFlags,expectDeferredDirty=source.m_dwFlags_Defer;
  unsigned char current[sizeof(source.m_Buffer)],pending[sizeof(source.m_Buffer_Defer)];memcpy(current,&source.m_Buffer,sizeof(current));memcpy(pending,&source.m_Buffer_Defer,sizeof(pending));
  bool invalid=property==1&&variant>=1&&variant<=6 || (property==2||property==4)&&variant>=1&&variant<=3;
  bool valid=property==1||property==2||property==4||property==8||property==16||property==32||property==64||property==128||property==256;
  unsigned offset=0,bytes=0,mask=0;
  if(valid&&!invalid){
   if(property==1){offset=0x50;bytes=60;data=(char*)&input+4;mask=0x754;}
   else if(property==2){offset=0x50;bytes=12;mask=4;}
   else if(property==4){offset=0x5c;bytes=12;mask=16;}
   else if(property==8){offset=0x68;bytes=8;mask=0x400;}
   else if(property==16){offset=0x70;bytes=12;mask=0x40;}
   else if(property==32){offset=0x7c;bytes=4;mask=0x400;if(variant==7){bytes=0;mask=0;}}
   else if(property==64){offset=0x80;bytes=4;mask=0x104;}
   else if(property==128){offset=0x84;bytes=4;mask=4;}
   else if(property==256){offset=0x88;bytes=4;mask=0x754;}
   if(bytes){memcpy(pending+offset,data,bytes);if(deferred!=1)memcpy(current+offset,data,bytes);}
   if(deferred==1)expectDeferredDirty|=mask;else expectDirty|=mask;
  }
  if(deferred==1)expectDirty|=1;else expectDirty&=~1u;
  queueCalls=commitCalls=0;
  data=property==1?(void*)&input:property==2||property==4||property==16?(void*)&vector:property==8?(void*)angles:property==64||property==128?(void*)&distance:(void*)&scalar;
  int result=source.Set3DBuffer(property,data,deferred);bool okay=valid&&!invalid;
  assert(result==(!okay?-3:deferred==1?0:17));assert(source.m_dwFlags==expectDirty);assert(source.m_dwFlags_Defer==expectDeferredDirty);
  assert(memcmp(current,&source.m_Buffer,sizeof(current))==0&&memcmp(pending,&source.m_Buffer_Defer,sizeof(pending))==0);
  assert(commitCalls==(okay&&deferred!=1?1u:0u));assert(queueCalls==(okay&&property==1?1u:0u));
  if(queueCalls)assert(recordVolume==input.lConeOutsideVolume&&recordID==199);
 }
 puts("00c60ff0_TEST PASS");return 0;
}