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
class CEAXBuffer;
// Layout: node vtable 012b9338; constructor 00c5ff50 has next+4 and source+8.
class CEAXBufferList : public CCodaObjectBase {
public:
 virtual ~CEAXBufferList();
 CEAXBufferList* m_pNext;
 CEAXBuffer* m_pSource;
};
class CEAXListener : public CCodaObjectBase {
public:
 virtual ~CEAXListener();
 EaxLong m_primarySlot;
 CEAXBufferList* m_pSources;
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
 _CHANGES* m_pEvents;
 unsigned char Opaque3c8[0x0c];
 EaxLong m_lastError;
 float m_decayTimeBase[4],m_decayTimeSize[4],m_reflectionsBase[4],m_reflectionsSize[4];
 float m_reflectionsDelayBase[4],m_reflectionsDelaySize[4],m_reverbBase[4],m_reverbSize[4];
 float m_reverbDelayBase[4],m_reverbDelaySize[4],m_echoTimeBase[4],m_echoTimeSize[4];
 float m_modulationTimeBase[4],m_modulationTimeSize[4];
 unsigned char Opaque4b8[0x38];
 _CHANGES* QueueEvent_00c5ffb0(EaxULong,EaxULong,const void*,EaxULong);
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
 unsigned char Opaque008[4];
 bool m_activeReverb;
 unsigned char Opaque00d[7];
 CEAXListener* m_pContext;
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

// 00c65a00 CEAXBuffer::CommitDeferredSettings
// Flags: /O2 /Oy /W3
// Leaf: ?CommitDeferredSettings@CEAXBuffer@@QAEJXZ
#include <string.h>
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
EaxLong CEAXBuffer::CommitDeferredSettings()
{
 if(m_dirty&0x400000){
  CEAXListener* context=m_pContext;
  if(memcmp(&m_activeSlots.guidActiveFXSlots[0],&context->m_context.guidPrimaryFXSlotID,sizeof(GUID))==0 ||
     memcmp(&m_activeSlots.guidActiveFXSlots[0],&EAX_PrimaryFXSlotID,sizeof(GUID))==0 ||
     memcmp(&m_activeSlots.guidActiveFXSlots[1],&context->m_context.guidPrimaryFXSlotID,sizeof(GUID))==0 ||
     memcmp(&m_activeSlots.guidActiveFXSlots[1],&EAX_PrimaryFXSlotID,sizeof(GUID))==0)
   m_activeReverb=true;
  else m_activeReverb=false;
  if(m_activeReverb&&context->m_primarySlot!=-1&&(context->m_slots[context->m_primarySlot].ulFlags&1)){
   m_deferred.lRoom=max(m_current.lRoom+m_sends[context->m_primarySlot].lSend,-10000);
   m_deferred.lRoomHF=max(m_sends[context->m_primarySlot].lSendHF+m_current.lRoomHF,-10000);
   EaxLong occlusion=m_current.lOcclusion;
   if(occlusion==0){
    m_deferred.lOcclusion=m_occlusionSends[context->m_primarySlot].lOcclusion;
    m_deferred.flOcclusionLFRatio=m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio;
    m_deferred.flOcclusionRoomRatio=m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio;
    m_deferred.flOcclusionDirectRatio=m_occlusionSends[context->m_primarySlot].flOcclusionDirectRatio;
   }else if(m_occlusionSends[context->m_primarySlot].lOcclusion==0){
    m_deferred.lOcclusion=occlusion;
    m_deferred.flOcclusionLFRatio=m_current.flOcclusionLFRatio;
    m_deferred.flOcclusionRoomRatio=m_current.flOcclusionRoomRatio;
    m_deferred.flOcclusionDirectRatio=m_current.flOcclusionDirectRatio;
   }else{
    EaxLong total=max(m_occlusionSends[context->m_primarySlot].lOcclusion+occlusion,-10000);
    m_deferred.lOcclusion=total;
    float direct=min(10.0f,(occlusion*m_current.flOcclusionDirectRatio+m_occlusionSends[context->m_primarySlot].lOcclusion*m_occlusionSends[context->m_primarySlot].flOcclusionDirectRatio)/total);
    m_deferred.flOcclusionDirectRatio=direct;
    float room=min(10.0f,(occlusion*m_current.flOcclusionRoomRatio+m_occlusionSends[context->m_primarySlot].lOcclusion*m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio)/total);
    m_deferred.flOcclusionRoomRatio=room;
    float currentDirect=max(m_current.flOcclusionLFRatio*m_current.flOcclusionDirectRatio,m_current.flOcclusionLFRatio+m_current.flOcclusionDirectRatio-1.0f);
    float sendDirect=max(m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio*m_occlusionSends[context->m_primarySlot].flOcclusionDirectRatio,m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio+m_occlusionSends[context->m_primarySlot].flOcclusionDirectRatio-1.0f);
    float combinedDirect=occlusion*currentDirect;
    combinedDirect+=m_occlusionSends[context->m_primarySlot].lOcclusion*sendDirect;
    combinedDirect/=total;
    float currentRoom=max(m_current.flOcclusionLFRatio*m_current.flOcclusionRoomRatio,m_current.flOcclusionLFRatio+m_current.flOcclusionRoomRatio-1.0f);
    const CEAXBuffer& source=*this;
    float sendRoom=(source.m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio*source.m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio > source.m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio+source.m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio-1.0f) ? m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio*m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio : m_occlusionSends[context->m_primarySlot].flOcclusionRoomRatio+m_occlusionSends[context->m_primarySlot].flOcclusionLFRatio-1.0f;
    float combinedRoom=(occlusion*currentRoom+m_occlusionSends[context->m_primarySlot].lOcclusion*sendRoom)/total;
    if(direct!=0.0f&&room!=0.0f){
     m_deferred.flOcclusionLFRatio=min(1.0f,combinedDirect/direct);
     m_deferred.flOcclusionLFRatio=min(m_deferred.flOcclusionLFRatio,combinedDirect+1.0f-direct);
     m_deferred.flOcclusionLFRatio=min(m_deferred.flOcclusionLFRatio,combinedRoom/room);
     m_deferred.flOcclusionLFRatio=min(m_deferred.flOcclusionLFRatio,combinedRoom+1.0f-room);
     m_deferred.flOcclusionLFRatio=max(m_deferred.flOcclusionLFRatio,0.0f);
    }else m_deferred.flOcclusionLFRatio=0.0f;
   }
   EaxLong exclusion=m_current.lExclusion;
   if(exclusion==0){
    m_deferred.lExclusion=m_exclusionSends[context->m_primarySlot].lExclusion;
    m_deferred.flExclusionLFRatio=m_exclusionSends[context->m_primarySlot].flExclusionLFRatio;
   }else if(m_exclusionSends[context->m_primarySlot].lExclusion==0){
    m_deferred.lExclusion=exclusion;
    m_deferred.flExclusionLFRatio=m_current.flExclusionLFRatio;
   }else{
    EaxLong total=max(m_exclusionSends[context->m_primarySlot].lExclusion+exclusion,-10000);
    m_deferred.lExclusion=total;
    m_deferred.flExclusionLFRatio=min(1.0f,(exclusion*m_current.flExclusionLFRatio+m_exclusionSends[context->m_primarySlot].lExclusion*m_exclusionSends[context->m_primarySlot].flExclusionLFRatio)/total);
   }
  }else{
   m_deferred.lRoom=-10000;
   m_deferred.lRoomHF=m_current.lRoomHF;
   m_deferred.lOcclusion=m_current.lOcclusion;
   m_deferred.flOcclusionLFRatio=m_current.flOcclusionLFRatio;
   m_deferred.flOcclusionRoomRatio=m_current.flOcclusionRoomRatio;
   m_deferred.flOcclusionDirectRatio=m_current.flOcclusionDirectRatio;
   m_deferred.lExclusion=m_current.lExclusion;
   m_deferred.flExclusionLFRatio=m_current.flExclusionLFRatio;
  }
  m_dirty=0x200000;
 }
 if(m_dirty&0x200000){
  m_pContext->QueueEvent_00c5ffb0(1,EAXSOURCE_ALLPARAMETERS,&m_deferred,m_sourceID);
  m_dirty=0;
 }
 if(m_dirty&0x00000008){
  m_pContext->QueueEvent_00c5ffb0(1,5,&m_deferred.lDirect,m_sourceID);
  m_dirty&=~0x00000008;
 }
 if(m_dirty&0x00000010){
  m_pContext->QueueEvent_00c5ffb0(1,6,&m_deferred.lDirectHF,m_sourceID);
  m_dirty&=~0x00000010;
 }
 if(m_dirty&0x00000020){
  m_pContext->QueueEvent_00c5ffb0(1,7,&m_deferred.lRoom,m_sourceID);
  m_dirty&=~0x00000020;
 }
 if(m_dirty&0x00000040){
  m_pContext->QueueEvent_00c5ffb0(1,8,&m_deferred.lRoomHF,m_sourceID);
  m_dirty&=~0x00000040;
 }
 if(m_dirty&0x00000080){
  m_pContext->QueueEvent_00c5ffb0(1,9,&m_deferred.lObstruction,m_sourceID);
  m_dirty&=~0x00000080;
 }
 if(m_dirty&0x00000100){
  m_pContext->QueueEvent_00c5ffb0(1,10,&m_deferred.flObstructionLFRatio,m_sourceID);
  m_dirty&=~0x00000100;
 }
 if(m_dirty&0x00000200){
  m_pContext->QueueEvent_00c5ffb0(1,11,&m_deferred.lOcclusion,m_sourceID);
  m_dirty&=~0x00000200;
 }
 if(m_dirty&0x00000400){
  m_pContext->QueueEvent_00c5ffb0(1,12,&m_deferred.flOcclusionLFRatio,m_sourceID);
  m_dirty&=~0x00000400;
 }
 if(m_dirty&0x00000800){
  m_pContext->QueueEvent_00c5ffb0(1,13,&m_deferred.flOcclusionRoomRatio,m_sourceID);
  m_dirty&=~0x00000800;
 }
 if(m_dirty&0x00001000){
  m_pContext->QueueEvent_00c5ffb0(1,14,&m_deferred.flOcclusionDirectRatio,m_sourceID);
  m_dirty&=~0x00001000;
 }
 if(m_dirty&0x00002000){
  m_pContext->QueueEvent_00c5ffb0(1,15,&m_deferred.lExclusion,m_sourceID);
  m_dirty&=~0x00002000;
 }
 if(m_dirty&0x00004000){
  m_pContext->QueueEvent_00c5ffb0(1,16,&m_deferred.flExclusionLFRatio,m_sourceID);
  m_dirty&=~0x00004000;
 }
 if(m_dirty&0x00008000){
  m_pContext->QueueEvent_00c5ffb0(1,17,&m_deferred.lOutsideVolumeHF,m_sourceID);
  m_dirty&=~0x00008000;
 }
 if(m_dirty&0x00010000){
  m_pContext->QueueEvent_00c5ffb0(1,18,&m_deferred.flDopplerFactor,m_sourceID);
  m_dirty&=~0x00010000;
 }
 if(m_dirty&0x00020000){
  m_pContext->QueueEvent_00c5ffb0(1,19,&m_deferred.flRolloffFactor,m_sourceID);
  m_dirty&=~0x00020000;
 }
 if(m_dirty&0x00040000){
  m_pContext->QueueEvent_00c5ffb0(1,20,&m_deferred.flRoomRolloffFactor,m_sourceID);
  m_dirty&=~0x00040000;
 }
 if(m_dirty&0x00080000){
  m_pContext->QueueEvent_00c5ffb0(1,21,&m_deferred.flAirAbsorptionFactor,m_sourceID);
  m_dirty&=~0x00080000;
 }
 if(m_dirty&0x00100000){
  m_pContext->QueueEvent_00c5ffb0(1,22,&m_deferred.ulFlags,m_sourceID);
  m_dirty&=~0x00100000;
 }
 if(m_dirty&0x00000001){
  m_pContext->QueueEvent_00c5ffb0(1,2,&m_deferred.lObstruction,m_sourceID);
  m_dirty&=~0x00000001;
 }
 if(m_dirty&0x00000002){
  m_pContext->QueueEvent_00c5ffb0(1,3,&m_deferred.lOcclusion,m_sourceID);
  m_dirty&=~0x00000002;
 }
 if(m_dirty&0x00000004){
  m_pContext->QueueEvent_00c5ffb0(1,4,&m_deferred.lExclusion,m_sourceID);
  m_dirty&=~0x00000004;
 }
 return 0;
}

CCodaObjectBase::~CCodaObjectBase(){}
CEAXListener::~CEAXListener(){}
CEAXBuffer::~CEAXBuffer(){}
CEAXBufferList::~CEAXBufferList(){}
void* __cdecl CCodaObjectBase::operator new(size_t size){return malloc(size);}
void __cdecl CCodaObjectBase::operator delete(void* p){free(p);}
struct Notification {EaxULong category,property,id;const void* payload;};
static Notification Events[64];static unsigned int Count,SourceCount,FixtureCases;
static CEAXBuffer* Sources[8];static EaxULong InjectDirty;static _CHANGES Event;
_CHANGES* CEAXListener::QueueEvent_00c5ffb0(EaxULong category,EaxULong property,const void* payload,EaxULong id)
{assert(Count<64);Events[Count].category=category;Events[Count].property=property;Events[Count].id=id;Events[Count].payload=payload;++Count;if(InjectDirty){m_dirty|=InjectDirty;InjectDirty=0;}return &Event;}
static void ResetTest(){Count=SourceCount=0;InjectDirty=0;++FixtureCases;}
static void CheckLayout(CEAXListener& context,CEAXBuffer& source,CEAXBufferList& node){
#ifdef _MSC_VER
 assert(sizeof(CEAXListener)==0x4f0&&sizeof(CEAXBuffer)==0x3c0&&sizeof(CEAXBufferList)==12);
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

extern "C" const GUID EAX_PrimaryFXSlotID={0xf317866d,0x924c,0x450c,{0x86,0x1b,0xe6,0xda,0xa2,0x5e,0x7c,0x20}};
static float Upper(float a,float b){return a<b?a:b;}
static float Lower(float a,float b){return a>b?a:b;}
static EaxLong Level(EaxLong a,EaxLong b){EaxLong sum=a+b;return sum>-10000?sum:-10000;}
static void Near(float a,float b){assert(a-b<0.000244140625f&&b-a<0.000244140625f);}
static void Initialize(CEAXListener& context,CEAXBuffer& source){
 memset(&context.m_context,0,sizeof(context.m_context));context.m_context.guidPrimaryFXSlotID.Data1=42;
 memset(context.m_slots,0,sizeof(context.m_slots));context.m_primarySlot=2;context.m_slots[2].ulFlags=1;
 memset(&source.m_current,0,sizeof(source.m_current));memset(source.m_sends,0,sizeof(source.m_sends));memset(source.m_occlusionSends,0,sizeof(source.m_occlusionSends));memset(source.m_exclusionSends,0,sizeof(source.m_exclusionSends));memset(&source.m_deferred,0,sizeof(source.m_deferred));
 memset(&source.m_activeSlots,0,sizeof(source.m_activeSlots));source.m_activeSlots.guidActiveFXSlots[0]=EAX_PrimaryFXSlotID;
 source.m_pContext=&context;source.m_dirty=0;source.m_sourceID=0x12345678;source.m_activeReverb=false;
 source.m_current.lRoom=-16;source.m_current.lRoomHF=-8;source.m_sends[2].lSend=-32;source.m_sends[2].lSendHF=-4;
}
static void CheckMath(const CEAXBuffer& source,const EAXSOURCEPROPERTIES& original,const EAXSOURCEOCCLUSIONSENDPROPERTIES& send,const EAXSOURCEEXCLUSIONSENDPROPERTIES& exclusion){
 const EAXSOURCEPROPERTIES& result=source.m_deferred;
 if(original.lOcclusion==0){assert(result.lOcclusion==send.lOcclusion);Near(result.flOcclusionLFRatio,send.flOcclusionLFRatio);Near(result.flOcclusionRoomRatio,send.flOcclusionRoomRatio);Near(result.flOcclusionDirectRatio,send.flOcclusionDirectRatio);}
 else if(send.lOcclusion==0){assert(result.lOcclusion==original.lOcclusion);Near(result.flOcclusionLFRatio,original.flOcclusionLFRatio);Near(result.flOcclusionRoomRatio,original.flOcclusionRoomRatio);Near(result.flOcclusionDirectRatio,original.flOcclusionDirectRatio);}
 else{
 const EaxLong total=Level(original.lOcclusion,send.lOcclusion);
 const float weight=(float)original.lOcclusion/(float)total;
 const float weightSend=(float)send.lOcclusion/(float)total;
 const float direct=Upper(10.0f,weight*original.flOcclusionDirectRatio+weightSend*send.flOcclusionDirectRatio);
 const float room=Upper(10.0f,weight*original.flOcclusionRoomRatio+weightSend*send.flOcclusionRoomRatio);
 const float directLF=weight*Lower(original.flOcclusionLFRatio*original.flOcclusionDirectRatio,original.flOcclusionLFRatio+original.flOcclusionDirectRatio-1.0f)+weightSend*Lower(send.flOcclusionLFRatio*send.flOcclusionDirectRatio,send.flOcclusionLFRatio+send.flOcclusionDirectRatio-1.0f);
 const float roomLF=weight*Lower(original.flOcclusionLFRatio*original.flOcclusionRoomRatio,original.flOcclusionLFRatio+original.flOcclusionRoomRatio-1.0f)+weightSend*Lower(send.flOcclusionLFRatio*send.flOcclusionRoomRatio,send.flOcclusionLFRatio+send.flOcclusionRoomRatio-1.0f);
 float low=0.0f;
 if(direct!=0.0f&&room!=0.0f){float constraints[5]={1.0f,directLF/direct,directLF+1.0f-direct,roomLF/room,roomLF+1.0f-room};low=constraints[0];for(int i=1;i<5;++i)low=Upper(low,constraints[i]);low=Lower(0.0f,low);}
 assert(result.lOcclusion==total);Near(result.flOcclusionLFRatio,low);Near(result.flOcclusionRoomRatio,room);Near(result.flOcclusionDirectRatio,direct);
 }
 if(original.lExclusion==0){assert(result.lExclusion==exclusion.lExclusion);Near(result.flExclusionLFRatio,exclusion.flExclusionLFRatio);}
 else if(exclusion.lExclusion==0){assert(result.lExclusion==original.lExclusion);Near(result.flExclusionLFRatio,original.flExclusionLFRatio);}
 else{const EaxLong total=Level(original.lExclusion,exclusion.lExclusion);assert(result.lExclusion==total);Near(result.flExclusionLFRatio,Upper(1.0f,((float)original.lExclusion*original.flExclusionLFRatio+(float)exclusion.lExclusion*exclusion.flExclusionLFRatio)/(float)total));}
}
int main(){CEAXListener context;CEAXBuffer source;CEAXBufferList node;CheckLayout(context,source,node);Initialize(context,source);
const void* fields[]={&source.m_deferred.lDirect,&source.m_deferred.lDirectHF,&source.m_deferred.lRoom,&source.m_deferred.lRoomHF,&source.m_deferred.lObstruction,&source.m_deferred.flObstructionLFRatio,&source.m_deferred.lOcclusion,&source.m_deferred.flOcclusionLFRatio,&source.m_deferred.flOcclusionRoomRatio,&source.m_deferred.flOcclusionDirectRatio,&source.m_deferred.lExclusion,&source.m_deferred.flExclusionLFRatio,&source.m_deferred.lOutsideVolumeHF,&source.m_deferred.flDopplerFactor,&source.m_deferred.flRolloffFactor,&source.m_deferred.flRoomRolloffFactor,&source.m_deferred.flAirAbsorptionFactor,&source.m_deferred.ulFlags};
ResetTest();assert(source.CommitDeferredSettings()==0);assert(Count==0);
for(unsigned int bit=3;bit<21;++bit){ResetTest();source.m_dirty=1UL<<bit;assert(source.CommitDeferredSettings()==0);assert(Count==1&&source.m_dirty==0);assert(Events[0].category==1&&Events[0].property==bit+2&&Events[0].payload==fields[bit-3]&&Events[0].id==source.m_sourceID);}
const unsigned int aggregates[3]={4,6,10};
for(unsigned int bit=0;bit<3;++bit){ResetTest();source.m_dirty=1UL<<bit;assert(source.CommitDeferredSettings()==0);assert(Count==1&&source.m_dirty==0);assert(Events[0].category==1&&Events[0].property==bit+2&&Events[0].payload==fields[aggregates[bit]]&&Events[0].id==source.m_sourceID);}
ResetTest();source.m_dirty=0x1fffff;assert(source.CommitDeferredSettings()==0);assert(Count==21&&source.m_dirty==0);for(unsigned int i=0;i<21;++i)assert(Events[i].category==1&&Events[i].property==(i<18?i+5:i-16)&&Events[i].id==source.m_sourceID);
ResetTest();source.m_dirty=0x3fffff;assert(source.CommitDeferredSettings()==0);assert(Count==1&&source.m_dirty==0);assert(Events[0].property==1&&Events[0].payload==&source.m_deferred);
ResetTest();source.m_dirty=0xff800000;assert(source.CommitDeferredSettings()==0);assert(Count==0&&source.m_dirty==0xff800000);
for(unsigned int identity=0;identity<5;++identity){ResetTest();Initialize(context,source);memset(&source.m_activeSlots,0,sizeof(source.m_activeSlots));if(identity<4)source.m_activeSlots.guidActiveFXSlots[identity/2]=(identity&1)?EAX_PrimaryFXSlotID:context.m_context.guidPrimaryFXSlotID;source.m_dirty=0x400000;assert(source.CommitDeferredSettings()==0);assert(source.m_activeReverb==(identity<4));assert(source.m_deferred.lRoom==(identity<4?-48:-10000));assert(Count==1&&source.m_dirty==0);assert(Events[0].property==1&&Events[0].payload==&source.m_deferred);}
for(int disabled=0;disabled<2;++disabled){ResetTest();Initialize(context,source);if(disabled)context.m_slots[2].ulFlags=0;else context.m_primarySlot=-1;source.m_current.lOcclusion=-32;source.m_current.flOcclusionLFRatio=0.5f;source.m_current.flOcclusionRoomRatio=2.0f;source.m_current.flOcclusionDirectRatio=0.25f;source.m_current.lExclusion=-16;source.m_current.flExclusionLFRatio=0.125f;source.m_dirty=0x400000;assert(source.CommitDeferredSettings()==0);assert(source.m_deferred.lRoom==-10000&&source.m_deferred.lRoomHF==-8);assert(source.m_deferred.lOcclusion==-32&&source.m_deferred.flOcclusionLFRatio==0.5f&&source.m_deferred.flOcclusionRoomRatio==2.0f&&source.m_deferred.flOcclusionDirectRatio==0.25f);assert(source.m_deferred.lExclusion==-16&&source.m_deferred.flExclusionLFRatio==0.125f);}
const EaxLong levels[4][2]={{0,-16},{-16,0},{-16,-16},{-8192,-8192}};
const float ratios[8][6]={{0.5f,0.5f,0.5f,0.5f,0.5f,0.5f},{0.25f,2.0f,4.0f,0.75f,4.0f,2.0f},{1.0f,0.0f,2.0f,0.5f,0.0f,4.0f},{0.5f,2.0f,0.0f,0.25f,4.0f,0.0f},{0.0f,0.5f,0.5f,0.0f,0.5f,0.5f},{1.0f,10.0f,10.0f,1.0f,10.0f,10.0f},{0.5f,1.0f,1.0f,0.25f,1.0f,1.0f},{0.125f,0.25f,0.5f,0.75f,0.5f,0.25f}};
for(unsigned int level=0;level<4;++level)for(unsigned int ratio=0;ratio<8;++ratio){ResetTest();Initialize(context,source);EAXSOURCEPROPERTIES& p=source.m_current;EAXSOURCEOCCLUSIONSENDPROPERTIES& s=source.m_occlusionSends[2];EAXSOURCEEXCLUSIONSENDPROPERTIES& e=source.m_exclusionSends[2];p.lOcclusion=levels[level][0];s.lOcclusion=levels[level][1];p.flOcclusionLFRatio=ratios[ratio][0];p.flOcclusionDirectRatio=ratios[ratio][1];p.flOcclusionRoomRatio=ratios[ratio][2];s.flOcclusionLFRatio=ratios[ratio][3];s.flOcclusionDirectRatio=ratios[ratio][4];s.flOcclusionRoomRatio=ratios[ratio][5];p.lExclusion=levels[level][0];e.lExclusion=levels[level][1];p.flExclusionLFRatio=0.5f;e.flExclusionLFRatio=0.25f;source.m_dirty=0xffffffff;assert(source.CommitDeferredSettings()==0);assert(source.m_dirty==0&&Count==1&&Events[0].category==1&&Events[0].property==1&&Events[0].payload==&source.m_deferred&&Events[0].id==source.m_sourceID);CheckMath(source,p,s,e);assert(source.m_deferred.lRoom==-48&&source.m_deferred.lRoomHF==-12);}
ResetTest();Initialize(context,source);source.m_current.lRoom=source.m_current.lRoomHF=-8192;source.m_sends[2].lSend=source.m_sends[2].lSendHF=-8192;source.m_dirty=0x400000;assert(source.CommitDeferredSettings()==0);assert(source.m_deferred.lRoom==-10000&&source.m_deferred.lRoomHF==-10000);
assert(FixtureCases==65);puts("00c65a00_TEST PASS");return 0;}