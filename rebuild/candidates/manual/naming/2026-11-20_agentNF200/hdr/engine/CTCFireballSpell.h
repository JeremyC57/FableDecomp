#ifndef FABLE_ENGINE_CTCFIREBALLSPELL_H
#define FABLE_ENGINE_CTCFIREBALLSPELL_H
// NEW (chunk 890, row 0x0056AD40 CTCBoastUI::Activate). Without the opt-in the header only names the class.
#ifdef FABLE_RETAIL_CTCFIREBALLSPELL
// Retail opt-in: RTTI .?AVCTCFireballSpell@@ : CTCBase (ghidra_out/rtti_bases.tsv; needs FABLE_RETAIL_CTCBASE, forced below). Members:
// FableWin.pdb ?StatGetInterfaceType@CTCFireballSpell@@SI?AW4ETCInterfaceType@@XZ (0x02154ABC; inline in retail: Activate looks up
// TC 157 = TCI_FIREBALL_SPELL, `mov dword ptr [esp+24h],9Dh` at 0x0056BDB5) and Ego_r.pdb 0x007C7D90 / FableWin.pdb 0x02721140
// ?CancelMove@CTCFireballSpell@@QAEXXZ (out of line in retail at 0x007C8FD0, Activate 0x0056BDD6). The data members are not modelled
// (no row reads them yet), so the class is used through pointers only.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#ifdef FABLE_RETAIL_CTCFIREBALLSPELL_MEMBERS
#include "fable_string.h"
#include "engine/CWillButtonHelperTimer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
class CSpecialAbilitiesFireballSpellDef;
#endif
class CTCFireballSpell : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_FIREBALL_SPELL; }
    void CancelMove();                                                   // 0x007C8FD0
#ifdef FABLE_RETAIL_CTCFIREBALLSPELL_FINISHSPELL
    // NF200 opt-in (2026-11-20, row 0x007C9180): Ego_r.pdb 0x007C7F80 + FableWin.pdb 0x02721680 ?FinishSpell@CTCFireballSpell@@QAEXXZ
    // (public, non-virtual), out of line in retail at 0x007C9180 (Ego_r order OnKill / OnRemove / FinishSpell / FrameUpdate = retail
    // 0x007C9160 / .. / 0x007C9180 / 0x007C93B0; called only from FrameUpdate 0x007C93B0). Declaration only.
    void FinishSpell();                                                  // 0x007C9180
#endif
#ifdef FABLE_RETAIL_CTCFIREBALLSPELL_MEMBERS
    // NF200 opt-in (2026-11-20, row 0x007C9180): the data members in the Ego_r.pdb order and spelling (struct_layouts_egor.tsv, 0x84 there)
    // with the retail 0x34-byte CWillButtonHelperTimer, so every member after the timer sits 4 bytes lower than the PDB says (as
    // engine/CTCTurncoatSpell.h); CIntelligentPointer is pack(1), so PTarget / PItemRemoved sit at +0x52 / +0x5A. The landed rows
    // 0x007C87F0 (constructor) and 0x007C93B0 / 0x007C9180 agree on every offset; sizeof 0x80. Member access is not recorded in the
    // name sets: kept private, as CTCTurncoatSpell.h.
private:
    CWillButtonHelperTimer Timer;                                        // +0x0c
    CDefPointer<const CSpecialAbilitiesFireballSpellDef> PDef;           // +0x40
    unsigned long InitLoopingSound;                                      // +0x44
    CIntelligentPointer<CThing> FireballBuildUpEffect;                   // +0x48
    bool ImmediateCastMode;                                              // +0x50
    bool OneAnimCastMode;                                                // +0x51
    CIntelligentPointer<CThing> PTarget;                                 // +0x52
    CIntelligentPointer<CThing> PItemRemoved;                            // +0x5a
    long WeaponState;                                                    // +0x64 (CTCSpecialAbilities::EWEAPON_STATE)
    bool LeftHandAnim;                                                   // +0x68
    CCharString FireballAttachmentName;                                  // +0x6c
    bool StartEffect;                                                    // +0x70
    long MaxLevelAllowableByStamina;                                     // +0x74
    long ReleaseDelayFrameCount;                                         // +0x78
    bool SpellFinished;                                                  // +0x7c
public:
#endif
#ifdef FABLE_RETAIL_CTCFIREBALLSPELL_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x007C95B0
public:
#endif
#ifdef FABLE_RETAIL_CTCFIREBALLSPELL_VSLOTS2
// agentFAM04 (2026-11-11): retail virtuals and the non-virtual they call (Ego_r.pdb / FableWin.pdb spellings; TLC vtable slots aligned with the Xbox 360 Test PDB order). Declarations only (no layout change).
public:
    virtual void OnKill();                                               // slot 14, 0x007C9160 (tail call to FinalCleanup)
private:
    void FinalCleanup();                                                 // 0x007C90A0 ?FinalCleanup@CTCFireballSpell@@AAEXXZ
public:
#endif // FABLE_RETAIL_CTCFIREBALLSPELL_VSLOTS2
};
#ifdef FABLE_RETAIL_CTCFIREBALLSPELL_MEMBERS
FABLE_STATIC_ASSERT(sizeof(CTCFireballSpell) == 0x80);
#endif
#else
class CTCFireballSpell;
#endif
#endif // FABLE_ENGINE_CTCFIREBALLSPELL_H
