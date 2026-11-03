#ifndef FABLE_CEAXBUFFER_H
#define FABLE_CEAXBUFFER_H
#include "CEAXListener.h"
// GameCODA 2.10 EAX translation buffer (owner decision 2026-11-03 D3; pass 11 spelled it CEaxSource). Class, member and access
// names: the original EAXBuffer.obj / EAX4Translation.obj debug types (Codex pass 12 SDK_COMPARISON). Native layout: allocation
// 0x00C42EAA = 0x3c0 bytes, ctor 0x00C61610, vtable 0x012B9344; the later SDK's EAX1/2 state is absent.
// Offsets proven by both models (pass 11 descriptive name in brackets): m_dwEAX3Changes +0x004 [m_dirty], m_bEAX4Active +0x00c
// [m_activeReverb], m_lpEAXListener +0x014 [m_pContext], m_Source.eax4BP +0x018 [m_current], m_Source.eax4BPSend +0x060
// [m_sends], m_Source.eax4BPOcclusionSend +0x0c0, m_Source.eax4BPExclusionSend +0x140, m_Source.eax4BPActiveFXSlotID +0x1a0
// [m_activeSlots], m_Source.eax3BP +0x1c0 [m_defaultProperties], m_Dest.eax3BP +0x208 [m_deferred], m_BufferID +0x370 [m_sourceID].
class CEAXBuffer:public CCodaObjectBase {
public:
 virtual ~CEAXBuffer();
 int GetEAX4Source(unsigned long,void*,unsigned long,unsigned long*); // 0x00C66C90
 int CommitEAX3(); // 0x00C65A00
 // No SDK record yet for the rows below: pass 11 descriptive method names, class respelled.
 long SetSourceProperty_00c68510(unsigned long,const void*,unsigned long,_CHANGES**);
 long SetAllParameters(const EAXSOURCEPROPERTIES*); // 00c67c00
 long SetObstructionParameters(const EAXOBSTRUCTIONPROPERTIES*); // 00c647b0
 long SetOcclusionParameters(const EAXOCCLUSIONPROPERTIES*); // 00c64840
 long SetExclusionParameters(const EAXEXCLUSIONPROPERTIES*); // 00c64940
 long SetDirect(long); // 00c649c0
 long SetDirectHF(long); // 00c64a00
 long SetRoom(long); // 00c64a40
 long SetRoomHF(long); // 00c64a80
 long SetObstruction(long); // 00c64ac0
 long SetObstructionLFRatio(float); // 00c64b00
 long SetOcclusion(long); // 00c64b70
 long SetOcclusionLFRatio(float); // 00c64bb0
 long SetOcclusionRoomRatio(float); // 00c64c10
 long SetOcclusionDirectRatio(float); // 00c64c70
 long SetExclusion(long); // 00c64cd0
 long SetExclusionLFRatio(float); // 00c64d10
 long SetOutsideVolumeHF(long); // 00c64d70
 long SetDopplerFactor(float); // 00c64db0
 long SetRolloffFactor(float); // 00c64e20
 long SetRoomRolloffFactor(float); // 00c64e90
 long SetAirAbsorptionFactor(float); // 00c64f00
 long SetFlags(unsigned long); // 00c64f70
 long SetSendParameters(const EAXSOURCESENDPROPERTIES*,unsigned long); // 00c64fb0
 long SetAllSendParameters(const EAXSOURCEALLSENDPROPERTIES*,unsigned long); // 00c65170
 long SetOcclusionSendParameters(const EAXSOURCEOCCLUSIONSENDPROPERTIES*,unsigned long); // 00c65450
 long SetExclusionSendParameters(const EAXSOURCEEXCLUSIONSENDPROPERTIES*,unsigned long); // 00c65690
 long SetActiveFXSlotID(const GUID*,unsigned long); // 00c65870
 unsigned long m_dwEAX3Changes,m_dwEAX3Changes_Defer;
 bool m_bEAX4Active;
 EAXVERSION m_EAXVersion;
private:
 int Set3DBuffer(unsigned long,void*,unsigned long); // 0x00C60FF0
 CEAXListener* m_lpEAXListener;
 SOURCEBP m_Source;
 DESTINATIONBP m_Dest;
 BUFFER m_Buffer,m_Buffer_Defer;
 unsigned long m_dwFlags,m_dwFlags_Defer;
 long m_BufferID;
 // Remaining native 0x4c storage differs from the later SDK, and is untouched by the landed rows.
 unsigned char Opaque374[0x4c];
};
#endif
