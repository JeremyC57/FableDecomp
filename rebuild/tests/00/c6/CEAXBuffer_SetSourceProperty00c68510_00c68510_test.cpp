// Self-contained C++03 property-dispatch behavior fixture.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#ifndef _MSC_VER
#define __cdecl
#endif
#ifdef _MSC_VER
typedef long EaxLong;typedef unsigned long EaxULong;
#else
typedef int EaxLong;typedef unsigned int EaxULong;
#endif
#ifndef FABLE_EAX40_TYPES_H
#define FABLE_EAX40_TYPES_H
// Creative EAX 4.0 API, revision 1.6.3, 21 February 2003.
// Original public SDK tags and members; all fields naturally align to four bytes.
#define EAX_MAX_ACTIVE_FXSLOTS 2
extern "C" {
typedef struct _GUID
        {
            EaxULong Data1;
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
    EaxLong          lDirect;                 // direct path level (at low and mid frequencies)
    EaxLong          lDirectHF;               // relative direct path level at high frequencies
    EaxLong          lRoom;                   // room effect level (at low and mid frequencies)
    EaxLong          lRoomHF;                 // relative room effect level at high frequencies
    EaxLong          lObstruction;            // main obstruction control (attenuation at high frequencies)
    float         flObstructionLFRatio;    // obstruction low-frequency level re. main control
    EaxLong          lOcclusion;              // main occlusion control (attenuation at high frequencies)
    float         flOcclusionLFRatio;      // occlusion low-frequency level re. main control
    float         flOcclusionRoomRatio;    // relative occlusion control for room effect
    float         flOcclusionDirectRatio;  // relative occlusion control for direct path
    EaxLong          lExclusion;              // main exlusion control (attenuation at high frequencies)
    float         flExclusionLFRatio;      // exclusion low-frequency level re. main control
    EaxLong          lOutsideVolumeHF;        // outside sound cone level at high frequencies
    float         flDopplerFactor;         // like DS3D flDopplerFactor but per source
    float         flRolloffFactor;         // like DS3D flRolloffFactor but per source
    float         flRoomRolloffFactor;     // like DS3D flRolloffFactor but for room effect
    float         flAirAbsorptionFactor;   // multiplies EAXREVERB_AIRABSORPTIONHF
    EaxULong ulFlags;                 // modifies the behavior of properties
} EAXSOURCEPROPERTIES, *LPEAXSOURCEPROPERTIES;

typedef struct _EAXSOURCEALLSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	EaxLong          lSend;                   // send level (at low and mid frequencies)
	EaxLong          lSendHF;                 // relative send level at high frequencies
	EaxLong          lOcclusion;
	float         flOcclusionLFRatio;
	float         flOcclusionRoomRatio;
	float         flOcclusionDirectRatio;
	EaxLong          lExclusion;
	float         flExclusionLFRatio;
} EAXSOURCEALLSENDPROPERTIES, *LPEAXSOURCEALLSENDPROPERTIES;

typedef struct _EAXACTIVEFXSLOTS
{
    GUID          guidActiveFXSlots[EAX_MAX_ACTIVE_FXSLOTS];
} EAXACTIVEFXSLOTS, *LPEAXACTIVEFXSLOTS;

typedef struct _EAXOBSTRUCTIONPROPERTIES
{
    EaxLong          lObstruction;
    float         flObstructionLFRatio;
} EAXOBSTRUCTIONPROPERTIES, *LPEAXOBSTRUCTIONPROPERTIES;

typedef struct _EAXOCCLUSIONPROPERTIES
{
    EaxLong          lOcclusion;
    float         flOcclusionLFRatio;
    float         flOcclusionRoomRatio;
    float         flOcclusionDirectRatio;
} EAXOCCLUSIONPROPERTIES, *LPEAXOCCLUSIONPROPERTIES;

typedef struct _EAXEXCLUSIONPROPERTIES
{
    EaxLong          lExclusion;
    float         flExclusionLFRatio;
} EAXEXCLUSIONPROPERTIES, *LPEAXEXCLUSIONPROPERTIES;

typedef struct _EAXSOURCESENDPROPERTIES
{
    GUID          guidReceivingFXSlotID;
    EaxLong          lSend;
    EaxLong          lSendHF;
} EAXSOURCESENDPROPERTIES, *LPEAXSOURCESENDPROPERTIES;

typedef struct _EAXSOURCEOCCLUSIONSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	EaxLong          lOcclusion;
	float         flOcclusionLFRatio;
	float         flOcclusionRoomRatio;
	float         flOcclusionDirectRatio;
} EAXSOURCEOCCLUSIONSENDPROPERTIES, *LPEAXSOURCEOCCLUSIONSENDPROPERTIES;

typedef struct _EAXSOURCEEXCLUSIONSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	EaxLong          lExclusion;
	float         flExclusionLFRatio;
} EAXSOURCEEXCLUSIONSENDPROPERTIES, *LPEAXSOURCEEXCLUSIONSENDPROPERTIES;

typedef struct _EAXFXSLOTPROPERTIES
{
	GUID          guidLoadEffect;
	EaxLong          lVolume;
	EaxLong          lLock;
	EaxULong ulFlags;
} EAXFXSLOTPROPERTIES, *LPEAXFXSLOTPROPERTIES;

typedef struct _EAXVECTOR {
    float x;
    float y;
    float z;
} EAXVECTOR;

typedef struct _EAXREVERBPROPERTIES
{
    EaxULong ulEnvironment;   // sets all reverb properties
    float flEnvironmentSize;       // environment size in meters
    float flEnvironmentDiffusion;  // environment diffusion
    EaxLong lRoom;                    // room effect level (at mid frequencies)
    EaxLong lRoomHF;                  // relative room effect level at high frequencies
    EaxLong lRoomLF;                  // relative room effect level at low frequencies
    float flDecayTime;             // reverberation decay time at mid frequencies
    float flDecayHFRatio;          // high-frequency to mid-frequency decay time ratio
    float flDecayLFRatio;          // low-frequency to mid-frequency decay time ratio
    EaxLong lReflections;             // early reflections level relative to room effect
    float flReflectionsDelay;      // initial reflection delay time
    EAXVECTOR vReflectionsPan;     // early reflections panning vector
    EaxLong lReverb;                  // late reverberation level relative to room effect
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
    EaxULong ulFlags;         // modifies the behavior of properties
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
}
#endif

#ifndef FABLE_CPKOBJECT_H
#define FABLE_CPKOBJECT_H
// Layout: libpk root vtable 012b5868; child destructors restore it at offset zero.
// Descriptive library identity, no donor PDB record. One virtual destructor slot.
class CCodaObjectBase {
public:
 virtual ~CCodaObjectBase();
 static void* __cdecl operator new(size_t);
 static void __cdecl operator delete(void*);
};
#endif

#ifndef FABLE_CEAXCONTEXT_H
#define FABLE_CEAXCONTEXT_H
// Layout: context vtable 012b9340, derived from root 012b5868. Allocation at
// 00c4e307 is 0x4f0 bytes; constructor 00c604e0 initializes this complete span.
// Class/member names are descriptive; native EAX protocol roles are documented.
struct _CHANGES {
 EaxULong m_flags,m_type,m_environment;
 unsigned char Opaque0c[0x88];
 _CHANGES* m_pNext;
};
class CEAXListener : public CCodaObjectBase {
public:
 virtual ~CEAXListener();
 EaxLong m_primarySlot;
 unsigned char Opaque008[0x7c];
 EAXFXSLOTPROPERTIES m_slots[4];
 EAXREVERBPROPERTIES m_reverb[4];
 EAXCONTEXTPROPERTIES m_context;
 EAXREVERBPROPERTIES m_defaultReverb;
 EAXREVERBPROPERTIES m_committed;
 unsigned char Opaque3b0[0x0c];
 EaxULong m_dirty;
 unsigned char Opaque3c0[4];
 _CHANGES* m_pEvents;
 unsigned char Opaque3c8[0x0c];
 EaxLong m_lastError;
 float m_decayTimeBase[4],m_decayTimeSize[4],m_reflectionsBase[4],m_reflectionsSize[4];
 float m_reflectionsDelayBase[4],m_reflectionsDelaySize[4],m_reverbBase[4],m_reverbSize[4];
 float m_reverbDelayBase[4],m_reverbDelaySize[4],m_echoTimeBase[4],m_echoTimeSize[4];
 float m_modulationTimeBase[4],m_modulationTimeSize[4];
 unsigned char Opaque4b8[0x38];
 void ClearEvents();
 EaxLong CommitDeferredSettings();
 EaxLong SetEffectProperty_00c68f90(EaxULong,EaxULong,const void*,EaxULong,_CHANGES**);
 EaxLong SetReverbAllParameters(EaxULong,const EAXREVERBPROPERTIES*); // 00c68d70
 EaxLong SetEnvironment(EaxULong,EaxULong); // 00c68a50
 EaxLong SetEnvironmentSize(EaxULong,float); // 00c677c0
 EaxLong SetEnvironmentDiffusion(EaxULong,float); // 00c631e0
 EaxLong SetRoom(EaxULong,EaxLong); // 00c63270
 EaxLong SetRoomHF(EaxULong,EaxLong); // 00c632e0
 EaxLong SetRoomLF(EaxULong,EaxLong); // 00c63350
 EaxLong SetDecayTime(EaxULong,float,bool,bool); // 00c633c0
 EaxLong SetDecayHFRatio(EaxULong,float); // 00c63480
 EaxLong SetDecayLFRatio(EaxULong,float); // 00c63520
 EaxLong SetReflections(EaxULong,EaxLong,bool,bool); // 00c635c0
 EaxLong SetReflectionsDelay(EaxULong,float,bool,bool); // 00c63690
 EaxLong SetReflectionsPan(EaxULong,EAXVECTOR); // 00c63750
 EaxLong SetReverb(EaxULong,EaxLong,bool,bool); // 00c63810
 EaxLong SetReverbDelay(EaxULong,float,bool,bool); // 00c638e0
 EaxLong SetReverbPan(EaxULong,EAXVECTOR); // 00c639a0
 EaxLong SetEchoTime(EaxULong,float,bool,bool); // 00c63a60
 EaxLong SetEchoDepth(EaxULong,float); // 00c63b20
 EaxLong SetModulationTime(EaxULong,float,bool,bool); // 00c63bc0
 EaxLong SetModulationDepth(EaxULong,float); // 00c63c80
 EaxLong SetSlotAirAbsorptionHF(EaxULong,float); // 00c63d20
 EaxLong SetSlotHFReference(EaxULong,float); // 00c63db0
 EaxLong SetLFReference(EaxULong,float); // 00c63e50
 EaxLong SetRoomRolloffFactor(EaxULong,float); // 00c63ef0
 EaxLong SetReverbFlags(EaxULong,EaxULong); // 00c67a10
 EaxLong SetSlotProperty(EaxULong,const EAXFXSLOTPROPERTIES&); // 00c689b0
 EaxLong SetSlotLoadEffect(EaxULong,GUID); // 00c674b0
 EaxLong SetSlotVolume(EaxULong,EaxLong); // 00c67720
 EaxLong SetSlotLock(EaxULong,EaxLong); // 00c67470
 EaxLong SetSlotFlags(EaxULong,EaxULong); // 00c67430
};
#endif

#ifndef FABLE_CEAXSOURCE_H
#define FABLE_CEAXSOURCE_H
// Layout: source vtable 012b9344, root 012b5868. Allocation at 00c42eaa is
// 0x3c0 bytes. Constructor 00c61610 proves context+0x14 and remaining storage.
// Names outside the EAX protocol are descriptive; see NAMING_EVIDENCE.md.
class CEAXBuffer : public CCodaObjectBase {
public:
 virtual ~CEAXBuffer();
 EaxULong m_dirty;
 unsigned char Opaque008[0x0c];
 CEAXListener* m_pContext;
 EAXSOURCEPROPERTIES m_current;
 EAXSOURCESENDPROPERTIES m_sends[4];
 EAXSOURCEOCCLUSIONSENDPROPERTIES m_occlusionSends[4];
 EAXSOURCEEXCLUSIONSENDPROPERTIES m_exclusionSends[4];
 EAXACTIVEFXSLOTS m_activeSlots;
 EAXSOURCEPROPERTIES m_defaultProperties,m_deferred;
 unsigned char Opaque250[0x170];
 EaxLong CommitDeferredSettings();
 EaxLong SetSourceProperty_00c68510(EaxULong,const void*,EaxULong,_CHANGES**);
 EaxLong SetAllParameters(const EAXSOURCEPROPERTIES*); // 00c67c00
 EaxLong SetObstructionParameters(const EAXOBSTRUCTIONPROPERTIES*); // 00c647b0
 EaxLong SetOcclusionParameters(const EAXOCCLUSIONPROPERTIES*); // 00c64840
 EaxLong SetExclusionParameters(const EAXEXCLUSIONPROPERTIES*); // 00c64940
 EaxLong SetDirect(EaxLong); // 00c649c0
 EaxLong SetDirectHF(EaxLong); // 00c64a00
 EaxLong SetRoom(EaxLong); // 00c64a40
 EaxLong SetRoomHF(EaxLong); // 00c64a80
 EaxLong SetObstruction(EaxLong); // 00c64ac0
 EaxLong SetObstructionLFRatio(float); // 00c64b00
 EaxLong SetOcclusion(EaxLong); // 00c64b70
 EaxLong SetOcclusionLFRatio(float); // 00c64bb0
 EaxLong SetOcclusionRoomRatio(float); // 00c64c10
 EaxLong SetOcclusionDirectRatio(float); // 00c64c70
 EaxLong SetExclusion(EaxLong); // 00c64cd0
 EaxLong SetExclusionLFRatio(float); // 00c64d10
 EaxLong SetOutsideVolumeHF(EaxLong); // 00c64d70
 EaxLong SetDopplerFactor(float); // 00c64db0
 EaxLong SetRolloffFactor(float); // 00c64e20
 EaxLong SetRoomRolloffFactor(float); // 00c64e90
 EaxLong SetAirAbsorptionFactor(float); // 00c64f00
 EaxLong SetFlags(EaxULong); // 00c64f70
 EaxLong SetSendParameters(const EAXSOURCESENDPROPERTIES*,EaxULong); // 00c64fb0
 EaxLong SetAllSendParameters(const EAXSOURCEALLSENDPROPERTIES*,EaxULong); // 00c65170
 EaxLong SetOcclusionSendParameters(const EAXSOURCEOCCLUSIONSENDPROPERTIES*,EaxULong); // 00c65450
 EaxLong SetExclusionSendParameters(const EAXSOURCEEXCLUSIONSENDPROPERTIES*,EaxULong); // 00c65690
 EaxLong SetActiveFXSlotID(const GUID*,EaxULong); // 00c65870
};
#endif

// CEAXBuffer::SetSourceProperty_00c68510(unsigned long,void const *,unsigned long,_CHANGES * *) -- dispatch EAX 4 source updates and return committed notifications.
// Leaf: ?SetSourceProperty_00c68510@CEAXBuffer@@QAEJKPBXKPAPAU_CHANGES@@@Z
EaxLong CEAXBuffer::SetSourceProperty_00c68510(EaxULong property,const void* value,EaxULong size,_CHANGES** events)
{
 EaxLong result=-3;
 m_pContext->ClearEvents();
 switch(property&0x7fffffff){
 case EAXSOURCE_NONE:result=0;break;
 case EAXSOURCE_ALLPARAMETERS:
  if(value&&size>=sizeof(EAXSOURCEPROPERTIES))result=SetAllParameters((const EAXSOURCEPROPERTIES*)value);
  break;
 case EAXSOURCE_OBSTRUCTIONPARAMETERS:
  if(value&&size>=sizeof(EAXOBSTRUCTIONPROPERTIES))result=SetObstructionParameters((const EAXOBSTRUCTIONPROPERTIES*)value);
  break;
 case EAXSOURCE_OCCLUSIONPARAMETERS:
  if(value&&size>=sizeof(EAXOCCLUSIONPROPERTIES))result=SetOcclusionParameters((const EAXOCCLUSIONPROPERTIES*)value);
  break;
 case EAXSOURCE_EXCLUSIONPARAMETERS:
  if(value&&size>=sizeof(EAXEXCLUSIONPROPERTIES))result=SetExclusionParameters((const EAXEXCLUSIONPROPERTIES*)value);
  break;
 case EAXSOURCE_DIRECT:
  if(value&&size>=sizeof(EaxLong))result=SetDirect(*(const EaxLong*)value);
  break;
 case EAXSOURCE_DIRECTHF:
  if(value&&size>=sizeof(EaxLong))result=SetDirectHF(*(const EaxLong*)value);
  break;
 case EAXSOURCE_ROOM:
  if(value&&size>=sizeof(EaxLong))result=SetRoom(*(const EaxLong*)value);
  break;
 case EAXSOURCE_ROOMHF:
  if(value&&size>=sizeof(EaxLong))result=SetRoomHF(*(const EaxLong*)value);
  break;
 case EAXSOURCE_OBSTRUCTION:
  if(value&&size>=sizeof(EaxLong))result=SetObstruction(*(const EaxLong*)value);
  break;
 case EAXSOURCE_OBSTRUCTIONLFRATIO:
  if(value&&size>=sizeof(float))result=SetObstructionLFRatio(*(const float*)value);
  break;
 case EAXSOURCE_OCCLUSION:
  if(value&&size>=sizeof(EaxLong))result=SetOcclusion(*(const EaxLong*)value);
  break;
 case EAXSOURCE_OCCLUSIONLFRATIO:
  if(value&&size>=sizeof(float))result=SetOcclusionLFRatio(*(const float*)value);
  break;
 case EAXSOURCE_OCCLUSIONROOMRATIO:
  if(value&&size>=sizeof(float))result=SetOcclusionRoomRatio(*(const float*)value);
  break;
 case EAXSOURCE_OCCLUSIONDIRECTRATIO:
  if(value&&size>=sizeof(float))result=SetOcclusionDirectRatio(*(const float*)value);
  break;
 case EAXSOURCE_EXCLUSION:
  if(value&&size>=sizeof(EaxLong))result=SetExclusion(*(const EaxLong*)value);
  break;
 case EAXSOURCE_EXCLUSIONLFRATIO:
  if(value&&size>=sizeof(float))result=SetExclusionLFRatio(*(const float*)value);
  break;
 case EAXSOURCE_OUTSIDEVOLUMEHF:
  if(value&&size>=sizeof(EaxLong))result=SetOutsideVolumeHF(*(const EaxLong*)value);
  break;
 case EAXSOURCE_DOPPLERFACTOR:
  if(value&&size>=sizeof(float))result=SetDopplerFactor(*(const float*)value);
  break;
 case EAXSOURCE_ROLLOFFFACTOR:
  if(value&&size>=sizeof(float))result=SetRolloffFactor(*(const float*)value);
  break;
 case EAXSOURCE_ROOMROLLOFFFACTOR:
  if(value&&size>=sizeof(float))result=SetRoomRolloffFactor(*(const float*)value);
  break;
 case EAXSOURCE_AIRABSORPTIONFACTOR:
  if(value&&size>=sizeof(float))result=SetAirAbsorptionFactor(*(const float*)value);
  break;
 case EAXSOURCE_FLAGS:
  if(value&&size>=sizeof(EaxULong))result=SetFlags(*(const EaxULong*)value);
  break;
 case EAXSOURCE_SENDPARAMETERS:
  if(value&&size>=sizeof(EAXSOURCESENDPROPERTIES))result=SetSendParameters((const EAXSOURCESENDPROPERTIES*)value,size);
  break;
 case EAXSOURCE_ALLSENDPARAMETERS:
  if(value&&size>=sizeof(EAXSOURCEALLSENDPROPERTIES))result=SetAllSendParameters((const EAXSOURCEALLSENDPROPERTIES*)value,size);
  break;
 case EAXSOURCE_OCCLUSIONSENDPARAMETERS:
  if(value&&size>=sizeof(EAXSOURCEOCCLUSIONSENDPROPERTIES))result=SetOcclusionSendParameters((const EAXSOURCEOCCLUSIONSENDPROPERTIES*)value,size);
  break;
 case EAXSOURCE_EXCLUSIONSENDPARAMETERS:
  if(value&&size>=sizeof(EAXSOURCEEXCLUSIONSENDPROPERTIES))result=SetExclusionSendParameters((const EAXSOURCEEXCLUSIONSENDPROPERTIES*)value,size);
  break;
 case EAXSOURCE_ACTIVEFXSLOTID:
  if(value&&size>=sizeof(GUID))result=SetActiveFXSlotID((const GUID*)value,size);
  break;
 default:result=-3;break;
 }
 if(result>=0&&!(property&0x80000000)){
  result=CommitDeferredSettings();
  if(result>=0)*events=m_pContext->m_pEvents;
 }
 return result;
}


extern "C" const GUID EAX_REVERB_EFFECT={0x0cf95c8f,0xa3cc,0x4849,{0xb0,0xb6,0x83,0x2e,0xcc,0x18,0x22,0xdf}};
static _CHANGES ClearedEvent,CommittedEvent,InitialEvent;
static int Clears,Commits,SetterCalls,SeenCase,SeenFlags,FixtureCases;
static EaxLong SetterResult,CommitResult;
static EaxULong SeenSlot,SeenSize;
static const void* SeenPointer;
static unsigned char SeenValue[32];
CCodaObjectBase::~CCodaObjectBase(){}
CEAXListener::~CEAXListener(){}
CEAXBuffer::~CEAXBuffer(){}
void* __cdecl CCodaObjectBase::operator new(size_t size){return malloc(size);}
void __cdecl CCodaObjectBase::operator delete(void* p){free(p);}
void CEAXListener::ClearEvents(){++Clears;m_pEvents=&ClearedEvent;}
EaxLong CEAXListener::CommitDeferredSettings(){++Commits;m_pEvents=&CommittedEvent;return CommitResult;}
EaxLong CEAXBuffer::CommitDeferredSettings(){return m_pContext->CommitDeferredSettings();}
static void ResetTest(){Clears=Commits=SetterCalls=0;SeenCase=-1;SeenSlot=SeenSize=0xffffffffUL;SeenPointer=0;SeenFlags=-1;SetterResult=CommitResult=0;memset(SeenValue,0,sizeof(SeenValue));++FixtureCases;}
EaxLong CEAXBuffer::SetAllParameters(const EAXSOURCEPROPERTIES* value){++SetterCalls;SeenCase=1;SeenPointer=value;return SetterResult;}
EaxLong CEAXBuffer::SetObstructionParameters(const EAXOBSTRUCTIONPROPERTIES* value){++SetterCalls;SeenCase=2;SeenPointer=value;return SetterResult;}
EaxLong CEAXBuffer::SetOcclusionParameters(const EAXOCCLUSIONPROPERTIES* value){++SetterCalls;SeenCase=3;SeenPointer=value;return SetterResult;}
EaxLong CEAXBuffer::SetExclusionParameters(const EAXEXCLUSIONPROPERTIES* value){++SetterCalls;SeenCase=4;SeenPointer=value;return SetterResult;}
EaxLong CEAXBuffer::SetDirect(EaxLong value){++SetterCalls;SeenCase=5;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetDirectHF(EaxLong value){++SetterCalls;SeenCase=6;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetRoom(EaxLong value){++SetterCalls;SeenCase=7;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetRoomHF(EaxLong value){++SetterCalls;SeenCase=8;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetObstruction(EaxLong value){++SetterCalls;SeenCase=9;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetObstructionLFRatio(float value){++SetterCalls;SeenCase=10;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetOcclusion(EaxLong value){++SetterCalls;SeenCase=11;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetOcclusionLFRatio(float value){++SetterCalls;SeenCase=12;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetOcclusionRoomRatio(float value){++SetterCalls;SeenCase=13;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetOcclusionDirectRatio(float value){++SetterCalls;SeenCase=14;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetExclusion(EaxLong value){++SetterCalls;SeenCase=15;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetExclusionLFRatio(float value){++SetterCalls;SeenCase=16;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetOutsideVolumeHF(EaxLong value){++SetterCalls;SeenCase=17;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetDopplerFactor(float value){++SetterCalls;SeenCase=18;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetRolloffFactor(float value){++SetterCalls;SeenCase=19;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetRoomRolloffFactor(float value){++SetterCalls;SeenCase=20;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetAirAbsorptionFactor(float value){++SetterCalls;SeenCase=21;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetFlags(EaxULong value){++SetterCalls;SeenCase=22;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXBuffer::SetSendParameters(const EAXSOURCESENDPROPERTIES* value,EaxULong size){++SetterCalls;SeenCase=23;SeenSize=size;SeenPointer=value;return SetterResult;}
EaxLong CEAXBuffer::SetAllSendParameters(const EAXSOURCEALLSENDPROPERTIES* value,EaxULong size){++SetterCalls;SeenCase=24;SeenSize=size;SeenPointer=value;return SetterResult;}
EaxLong CEAXBuffer::SetOcclusionSendParameters(const EAXSOURCEOCCLUSIONSENDPROPERTIES* value,EaxULong size){++SetterCalls;SeenCase=25;SeenSize=size;SeenPointer=value;return SetterResult;}
EaxLong CEAXBuffer::SetExclusionSendParameters(const EAXSOURCEEXCLUSIONSENDPROPERTIES* value,EaxULong size){++SetterCalls;SeenCase=26;SeenSize=size;SeenPointer=value;return SetterResult;}
EaxLong CEAXBuffer::SetActiveFXSlotID(const GUID* value,EaxULong size){++SetterCalls;SeenCase=27;SeenSize=size;SeenPointer=value;return SetterResult;}
EaxLong CEAXListener::SetReverbAllParameters(EaxULong slot,const EAXREVERBPROPERTIES* value){++SetterCalls;SeenCase=1;SeenSlot=slot;SeenPointer=value;return SetterResult;}
EaxLong CEAXListener::SetEnvironment(EaxULong slot,EaxULong value){++SetterCalls;SeenCase=2;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetEnvironmentSize(EaxULong slot,float value){++SetterCalls;SeenCase=3;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetEnvironmentDiffusion(EaxULong slot,float value){++SetterCalls;SeenCase=4;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetRoom(EaxULong slot,EaxLong value){++SetterCalls;SeenCase=5;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetRoomHF(EaxULong slot,EaxLong value){++SetterCalls;SeenCase=6;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetRoomLF(EaxULong slot,EaxLong value){++SetterCalls;SeenCase=7;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetDecayTime(EaxULong slot,float value,bool record,bool clamp){++SetterCalls;SeenCase=8;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));SeenFlags=(record?1:0)|(clamp?2:0);return SetterResult;}
EaxLong CEAXListener::SetDecayHFRatio(EaxULong slot,float value){++SetterCalls;SeenCase=9;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetDecayLFRatio(EaxULong slot,float value){++SetterCalls;SeenCase=10;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetReflections(EaxULong slot,EaxLong value,bool record,bool clamp){++SetterCalls;SeenCase=11;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));SeenFlags=(record?1:0)|(clamp?2:0);return SetterResult;}
EaxLong CEAXListener::SetReflectionsDelay(EaxULong slot,float value,bool record,bool clamp){++SetterCalls;SeenCase=12;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));SeenFlags=(record?1:0)|(clamp?2:0);return SetterResult;}
EaxLong CEAXListener::SetReflectionsPan(EaxULong slot,EAXVECTOR value){++SetterCalls;SeenCase=13;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetReverb(EaxULong slot,EaxLong value,bool record,bool clamp){++SetterCalls;SeenCase=14;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));SeenFlags=(record?1:0)|(clamp?2:0);return SetterResult;}
EaxLong CEAXListener::SetReverbDelay(EaxULong slot,float value,bool record,bool clamp){++SetterCalls;SeenCase=15;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));SeenFlags=(record?1:0)|(clamp?2:0);return SetterResult;}
EaxLong CEAXListener::SetReverbPan(EaxULong slot,EAXVECTOR value){++SetterCalls;SeenCase=16;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetEchoTime(EaxULong slot,float value,bool record,bool clamp){++SetterCalls;SeenCase=17;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));SeenFlags=(record?1:0)|(clamp?2:0);return SetterResult;}
EaxLong CEAXListener::SetEchoDepth(EaxULong slot,float value){++SetterCalls;SeenCase=18;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetModulationTime(EaxULong slot,float value,bool record,bool clamp){++SetterCalls;SeenCase=19;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));SeenFlags=(record?1:0)|(clamp?2:0);return SetterResult;}
EaxLong CEAXListener::SetModulationDepth(EaxULong slot,float value){++SetterCalls;SeenCase=20;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetSlotAirAbsorptionHF(EaxULong slot,float value){++SetterCalls;SeenCase=21;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetSlotHFReference(EaxULong slot,float value){++SetterCalls;SeenCase=22;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetLFReference(EaxULong slot,float value){++SetterCalls;SeenCase=23;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetRoomRolloffFactor(EaxULong slot,float value){++SetterCalls;SeenCase=24;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetReverbFlags(EaxULong slot,EaxULong value){++SetterCalls;SeenCase=25;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetSlotProperty(EaxULong slot,const EAXFXSLOTPROPERTIES& value){++SetterCalls;SeenCase=65537;SeenSlot=slot;SeenPointer=&value;return SetterResult;}
EaxLong CEAXListener::SetSlotLoadEffect(EaxULong slot,GUID value){++SetterCalls;SeenCase=65538;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetSlotVolume(EaxULong slot,EaxLong value){++SetterCalls;SeenCase=65539;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetSlotLock(EaxULong slot,EaxLong value){++SetterCalls;SeenCase=65540;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}
EaxLong CEAXListener::SetSlotFlags(EaxULong slot,EaxULong value){++SetterCalls;SeenCase=65541;SeenSlot=slot;memcpy(SeenValue,&value,sizeof(value));return SetterResult;}

static void CheckLayout(CEAXListener& context,CEAXBuffer& source)
{
#ifdef _MSC_VER
 assert(sizeof(GUID)==16&&sizeof(EAXVECTOR)==12&&sizeof(EAXSOURCEPROPERTIES)==0x48&&sizeof(EAXREVERBPROPERTIES)==0x70);
 assert(sizeof(CEAXListener)==0x4f0&&sizeof(CEAXBuffer)==0x3c0&&sizeof(_CHANGES)==0x98);
 assert((char*)&context.m_slots-(char*)&context==0x84);assert((char*)&context.m_pEvents-(char*)&context==0x3c4);assert((char*)&context.m_lastError-(char*)&context==0x3d4);
 assert((char*)&source.m_pContext-(char*)&source==0x14);assert((char*)&source.m_deferred-(char*)&source==0x208);assert((char*)&source.m_activeSlots-(char*)&source==0x1a0);
#else
 (void)context;(void)source;
#endif
}
struct CaseRecord{EaxULong property,size;int kind;};
static const CaseRecord Cases[]={{0,0,0},{1,sizeof(EAXSOURCEPROPERTIES),2},{2,sizeof(EAXOBSTRUCTIONPROPERTIES),2},{3,sizeof(EAXOCCLUSIONPROPERTIES),2},{4,sizeof(EAXEXCLUSIONPROPERTIES),2},{5,sizeof(EaxLong),1},{6,sizeof(EaxLong),1},{7,sizeof(EaxLong),1},{8,sizeof(EaxLong),1},{9,sizeof(EaxLong),1},{10,sizeof(float),1},{11,sizeof(EaxLong),1},{12,sizeof(float),1},{13,sizeof(float),1},{14,sizeof(float),1},{15,sizeof(EaxLong),1},{16,sizeof(float),1},{17,sizeof(EaxLong),1},{18,sizeof(float),1},{19,sizeof(float),1},{20,sizeof(float),1},{21,sizeof(float),1},{22,sizeof(EaxULong),1},{23,sizeof(EAXSOURCESENDPROPERTIES),3},{24,sizeof(EAXSOURCEALLSENDPROPERTIES),3},{25,sizeof(EAXSOURCEOCCLUSIONSENDPROPERTIES),3},{26,sizeof(EAXSOURCEEXCLUSIONSENDPROPERTIES),3},{27,sizeof(GUID),3}};
int main(){CEAXListener context;CEAXBuffer source;source.m_pContext=&context;CheckLayout(context,source);for(int i=0;i<4;++i)context.m_slots[i].guidLoadEffect=EAX_REVERB_EFFECT;
 union{float floats[40];unsigned char bytes[160];}payload;for(int i=0;i<40;++i)payload.floats[i]=(float)(i+2);
 const void* value=payload.bytes;EaxULong slot=2,size,property;_CHANGES*events;
 for(unsigned int index=0;index<sizeof(Cases)/sizeof(Cases[0]);++index){
 const CaseRecord& c=Cases[index];
 ResetTest();property=c.property;size=c.size;value=payload.bytes;events=&InitialEvent;context.m_lastError=97;
 assert(source.SetSourceProperty_00c68510(property,value,size,&events)==0);assert(Clears==1&&Commits==1&&events==&CommittedEvent);assert(SetterCalls==(c.kind!=0));
 if(c.kind){assert(SeenCase==(int)c.property);if(c.kind==2||c.kind==3)assert(SeenPointer==payload.bytes);else assert(!memcmp(SeenValue,payload.bytes,c.size));if(c.kind==3)assert(SeenSize==size);if(c.kind==4)assert(SeenFlags==1);}
 ResetTest();property=c.property|0x80000000UL;events=&InitialEvent;
 assert(source.SetSourceProperty_00c68510(property,value,size,&events)==0);assert(Clears==1&&Commits==0&&events==&InitialEvent);assert(SetterCalls==(c.kind!=0));
 if(c.kind){
 ResetTest();property=c.property;value=0;size=c.size;events=&InitialEvent;assert(source.SetSourceProperty_00c68510(property,value,size,&events)==-3);assert(Clears==1&&!SetterCalls&&!Commits&&events==&InitialEvent);
 ResetTest();value=payload.bytes;size=c.size-1;events=&InitialEvent;assert(source.SetSourceProperty_00c68510(property,value,size,&events)==-3);assert(Clears==1&&!SetterCalls&&!Commits&&events==&InitialEvent);
 ResetTest();size=c.size;SetterResult=-12;events=&InitialEvent;assert(source.SetSourceProperty_00c68510(property,value,size,&events)==-12);assert(SetterCalls==1&&!Commits&&events==&InitialEvent);
 ResetTest();CommitResult=-13;events=&InitialEvent;assert(source.SetSourceProperty_00c68510(property,value,size,&events)==-13);assert(SetterCalls==1&&Commits==1&&events==&InitialEvent);
 ResetTest();property=c.property|0x80000000UL;SetterResult=3;events=&InitialEvent;assert(source.SetSourceProperty_00c68510(property,value,size,&events)==3);assert(Commits==0&&events==&InitialEvent);
 ResetTest();property=c.property;size=c.size+16;CommitResult=5;events=&InitialEvent;assert(source.SetSourceProperty_00c68510(property,value,size,&events)==5);assert(SetterCalls==1&&Commits==1&&events==&CommittedEvent);if(c.kind==3)assert(SeenSize==size);
 }
 }
 ResetTest();property=0x7fffffffUL;size=160;value=payload.bytes;events=&InitialEvent;assert(source.SetSourceProperty_00c68510(property,value,size,&events)==-3);assert(Clears==1&&!Commits&&!SetterCalls&&events==&InitialEvent);
puts("00c68510_TEST PASS");return 0;}