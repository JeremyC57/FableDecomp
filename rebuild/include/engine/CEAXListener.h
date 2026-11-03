#ifndef FABLE_CEAXLISTENER_H
#define FABLE_CEAXLISTENER_H
#include "EAXNativeTypes.h"
#include "CCodaObjectBase.h"
// GameCODA 2.10 EAX translation listener (owner decision 2026-11-03 D3; pass 11 spelled it CEaxContext). Class, member and
// access names: the original EAXListener.obj / EAX4Translation.obj debug types (Codex pass 12 SDK_COMPARISON). Native layout:
// allocation 0x00C4E307 = 0x4f0 bytes, ctor 0x00C604E0, vtable 0x012B9340; the later SDK's EAX1/2 conversion state is absent.
// Offsets proven by both models (pass 11 descriptive name in brackets): m_ListenerSlotID +0x004 [m_primarySlot],
// m_lpBufferList +0x008 [m_pSources], m_Listener.flDistanceFactor +0x03c [m_distanceFactor], m_Source.eax4FXSlot +0x084
// [m_slots], m_Source.eax4LPSlot +0x0f4 [m_reverb], m_Source.eax4Context +0x2b4 [m_context], m_Source.eax3LP +0x2d0
// [m_defaultReverb], m_Dest.eax3LP +0x340 [m_committed], m_Dest.flDistanceFactor +0x3b0 [m_effectiveDistanceFactor],
// m_dwEAX3Changes +0x3bc [m_dirty], lpChanges +0x3c4 [m_pEvents], m_lEAX4ErrorCode +0x3d4 [m_lastError],
// m_flEAX4Unsized* +0x3d8..0x4b7 [m_decayTimeBase..m_modulationTimeSize], m_flEAX3Unsized* +0x4b8..0x4ef.
class CEAXBuffer;
// Layout: vtable 0x012B9338; ctor 0x00C5FF50 (pass 11 CEaxSourceNode): next +4, buffer +8.
class CEAXBufferList:public CCodaObjectBase {
public:
 CEAXBufferList(); // 0x00C5FF50
 virtual ~CEAXBufferList();
 CEAXBufferList* lpNext;
 CEAXBuffer* lpEAXBuffer;
};
class CEAXListener:public CCodaObjectBase {
public:
 virtual ~CEAXListener();
 int GetEAX4FXSlot(unsigned long,unsigned long,void*,unsigned long,unsigned long*); // 0x00C66330
 void AddToChangesList(IDENTIFIER,unsigned long,void*,long); // 0x00C5FFB0
 void SetEAX4ErrorCode(long code){m_lEAX4ErrorCode=code;}
 // No SDK record yet for the rows below: pass 11 descriptive method names, class respelled.
 void ClearEvents(); // 00c60410
 long CommitDeferredSettings(); // 00c67f60
 long SetEffectProperty_00c68f90(unsigned long,unsigned long,const void*,unsigned long,_CHANGES**);
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
 unsigned long m_ListenerSlotID;
 CEAXBufferList* m_lpBufferList;
 LISTENER m_Listener,m_Listener_Defer;
 SOURCE m_Source;
 DESTINATION m_Dest;
 unsigned long m_dwFlags,m_dwFlags_Defer,m_dwEAX3Changes,m_dwEAX3Changes_Defer;
 _CHANGES* lpChanges;
 float m_flDecayHFFactor;
 bool m_bEnsoniq;
 EAXVERSION m_EAXVersion;
private:
 long m_lEAX4ErrorCode;
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
