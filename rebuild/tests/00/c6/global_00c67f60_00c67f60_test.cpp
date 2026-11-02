// Self-contained C++03 EAX commit behavior fixture.
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
extern const GUID EAX_PrimaryFXSlotID;
}
#endif

#ifndef FABLE_CPKOBJECT_H
#define FABLE_CPKOBJECT_H
// Layout: libpk root vtable 012b5868; child destructors restore it at offset zero.
// Descriptive library identity, no donor PDB record. One virtual destructor slot.
class CPKObject {
public:
 virtual ~CPKObject();
 static void* __cdecl operator new(size_t);
 static void __cdecl operator delete(void*);
};
#endif

#ifndef FABLE_CEAXCONTEXT_H
#define FABLE_CEAXCONTEXT_H
// Layout: context vtable 012b9340, derived from root 012b5868. Allocation at
// 00c4e307 is 0x4f0 bytes; constructor 00c604e0 initializes this complete span.
// Class/member names are descriptive; native EAX protocol roles are documented.
struct SEaxEvent {
 EaxULong m_flags,m_type,m_environment;
 unsigned char Opaque0c[0x88];
 SEaxEvent* m_pNext;
};
class CEaxSource;
// Layout: node vtable 012b9338; constructor 00c5ff50 has next+4 and source+8.
class CEaxSourceNode : public CPKObject {
public:
 virtual ~CEaxSourceNode();
 CEaxSourceNode* m_pNext;
 CEaxSource* m_pSource;
};
class CEaxContext : public CPKObject {
public:
 virtual ~CEaxContext();
 EaxLong m_primarySlot;
 CEaxSourceNode* m_pSources;
 unsigned char Opaque00c[0x30];
 float m_distanceFactor;
 unsigned char Opaque040[0x44];
 EAXFXSLOTPROPERTIES m_slots[4];
 EAXREVERBPROPERTIES m_reverb[4];
 EAXCONTEXTPROPERTIES m_context;
 EAXREVERBPROPERTIES m_defaultReverb;
 EAXREVERBPROPERTIES m_committed;
 float m_effectiveDistanceFactor;
 unsigned char Opaque3b4[8];
 EaxULong m_dirty;
 unsigned char Opaque3c0[4];
 SEaxEvent* m_pEvents;
 unsigned char Opaque3c8[0x0c];
 EaxLong m_lastError;
 float m_decayTimeBase[4],m_decayTimeSize[4],m_reflectionsBase[4],m_reflectionsSize[4];
 float m_reflectionsDelayBase[4],m_reflectionsDelaySize[4],m_reverbBase[4],m_reverbSize[4];
 float m_reverbDelayBase[4],m_reverbDelaySize[4],m_echoTimeBase[4],m_echoTimeSize[4];
 float m_modulationTimeBase[4],m_modulationTimeSize[4];
 unsigned char Opaque4b8[0x38];
 SEaxEvent* QueueEvent_00c5ffb0(EaxULong,EaxULong,const void*,EaxULong);
 void ClearEvents();
 EaxLong CommitDeferredSettings();
 EaxLong SetEffectProperty_00c68f90(EaxULong,EaxULong,const void*,EaxULong,SEaxEvent**);
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
class CEaxSource : public CPKObject {
public:
 virtual ~CEaxSource();
 EaxULong m_dirty;
 unsigned char Opaque008[4];
 bool m_activeReverb;
 unsigned char Opaque00d[7];
 CEaxContext* m_pContext;
 EAXSOURCEPROPERTIES m_current;
 EAXSOURCESENDPROPERTIES m_sends[4];
 EAXSOURCEOCCLUSIONSENDPROPERTIES m_occlusionSends[4];
 EAXSOURCEEXCLUSIONSENDPROPERTIES m_exclusionSends[4];
 EAXACTIVEFXSLOTS m_activeSlots;
 EAXSOURCEPROPERTIES m_defaultProperties,m_deferred;
 unsigned char Opaque250[0x120];
 EaxULong m_sourceID;
 unsigned char Opaque374[0x4c];
 EaxLong CommitDeferredSettings();
 EaxLong SetSourceProperty_00c68510(EaxULong,const void*,EaxULong,SEaxEvent**);
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

// 00c67f60 CEaxContext::CommitDeferredSettings
// Flags: /O2 /Oy /W3
// Leaf: ?CommitDeferredSettings@CEaxContext@@QAEJXZ
EaxLong CEaxContext::CommitDeferredSettings()
{
 if(m_dirty&0x02000000){
  m_effectiveDistanceFactor=m_context.flDistanceFactor*m_distanceFactor;
  QueueEvent_00c5ffb0(4,4,&m_effectiveDistanceFactor,0);
  m_dirty&=~0x02000000;
 }
 if(m_dirty&0x01000000){
  QueueEvent_00c5ffb0(0,EAXREVERB_ALLPARAMETERS,&m_committed,0);
  m_dirty&=0xfe000000;
 }
 if(m_dirty&0x00000001){
  QueueEvent_00c5ffb0(0,2,&m_committed.ulEnvironment,0);
  m_dirty&=~0x00000001;
 }
 if(m_dirty&0x00000002){
  QueueEvent_00c5ffb0(0,3,&m_committed.flEnvironmentSize,0);
  m_dirty&=~0x00000002;
 }
 if(m_dirty&0x00000004){
  QueueEvent_00c5ffb0(0,4,&m_committed.flEnvironmentDiffusion,0);
  m_dirty&=~0x00000004;
 }
 if(m_dirty&0x00000008){
  QueueEvent_00c5ffb0(0,5,&m_committed.lRoom,0);
  m_dirty&=~0x00000008;
 }
 if(m_dirty&0x00000010){
  QueueEvent_00c5ffb0(0,6,&m_committed.lRoomHF,0);
  m_dirty&=~0x00000010;
 }
 if(m_dirty&0x00000020){
  QueueEvent_00c5ffb0(0,7,&m_committed.lRoomLF,0);
  m_dirty&=~0x00000020;
 }
 if(m_dirty&0x00000040){
  QueueEvent_00c5ffb0(0,8,&m_committed.flDecayTime,0);
  m_dirty&=~0x00000040;
 }
 if(m_dirty&0x00000080){
  QueueEvent_00c5ffb0(0,9,&m_committed.flDecayHFRatio,0);
  m_dirty&=~0x00000080;
 }
 if(m_dirty&0x00000100){
  QueueEvent_00c5ffb0(0,10,&m_committed.flDecayLFRatio,0);
  m_dirty&=~0x00000100;
 }
 if(m_dirty&0x00000200){
  QueueEvent_00c5ffb0(0,11,&m_committed.lReflections,0);
  m_dirty&=~0x00000200;
 }
 if(m_dirty&0x00000400){
  QueueEvent_00c5ffb0(0,12,&m_committed.flReflectionsDelay,0);
  m_dirty&=~0x00000400;
 }
 if(m_dirty&0x00000800){
  QueueEvent_00c5ffb0(0,13,&m_committed.vReflectionsPan,0);
  m_dirty&=~0x00000800;
 }
 if(m_dirty&0x00001000){
  QueueEvent_00c5ffb0(0,14,&m_committed.lReverb,0);
  m_dirty&=~0x00001000;
 }
 if(m_dirty&0x00002000){
  QueueEvent_00c5ffb0(0,15,&m_committed.flReverbDelay,0);
  m_dirty&=~0x00002000;
 }
 if(m_dirty&0x00004000){
  QueueEvent_00c5ffb0(0,16,&m_committed.vReverbPan,0);
  m_dirty&=~0x00004000;
 }
 if(m_dirty&0x00008000){
  QueueEvent_00c5ffb0(0,17,&m_committed.flEchoTime,0);
  m_dirty&=~0x00008000;
 }
 if(m_dirty&0x00010000){
  QueueEvent_00c5ffb0(0,18,&m_committed.flEchoDepth,0);
  m_dirty&=~0x00010000;
 }
 if(m_dirty&0x00020000){
  QueueEvent_00c5ffb0(0,19,&m_committed.flModulationTime,0);
  m_dirty&=~0x00020000;
 }
 if(m_dirty&0x00040000){
  QueueEvent_00c5ffb0(0,20,&m_committed.flModulationDepth,0);
  m_dirty&=~0x00040000;
 }
 if(m_dirty&0x00080000){
  QueueEvent_00c5ffb0(0,21,&m_committed.flAirAbsorptionHF,0);
  m_dirty&=~0x00080000;
 }
 if(m_dirty&0x00100000){
  QueueEvent_00c5ffb0(0,22,&m_committed.flHFReference,0);
  m_dirty&=~0x00100000;
 }
 if(m_dirty&0x00200000){
  QueueEvent_00c5ffb0(0,23,&m_committed.flLFReference,0);
  m_dirty&=~0x00200000;
 }
 if(m_dirty&0x00400000){
  QueueEvent_00c5ffb0(0,24,&m_committed.flRoomRolloffFactor,0);
  m_dirty&=~0x00400000;
 }
 if(m_dirty&0x00800000){
  QueueEvent_00c5ffb0(0,25,&m_committed.ulFlags,0);
  m_dirty&=~0x00800000;
 }
 if(m_dirty&0x04000000){
  for(CEaxSourceNode* node=m_pSources;node;node=node->m_pNext){
   node->m_pSource->m_dirty|=0x400000;
   node->m_pSource->CommitDeferredSettings();
  }
  m_dirty&=~0x04000000;
 }
 return 0;
}

CPKObject::~CPKObject(){}
CEaxContext::~CEaxContext(){}
CEaxSource::~CEaxSource(){}
CEaxSourceNode::~CEaxSourceNode(){}
void* __cdecl CPKObject::operator new(size_t size){return malloc(size);}
void __cdecl CPKObject::operator delete(void* p){free(p);}
struct Notification {EaxULong category,property,id;const void* payload;};
static Notification Events[64];static unsigned int Count,SourceCount,FixtureCases;
static CEaxSource* Sources[8];static EaxULong InjectDirty;static SEaxEvent Event;
SEaxEvent* CEaxContext::QueueEvent_00c5ffb0(EaxULong category,EaxULong property,const void* payload,EaxULong id)
{assert(Count<64);Events[Count].category=category;Events[Count].property=property;Events[Count].id=id;Events[Count].payload=payload;++Count;if(InjectDirty){m_dirty|=InjectDirty;InjectDirty=0;}return &Event;}
static void ResetTest(){Count=SourceCount=0;InjectDirty=0;++FixtureCases;}
static void CheckLayout(CEaxContext& context,CEaxSource& source,CEaxSourceNode& node){
#ifdef _MSC_VER
 assert(sizeof(CEaxContext)==0x4f0&&sizeof(CEaxSource)==0x3c0&&sizeof(CEaxSourceNode)==12);
 assert((char*)&context.m_pSources-(char*)&context==8);
 assert((char*)&context.m_distanceFactor-(char*)&context==0x3c);
 assert((char*)&context.m_effectiveDistanceFactor-(char*)&context==0x3b0);
 assert((char*)&source.m_activeReverb-(char*)&source==0xc);
 assert((char*)&source.m_sourceID-(char*)&source==0x370);
 assert((char*)&node.m_pNext-(char*)&node==4&&(char*)&node.m_pSource-(char*)&node==8);
#else
 (void)context;(void)source;(void)node;
#endif
}
EaxLong CEaxSource::CommitDeferredSettings(){assert(m_dirty&0x400000);assert(SourceCount<8);Sources[SourceCount++]=this;m_dirty=0;return -17;}
int main(){CEaxContext context;CEaxSource source;CEaxSourceNode node;CheckLayout(context,source,node);context.m_pSources=0;
const void* fields[]={&context.m_committed.ulEnvironment,&context.m_committed.flEnvironmentSize,&context.m_committed.flEnvironmentDiffusion,&context.m_committed.lRoom,&context.m_committed.lRoomHF,&context.m_committed.lRoomLF,&context.m_committed.flDecayTime,&context.m_committed.flDecayHFRatio,&context.m_committed.flDecayLFRatio,&context.m_committed.lReflections,&context.m_committed.flReflectionsDelay,&context.m_committed.vReflectionsPan,&context.m_committed.lReverb,&context.m_committed.flReverbDelay,&context.m_committed.vReverbPan,&context.m_committed.flEchoTime,&context.m_committed.flEchoDepth,&context.m_committed.flModulationTime,&context.m_committed.flModulationDepth,&context.m_committed.flAirAbsorptionHF,&context.m_committed.flHFReference,&context.m_committed.flLFReference,&context.m_committed.flRoomRolloffFactor,&context.m_committed.ulFlags};
ResetTest();context.m_dirty=0;assert(context.CommitDeferredSettings()==0);assert(Count==0&&SourceCount==0);
for(unsigned int bit=0;bit<24;++bit){ResetTest();context.m_dirty=1UL<<bit;assert(context.CommitDeferredSettings()==0);assert(context.m_dirty==0&&Count==1&&SourceCount==0);assert(Events[0].category==0&&Events[0].property==bit+2&&Events[0].payload==fields[bit]&&Events[0].id==0);}
ResetTest();context.m_dirty=0x00ffffff;assert(context.CommitDeferredSettings()==0);assert(Count==24&&context.m_dirty==0);for(unsigned int i=0;i<24;++i)assert(Events[i].category==0&&Events[i].property==i+2&&Events[i].payload==fields[i]&&Events[i].id==0);
ResetTest();context.m_dirty=0x01000000;assert(context.CommitDeferredSettings()==0);assert(Count==1&&context.m_dirty==0);assert(Events[0].property==1&&Events[0].payload==&context.m_committed);
ResetTest();context.m_dirty=0x01ffffff;assert(context.CommitDeferredSettings()==0);assert(Count==1&&context.m_dirty==0);assert(Events[0].property==1&&Events[0].payload==&context.m_committed);
const float pairs[3][2]={{2.0f,0.5f},{0.0f,4.0f},{0.25f,8.0f}};
for(unsigned int i=0;i<3;++i){ResetTest();context.m_dirty=0x02000000;context.m_context.flDistanceFactor=pairs[i][0];context.m_distanceFactor=pairs[i][1];assert(context.CommitDeferredSettings()==0);assert(Count==1&&context.m_dirty==0);assert(Events[0].category==4&&Events[0].property==4&&Events[0].payload==&context.m_effectiveDistanceFactor&&Events[0].id==0);assert(context.m_effectiveDistanceFactor==pairs[i][0]*pairs[i][1]);}
ResetTest();context.m_dirty=0x03ffffff;assert(context.CommitDeferredSettings()==0);assert(Count==2&&context.m_dirty==0);assert(Events[0].category==4&&Events[1].category==0&&Events[1].property==1);
ResetTest();context.m_dirty=0xf8000000;assert(context.CommitDeferredSettings()==0);assert(Count==0&&context.m_dirty==0xf8000000);
ResetTest();context.m_dirty=1;InjectDirty=2;assert(context.CommitDeferredSettings()==0);assert(Count==2&&context.m_dirty==0);assert(Events[0].property==2&&Events[1].property==3);
ResetTest();context.m_dirty=0x01000000;InjectDirty=0x00400000;assert(context.CommitDeferredSettings()==0);assert(Count==1&&context.m_dirty==0);
CEaxSource source2,source3;CEaxSourceNode node2,node3;
node.m_pSource=&source;node.m_pNext=&node2;node2.m_pSource=&source2;node2.m_pNext=&node3;node3.m_pSource=&source3;node3.m_pNext=0;
for(unsigned int length=0;length<4;++length){ResetTest();context.m_pSources=length?&node:0;node.m_pNext=length>1?&node2:0;node2.m_pNext=length>2?&node3:0;source.m_dirty=source2.m_dirty=source3.m_dirty=8;context.m_dirty=0x04000000;assert(context.CommitDeferredSettings()==0);assert(Count==0&&SourceCount==length&&context.m_dirty==0);if(length)assert(Sources[0]==&source&&source.m_dirty==0);if(length>1)assert(Sources[1]==&source2&&source2.m_dirty==0);if(length>2)assert(Sources[2]==&source3&&source3.m_dirty==0);}
ResetTest();context.m_dirty=0xffffffff;context.m_pSources=&node;source.m_dirty=source2.m_dirty=source3.m_dirty=0;assert(context.CommitDeferredSettings()==0);assert(Count==2&&SourceCount==3&&context.m_dirty==0xf8000000);
assert(FixtureCases==40);puts("00c67f60_TEST PASS");return 0;}