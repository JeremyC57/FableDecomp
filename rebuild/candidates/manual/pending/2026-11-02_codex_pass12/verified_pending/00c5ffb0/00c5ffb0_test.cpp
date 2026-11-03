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
extern "C" void* __cdecl CodaMalloc(unsigned int n){void* p=malloc(n);assert(p);memset(p,0x79,n);return p;}
// void CEAXListener::AddToChangesList(IDENTIFIER,unsigned int,void*,int) -- append a typed EAX or source-property notification.
// Leaf: ?AddToChangesList@CEAXListener@@QAEXW4IDENTIFIER@@KPAXJ@Z
void CEAXListener::AddToChangesList(IDENTIFIER group,unsigned int property,void* value,int sourceID)
{
 _CHANGES* event=(_CHANGES*)CodaMalloc(sizeof(_CHANGES));
 event->id=group;
 event->property=property;
 switch(group){
 case 4:
  switch(property){
  case 4:event->Listener.flDistanceFactor=*(const float*)value;break;
  case 5:event->Listener.flRolloffFactor=*(const float*)value;break;
  }
  break;
 case 5:
  event->Buffer.BufferID=sourceID;
  switch(property){
  case 0x20:event->Buffer.lConeOutsideVolume=*(const int*)value;break;
  case 0x200:event->Buffer.lVolume=*(const unsigned int*)value;break;
  }
  break;
 case 0:
  switch(property){
  case 1:event->eax30LP=*(const EAX30LISTENERPROPERTIES*)value;break;
  case 2:event->eax30LP.ulEnvironment=*(const unsigned int*)value;break;
  case 3:event->eax30LP.flEnvironmentSize=*(const float*)value;break;
  case 4:event->eax30LP.flEnvironmentDiffusion=*(const float*)value;break;
  case 5:event->eax30LP.lRoom=*(const int*)value;break;
  case 6:event->eax30LP.lRoomHF=*(const int*)value;break;
  case 7:event->eax30LP.lRoomLF=*(const int*)value;break;
  case 8:event->eax30LP.flDecayTime=*(const float*)value;break;
  case 9:event->eax30LP.flDecayHFRatio=*(const float*)value;break;
  case 10:event->eax30LP.flDecayLFRatio=*(const float*)value;break;
  case 11:event->eax30LP.lReflections=*(const int*)value;break;
  case 12:event->eax30LP.flReflectionsDelay=*(const float*)value;break;
  case 13:event->eax30LP.vReflectionsPan=*(const EAXVECTOR*)value;break;
  case 14:event->eax30LP.lReverb=*(const int*)value;break;
  case 15:event->eax30LP.flReverbDelay=*(const float*)value;break;
  case 16:event->eax30LP.vReverbPan=*(const EAXVECTOR*)value;break;
  case 17:event->eax30LP.flEchoTime=*(const float*)value;break;
  case 18:event->eax30LP.flEchoDepth=*(const float*)value;break;
  case 19:event->eax30LP.flModulationTime=*(const float*)value;break;
  case 20:event->eax30LP.flModulationDepth=*(const float*)value;break;
  case 21:event->eax30LP.flAirAbsorptionHF=*(const float*)value;break;
  case 22:event->eax30LP.flHFReference=*(const float*)value;break;
  case 23:event->eax30LP.flLFReference=*(const float*)value;break;
  case 24:event->eax30LP.flRoomRolloffFactor=*(const float*)value;break;
  case 25:event->eax30LP.ulFlags=*(const unsigned int*)value;break;
  }
  break;
 case 1:
  event->Buffer.BufferID=sourceID;
  switch(property){
  case 1:event->Buffer.eax30BP=*(const EAX30BUFFERPROPERTIES*)value;break;
  case 2:*(EAXOBSTRUCTIONPROPERTIES*)&event->Buffer.eax30BP.lObstruction=*(const EAXOBSTRUCTIONPROPERTIES*)value;break;
  case 3:*(EAXOCCLUSIONPROPERTIES*)&event->Buffer.eax30BP.lOcclusion=*(const EAXOCCLUSIONPROPERTIES*)value;break;
  case 4:*(EAXEXCLUSIONPROPERTIES*)&event->Buffer.eax30BP.lExclusion=*(const EAXEXCLUSIONPROPERTIES*)value;break;
  case 5:event->Buffer.eax30BP.lDirect=*(const int*)value;break;
  case 6:event->Buffer.eax30BP.lDirectHF=*(const int*)value;break;
  case 7:event->Buffer.eax30BP.lRoom=*(const int*)value;break;
  case 8:event->Buffer.eax30BP.lRoomHF=*(const int*)value;break;
  case 9:event->Buffer.eax30BP.lObstruction=*(const int*)value;break;
  case 10:event->Buffer.eax30BP.flObstructionLFRatio=*(const float*)value;break;
  case 11:event->Buffer.eax30BP.lOcclusion=*(const int*)value;break;
  case 12:event->Buffer.eax30BP.flOcclusionLFRatio=*(const float*)value;break;
  case 13:event->Buffer.eax30BP.flOcclusionRoomRatio=*(const float*)value;break;
  case 14:event->Buffer.eax30BP.flOcclusionDirectRatio=*(const float*)value;break;
  case 15:event->Buffer.eax30BP.lExclusion=*(const int*)value;break;
  case 16:event->Buffer.eax30BP.flExclusionLFRatio=*(const float*)value;break;
  case 17:event->Buffer.eax30BP.lOutsideVolumeHF=*(const int*)value;break;
  case 18:event->Buffer.eax30BP.flDopplerFactor=*(const float*)value;break;
  case 19:event->Buffer.eax30BP.flRolloffFactor=*(const float*)value;break;
  case 20:event->Buffer.eax30BP.flRoomRolloffFactor=*(const float*)value;break;
  case 21:event->Buffer.eax30BP.flAirAbsorptionFactor=*(const float*)value;break;
  case 22:event->Buffer.eax30BP.ulFlags=*(const unsigned int*)value;break;
  }
  break;
 }
 event->next=0;
 _CHANGES* last=lpChanges;
 if(!last)lpChanges=event;
 else{
  while(last->next)last=last->next;
  last->next=event;
 }

}
int main(){
#ifdef _MSC_VER
 assert(sizeof(CEAXListener)==0x4f0);assert(sizeof(CEAXBuffer)==0x3c0);
 assert(sizeof(_CHANGES)==0x98);assert(sizeof(BUFFER)==0x8c);
 assert(offsetof(CEAXListener,m_Source)==0x84);assert(offsetof(CEAXListener,m_lEAX4ErrorCode)==0x3d4);
 assert(offsetof(CEAXBuffer,m_Source)==0x18);assert(offsetof(CEAXBuffer,m_Buffer)==0x250);
 assert(offsetof(CEAXBuffer,m_dwFlags)==0x368);assert(offsetof(CEAXBuffer,m_BufferID)==0x370);
#endif

 CEAXListener context;context.lpChanges=0;_CHANGES* previous=0;
 unsigned buffer[36];for(unsigned i=0;i<36;++i)buffer[i]=0x3f000000u+i*0x8000u;
 unsigned offsets[26]={0,8,8,12,16,20,24,28,32,36,40,44,48,52,64,68,72,84,88,92,96,100,104,108,112,116};
 unsigned ids[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,32,511,512,513,0xffffffffu};
 for(unsigned group=0;group<8;++group)for(unsigned j=0;j<sizeof(ids)/sizeof(ids[0]);++j){
  unsigned property=ids[j],sourceID=0x12ab6789u+j;
  unsigned char expected[0x94];memset(expected,0x79,sizeof(expected));memcpy(expected,&group,4);memcpy(expected+4,&property,4);
  unsigned offset=0,count=0;
  if(group==0&&property>=1&&property<=25){offset=offsets[property];count=property==1?112:property==13||property==16?12:4;}
  if(group==1){memcpy(expected+8,&sourceID,4);if(property==1){offset=0x4c;count=72;}else if(property>=2&&property<=4){offset=property==2?0x5c:property==3?0x64:0x74;count=property==3?16:8;}else if(property>=5&&property<=22){offset=0x4c+(property-5)*4;count=4;}}
  if(group==4&&(property==4||property==5)){offset=property==4?0x38:0x3c;count=4;}
  if(group==5){memcpy(expected+8,&sourceID,4);if(property==32||property==512){offset=property==32?0x3c:0xc;count=4;}}
  if(count)memcpy(expected+offset,buffer,count);
  context.AddToChangesList((IDENTIFIER)group,property,buffer,sourceID);
  _CHANGES* event=previous?previous->next:context.lpChanges;
  assert(memcmp(event,expected,0x94)==0);assert(event->next==0);
  if(previous)assert(previous->next==event);else assert(context.lpChanges==event);
  previous=event;
 }
 unsigned count=0;_CHANGES* item=context.lpChanges;
 while(item){_CHANGES* next=item->next;free(item);item=next;++count;}
 assert(count==8*sizeof(ids)/sizeof(ids[0]));
 puts("00c5ffb0_TEST PASS");return 0;
}
