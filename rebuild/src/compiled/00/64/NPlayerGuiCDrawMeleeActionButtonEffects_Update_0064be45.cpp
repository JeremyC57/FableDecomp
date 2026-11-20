// NPlayerGui::CDrawMeleeActionButtonEffects::Update() -- pick the hero's current melee ability def (the flourish ability when a flourish is valid, else the block-counter ability when a block counter is valid), restart the flash timers when it changes, clear the effects when there is none, and count the timers down
// Leaf: ?Update@CDrawMeleeActionButtonEffects@@QAEXXZ
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064BE45, length 373 bytes (frame pointer, `ret`).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::PeekPControlledCreature @ 0x00487DD0, IS_THING_ALIVE @ 0x0043DBF0
// (`__fastcall`, by reference), std::bitset::test @ 0x00410DE0, the interface map LowerBound const @ 0x004365B0, CTCCombat::
// IsMeleeFlourishValid @ 0x00733EE0, ::GetPMeleeFlourishAbility @ 0x00733480 (CDefPointer by hidden pointer), ::IsMeleeBlockCounterValid
// @ 0x00733290, ::GetPMeleeBlockCounterAbility @ 0x007334B0 (thiscall on the CTCCombat component, CDefPointer by hidden pointer; all four
// Ego_r.pdb / FableWin.pdb const members), ClearEffects @ 0x006445BA (thiscall on the Effects vector), the def's virtual slot 1 on release.
// Layout: Ego_r.pdb CDrawMeleeActionButtonEffects minus 4 (the 16-byte PDB vector is 12 in retail): PlayerManager +0x08, Effects +0x28,
// FlashTimer +0x34, ComboLoadingIconTimer +0x38, FlourishCounter +0x3c, FlourishEffect +0x40, PMeleeAbilityDef +0x44 (Ego_r.pdb member
// names, offsets minus 8 after the two 16-byte PDB vectors; FableWin.pdb ?SetFlourishEffect@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXJ@Z
// writes FlourishEffect; name-fix pass 2026-11-12, were the descriptive names EffectCount (+0x40) and PCurrentAbility (+0x44)). CTCCombat = interface 0x30.
// Shape: frame-pointer size TU; IS_THING_ALIVE (out of line in retail) has a same-TU read-only stand-in body so the hero's frame word
// does not escape and is reused for the lookup key and the second temporary; the component comes from the out-param GetInterface
// (the type constant 0x30 is held in ebx); both getters are called on the CTCCombat component; the chosen def is a CDefPointer local
// assigned from each getter's full-expression temporary (engine/CDefPointer.h operator=: the local is known null, so only the AddRef
// copy remains); the temporaries and the member release through the shared CDefPointer (clear only inside the non-null arm).
// Naming (NF138, 2026-11-20; q4747): the local CDefPointer / CAbilityDef / CRefCounted / CDefVirtual models replaced by engine/CDefPointer.h
//   (CDefPointer<const CMeleeCombatAbilityDef>, Ego_r.pdb PMeleeAbilityDef type); the CTCCombat declarations respelled in place to the
//   PDB members: GetPMeleeFlourishAbility QAE CDefPointer<CAbilityDef> -> QBE CDefPointer<const CMeleeCombatAbilityDef>, the stand-ins
//   IsShowingBaseAbility / GetPBaseAbility -> IsMeleeBlockCounterValid / GetPMeleeBlockCounterAbility, IsMeleeFlourishValid QAE -> QBE.
// Port (NF177, q11775): the local std::bitset / std::pair / CVectorMap / CThing / CThingPlayerCreature / CPlayer / IS_THING_ALIVE /
//   CTCCombat / CEffectVector models are replaced by the shared headers (engine/CThing.h GETTC_EXPANDED with the out-of-line bitset
//   test, CThingPlayerCreature.h, CPlayer.h, ThingAlive.h NOINLINE, CTCCombat.h opt-ins, NPlayerGui_CDrawBase.h CLEAREFFECTS), so the
//   effects call binds ?ClearEffects@CDrawBase@NPlayerGui@@SIXAAV?$vector@V?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@..@Z
//   (Ego_r.pdb 0x0068766A + FableWin.pdb 0x020D944C, 0x006445BA). The drawer class keeps its row-local model (no shared header).
#include "fable_compiler.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_WEAPONTRAIL
#define FABLE_RETAIL_CTCCOMBAT_GETPMELEEFLOURISHABILITY
#define FABLE_RETAIL_CTCCOMBAT_GETPMELEEBLOCKCOUNTERABILITY
#define FABLE_RETAIL_CTCCOMBAT_ISMELEEBLOCKCOUNTERVALID
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CThing.h"
#include "engine/CTCCombat.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/ThingAlive.h"
#include "engine/NPlayerGui_CDrawBase.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CMeleeCombatAbilityDef;
struct CDrawMeleeActionButtonEffects {
    unsigned char            _pad0[0x08];
    CPlayerManager*          PlayerManager;        // +0x08
    unsigned char            _pad1[0x28 - 0x0c];
    std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;   // +0x28 (STLport, 12 bytes)
    long                     FlashTimer;           // +0x34
    long                     ComboLoadingIconTimer;// +0x38
    long                     FlourishCounter;      // +0x3c
    long                     FlourishEffect;          // +0x40
    CDefPointer<const CMeleeCombatAbilityDef> PMeleeAbilityDef; // +0x44
    void Update();
};

void CDrawMeleeActionButtonEffects::Update()
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    CDefPointer<const CMeleeCombatAbilityDef> ability;
    if (IS_THING_ALIVE(hero)) {
        const CTCCombat* combat;
        if (hero->PeekTC(TCI_COMBAT, &combat)) {
            if (combat->IsMeleeFlourishValid()) {
                ability = combat->GetPMeleeFlourishAbility();
            }
            else if (combat->IsMeleeBlockCounterValid()) {
                ability = combat->GetPMeleeBlockCounterAbility();
            }
            if (ability.Object != PMeleeAbilityDef.Object) {
                ComboLoadingIconTimer = -1;
                FlashTimer = 15;
            }
        }
    }
    PMeleeAbilityDef = ability;
    if (FlashTimer == 0)
        ComboLoadingIconTimer = 30;
    if (!ability.Object) {
        NPlayerGui::CDrawBase::ClearEffects(Effects);
        FlourishCounter = -1;
        FlourishEffect = 0;
        ComboLoadingIconTimer = -1;
        FlashTimer = -1;
    }
    if (ComboLoadingIconTimer > 0)
        --ComboLoadingIconTimer;
    if (FlashTimer > 0)
        --FlashTimer;
}