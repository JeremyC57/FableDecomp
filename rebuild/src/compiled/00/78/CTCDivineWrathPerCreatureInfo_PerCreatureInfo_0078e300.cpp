// CTCDivineWrath::PerCreatureInfo::PerCreatureInfo() -- initialize tracked pointers and the default lighting channel.
// Leaf: ??0PerCreatureInfo@CTCDivineWrath@@QAE@XZ
// Identity: probable owner/function role; PDB unique24-byte PerCreatureInfo record has ActualCreature+0,FramesToGetHit+8,Effect+12,DefaultChannel+20; both PDB sets contain CTCDivineWrath nested ctor; owner allocation by position in own spell code region, no xrefs.
// Layout: generated outer headers have no nested record; outer declaration supplies naming scope only. Real eight-byte tracked pointers and four-byte channel retain Pad/FramesToGetHit uninitialized.
class CThing; class CThingCreatureBase;
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); CBaseIntelligentPointer(); void* PData; };
template<class T> class CIntelligentPointer:public CBaseIntelligentPointer { public: CIntelligentPointer():CBaseIntelligentPointer() {} };
class CEnginePrimitiveThemeChannelDesc { public: unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad; CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0) {} };
class CTCDivineWrath { public: struct PerCreatureInfo { CIntelligentPointer<CThingCreatureBase> ActualCreature; long FramesToGetHit; CIntelligentPointer<CThing> Effect; CEnginePrimitiveThemeChannelDesc DefaultChannel; PerCreatureInfo(); }; };
CTCDivineWrath::PerCreatureInfo::PerCreatureInfo() {}