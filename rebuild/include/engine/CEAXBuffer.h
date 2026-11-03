#ifndef FABLE_CEAXSOURCE_H
#define FABLE_CEAXSOURCE_H
#include "CEaxContext.h"
// Layout: source vtable 012b9344, root 012b5868. Allocation at 00c42eaa is
// 0x3c0 bytes. Constructor 00c61610 proves context+0x14 and remaining storage.
// Names outside the EAX protocol are descriptive; see NAMING_EVIDENCE.md.
class CEaxSource : public CPKObject {
public:
 virtual ~CEaxSource();
 unsigned long m_dirty;
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
 unsigned long m_sourceID;
 unsigned char Opaque374[0x4c];
 long CommitDeferredSettings();
 long SetSourceProperty_00c68510(unsigned long,const void*,unsigned long,SEaxEvent**);
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
};
#endif
