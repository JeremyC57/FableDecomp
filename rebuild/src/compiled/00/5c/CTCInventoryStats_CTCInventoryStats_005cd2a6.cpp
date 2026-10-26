#pragma optimize("s", on)
// CTCInventoryStats::CTCInventoryStats(CThing*) -- builds the CTCInventoryBase and NHeroInformationScreens::CBase bases, stores both vtables, constructs the CreaturesKilled map and zeroes the mannequin / category / cached-TC members
// Leaf: ??0CTCInventoryStats@@QAE@PAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CD2A6, length 88 bytes (unaligned, no padding: size-optimised inventory module).
// Manifest label "CTCHero::CTCHero" is propagated; the vtable stores (0x01254ADC primary, 0x01254AC4 at +0x130) are
// CTCInventoryStats's (rtti-vtables.tsv).
// Callees: CTCInventoryBase::CTCInventoryBase(CThing*) @ 0x005BFF07, NHeroInformationScreens::CBase::CBase @ 0x0066E734 (stores
// vtable 0x0123A3E0; out of line in this module), the std::map<unsigned long,long> constructor @ 0x004533C5 (manifest
// "CDisplayManager::CopyBackBufferToTexture" is propagated) for CreaturesKilled at +0x134.
// Shape: base constructors in declaration order, then the two vtable stores, then the member map's constructor (its `lea ecx`
// is scheduled before the vtable stores), then the plain initialisers in declaration order through one zero register
// (`xor eax,eax` + dword/byte stores). Retail layout (VC7.1 release STL, 0xc map): +0x134 map, +0x140 pMannequin, +0x144
// UseMannequin, +0x148 lastCategoryIdx, +0x14c pTCHeroExperience, +0x150 pTCHeroStats (Ego_r.pdb has the same order shifted
// by its larger debug STL; no engine header exists for CTCInventoryStats).
// Pragma: optimize("s", on).
class CThing;
class CTCHeroExperience;
class CTCHeroStats;

struct CTCInventoryBase {
    virtual void v00();
    unsigned char _base[0x12c];                             // +0x04 .. +0x130
    CTCInventoryBase(CThing* thing);                        // 0x005BFF07
};

namespace NHeroInformationScreens {
class CBase { public:                                              // vtable 0x0123A3E0
    virtual void v00();
    CBase();                                                // 0x0066E734
};
}

struct CCreatureKillMap {                                   // std::map<unsigned long, long> (VC7.1 release: 0xc bytes)
    unsigned char _body[0xc];
    CCreatureKillMap();                                     // 0x004533C5
};

class CTCInventoryStats : public CTCInventoryBase, public NHeroInformationScreens::CBase { public:
    CCreatureKillMap CreaturesKilled;                       // +0x134
    CThing* pMannequin;                                     // +0x140
    bool UseMannequin;                                      // +0x144
    long lastCategoryIdx;                                   // +0x148
    CTCHeroExperience* pTCHeroExperience;                   // +0x14c
    CTCHeroStats* pTCHeroStats;                             // +0x150
    virtual void v00();
    CTCInventoryStats(CThing* thing);
};

CTCInventoryStats::CTCInventoryStats(CThing* thing)
    : CTCInventoryBase(thing), pMannequin(0), UseMannequin(false), lastCategoryIdx(0),
      pTCHeroExperience(0), pTCHeroStats(0)
{
}