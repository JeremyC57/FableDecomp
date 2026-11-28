#ifndef FABLE_ENGINE_CTCHEROMORPH_H
#define FABLE_ENGINE_CTCHEROMORPH_H
// Overlay header (chunk 1009b): there is no promoted engine/CTCHeroMorph.h (the generated flattened layout sits in
// engine/_quarantine/ with a reconciliation conflict). This file only carries a retail opt-in; without it the class is
// forward-declared.
#if defined(FABLE_RETAIL_CTCHEROMORPH) || defined(FABLE_RETAIL_CTCHEROMORPH_TATTOOTHINGS) || defined(FABLE_RETAIL_CTCHEROMORPH_TATTOONAMES)
// Retail opt-in (chunk 1009b, row 0x0059FC17 CTCInventoryTrade::AlreadyHaveSpecial): the hero's morph component as the trade
// screen reaches it. RTTI .?AVCTCHeroMorph@@ : CTCAppearanceMorphBase : CTCBase (ghidra_out/rtti_bases.tsv); no
// CTCAppearanceMorphBase header exists, so the class derives CTCBase directly (both single-inheritance at offset 0). The GetTC key
// is CTCAppearanceMorphBase's interface (Ego_r.pdb ?StatGetInterfaceType@CTCAppearanceMorphBase@@SI?AW4ETCInterfaceType@@XZ:
// TCI_APPEARANCE_MORPH = 3, `push 3 ; pop edi` 0x0059FC99). Members (Ego_r.pdb / FableWin.pdb, out of line, both landed):
// ?GetTattoos@CTCHeroMorph@@QBEXAAV?$vector@V?$CDefPointer@$$CBVCTattooDef@@@@V?$allocator@V?$CDefPointer@$$CBVCTattooDef@@@@@std@@@std@@@Z
// 0x0071DD50 and ?HasAPermanentTattooCoveringArea@CTCHeroMorph@@QBEJXZ 0x0071C2D0. Members not modelled. Needs
// FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_CTATTOODEF (the vector element).
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CTattooDef.h"
#include "stlport/fable_containers.h"
class CThingBaseDef;
#ifdef FABLE_RETAIL_CTCHEROMORPH_TATTOONAMES
class CWideString;
#endif
#ifdef FABLE_RETAIL_HERO_MORPH_SERIALISE_PASS29
#include "engine/CTCAppearanceMorphBase.h"
class CPersistContext;
class CTCHeroMorph : public CTCAppearanceMorphBase {
public:
    virtual void OnSerialise(CPersistContext& context, bool in_game);
private:
    unsigned char _unmodelled_0c[0x10];
    std::vector<CDefPointer<const CThingBaseDef> > Tattoos;
#ifdef FABLE_RETAIL_HERO_APPEARANCE_UPDATE_PASS30
    unsigned char _unmodelled_28[0x0c];
    long FrameTattoosChanged;
    unsigned char _unmodelled_38[8];
#else
    unsigned char _unmodelled_28[0x18];
#endif
    float Strength, Berserk, Will, Skill, Age, Morality, Fatness, Tan;
    bool Teenager;
#else
class CTCHeroMorph : public CTCBase {
#endif
public:
#ifdef FABLE_RETAIL_HERO_APPEARANCE_UPDATE_PASS30
    long GetFrameTattoosLastChanged() const { return FrameTattoosChanged; }
#endif
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_APPEARANCE_MORPH; }
#ifdef FABLE_RETAIL_HERO_MODIFIER_GRAPHICS_PASS29
    float GetMorphStrength(EHeroMorphType morph_type) const;
#endif
#ifdef FABLE_RETAIL_CTCHEROMORPH_KILLPARTICLES
    // p25Q opt-in (row 0x0051F0E0 CThingManager::CopyHeroToHero): Ego_r.pdb ?KillParticles@CTCHeroMorph@@QAEXXZ, out of line in retail at
    // 0x0071C260 (`mov ecx,<morph> ; call 0071C260h`, 0x0051F17B). Declaration only.
    void KillParticles();                                                 // 0x0071C260
#endif
#ifdef FABLE_RETAIL_CTCHEROMORPH
    void GetTattoos(std::vector<CDefPointer<const CTattooDef> >& tattoos) const;   // 0x0071DD50
    long HasAPermanentTattooCoveringArea() const;                                   // 0x0071C2D0
#endif
#ifdef FABLE_RETAIL_CTCHEROMORPH_TATTOOTHINGS
    // Nested opt-in (w22 agent2012 / acctB r3019, row 0x006D1250 CTCWife::CheckAppearanceChanges): Ego_r.pdb / FableWin.pdb
    // ?GetTattooThings@CTCHeroMorph@@QBEXAAV?$vector@V?$CDefPointer@$$CBVCThingBaseDef@@@@V?$allocator@...@std@@@std@@@Z (Ego_r 0x00568D50
    // after GetTattoos 0x005686C0 = retail 0x0071DE20 after 0x0071DD50), out of line in retail (`lea eax,<vector> ; push eax ;
    // mov ecx,morph ; call 0x0071DE20`, 0x006D1691). The class block above with this opt-in alone gives the interface key only.
    // Declaration only.
    void GetTattooThings(std::vector<CDefPointer<const CThingBaseDef> >& tattoos) const;   // 0x0071DE20
#endif
#ifdef FABLE_RETAIL_CTCHEROMORPH_ADDTATTOO
    // Nested opt-in (egod e04, row 0x005AD305 CTCInventoryTrade::ApplyTattooCard): Ego_r.pdb ?AddTattoo@CTCHeroMorph@@QAEXAAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z
    // (Ego_r 0x00568390) and ?AddTattoo@CTCHeroMorph@@QAEXJ@Z (Ego_r 0x005684E0), out of line in retail at 0x0071D940 / 0x0071DA70 (same
    // order; the trade row calls them with the morph in ecx: `push ebx ; mov ecx,edi ; call 0x0071D940`, `push [ebp+8] ; call
    // 0x0071DA70`). Declarations only.
    void AddTattoo(CDefPointer<const CThingBaseDef>& tattoo_thing);    // 0x0071D940
    void AddTattoo(long tattoo_def_index);                              // 0x0071DA70
#endif
#ifdef FABLE_RETAIL_CTCHEROMORPH_SETBERSERKSKIN
    // Nested opt-in (hand3c3, row 0x0078D250 CTCBerserk::FrameUpdate): FableWin.pdb / Ego_d.pdb ?SetBerserkSkin@CTCHeroMorph@@QAEXM@Z,
    // out of line in retail at 0x0071BEE0 (landed; FrameUpdate calls it with the morph in ecx at 0x0078D66D / 0x0078D856).
    // Declaration only.
    void SetBerserkSkin(float berserk);                                 // 0x0071BEE0
#endif
#ifdef FABLE_RETAIL_CTCHEROMORPH_TATTOONAMES
    // Nested opt-in (chunk 4006 / acctC agentC1, rows 0x005D1F0C / 0x005CE0E6 CTCInventoryStats::ConstructPersonalityListPC /
    // ConstructPersonalityList): Ego_r.pdb / FableWin.pdb ?GetTattooNames@CTCHeroMorph@@QAEXAAV?$vector@VCWideString@@V?$allocator@
    // VCWideString@@@std@@@std@@@Z (non-const), out of line at 0x0071CE80 (landed). Declaration only.
    void GetTattooNames(std::vector<CWideString>& names);                // 0x0071CE80
#endif
};
#else
class CTCHeroMorph;
#endif
#endif // FABLE_ENGINE_CTCHEROMORPH_H
