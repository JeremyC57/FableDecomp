// CTCDivineWrath::PerCreatureInfo::PerCreatureInfo() -- initialize tracked pointers and the default lighting channel.
// Leaf: ??0PerCreatureInfo@CTCDivineWrath@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Identity: probable owner/function role; PDB unique24-byte PerCreatureInfo record has ActualCreature+0,FramesToGetHit+8,Effect+12,DefaultChannel+20; both PDB sets contain CTCDivineWrath nested ctor; owner allocation by position in own spell code region, no xrefs.
// Layout: generated outer headers have no nested record; outer declaration supplies naming scope only. Real eight-byte tracked pointers and four-byte channel retain Pad/FramesToGetHit uninitialized.
class CThing; class CThingCreatureBase;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CEnginePrimitiveThemeChannelDesc { public: unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad; CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0) {} };
class CTCDivineWrath { public: struct PerCreatureInfo { CIntelligentPointer<CThingCreatureBase> ActualCreature; long FramesToGetHit; CIntelligentPointer<CThing> Effect; CEnginePrimitiveThemeChannelDesc DefaultChannel; PerCreatureInfo(); }; };
CTCDivineWrath::PerCreatureInfo::PerCreatureInfo() {}