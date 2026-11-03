#ifndef FABLE_CEAXCONTEXT_H
#define FABLE_CEAXCONTEXT_H
#include "EAX40.h"
#include "CPKObject.h"
// Layout: context vtable 012b9340, derived from root 012b5868. Allocation at
// 00c4e307 is 0x4f0 bytes; constructor 00c604e0 initializes this complete span.
// Class/member names are descriptive; native EAX protocol roles are documented.
struct SEaxEvent {
 unsigned long m_flags,m_type,m_environment;
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
 long m_primarySlot;
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
 unsigned long m_dirty;
 unsigned char Opaque3c0[4];
 SEaxEvent* m_pEvents;
 unsigned char Opaque3c8[0x0c];
 long m_lastError;
 float m_decayTimeBase[4],m_decayTimeSize[4],m_reflectionsBase[4],m_reflectionsSize[4];
 float m_reflectionsDelayBase[4],m_reflectionsDelaySize[4],m_reverbBase[4],m_reverbSize[4];
 float m_reverbDelayBase[4],m_reverbDelaySize[4],m_echoTimeBase[4],m_echoTimeSize[4];
 float m_modulationTimeBase[4],m_modulationTimeSize[4];
 unsigned char Opaque4b8[0x38];
 SEaxEvent* QueueEvent_00c5ffb0(unsigned long,unsigned long,const void*,unsigned long);
 void ClearEvents();
 long CommitDeferredSettings();
 long SetEffectProperty_00c68f90(unsigned long,unsigned long,const void*,unsigned long,SEaxEvent**);
 long SetReverbAllParameters(unsigned long,const EAXREVERBPROPERTIES*); // 00c68d70
 long SetEnvironment(unsigned long,unsigned long); // 00c68a50
 long SetEnvironmentSize(unsigned long,float); // 00c677c0
 long SetEnvironmentDiffusion(unsigned long,float); // 00c631e0
 long SetRoom(unsigned long,long); // 00c63270
 long SetRoomHF(unsigned long,long); // 00c632e0
 long SetRoomLF(unsigned long,long); // 00c63350
 long SetDecayTime(unsigned long,float,bool,bool); // 00c633c0
 long SetDecayHFRatio(unsigned long,float); // 00c63480
 long SetDecayLFRatio(unsigned long,float); // 00c63520
 long SetReflections(unsigned long,long,bool,bool); // 00c635c0
 long SetReflectionsDelay(unsigned long,float,bool,bool); // 00c63690
 long SetReflectionsPan(unsigned long,EAXVECTOR); // 00c63750
 long SetReverb(unsigned long,long,bool,bool); // 00c63810
 long SetReverbDelay(unsigned long,float,bool,bool); // 00c638e0
 long SetReverbPan(unsigned long,EAXVECTOR); // 00c639a0
 long SetEchoTime(unsigned long,float,bool,bool); // 00c63a60
 long SetEchoDepth(unsigned long,float); // 00c63b20
 long SetModulationTime(unsigned long,float,bool,bool); // 00c63bc0
 long SetModulationDepth(unsigned long,float); // 00c63c80
 long SetSlotAirAbsorptionHF(unsigned long,float); // 00c63d20
 long SetSlotHFReference(unsigned long,float); // 00c63db0
 long SetLFReference(unsigned long,float); // 00c63e50
 long SetRoomRolloffFactor(unsigned long,float); // 00c63ef0
 long SetReverbFlags(unsigned long,unsigned long); // 00c67a10
 long SetSlotProperty(unsigned long,const EAXFXSLOTPROPERTIES&); // 00c689b0
 long SetSlotLoadEffect(unsigned long,GUID); // 00c674b0
 long SetSlotVolume(unsigned long,long); // 00c67720
 long SetSlotLock(unsigned long,long); // 00c67470
 long SetSlotFlags(unsigned long,unsigned long); // 00c67430
};
#endif
