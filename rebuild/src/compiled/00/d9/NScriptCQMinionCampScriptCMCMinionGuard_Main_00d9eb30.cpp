// NScript::CQ_MinionCampScript::CMC_MinionGuard::Main() -- minion-camp guard entity script: each frame, when Me is hit and the camp says Me is a hero attacker, retarget Me onto whoever hit it unless that is already its target (the hero while it has none)
// Leaf: ?Main@CMC_MinionGuard@CQ_MinionCampScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9EB30, 288 bytes, plain `ret`; slot 1 of the CMC_MinionGuard vtable 0x012D6D28 (stored by
//   the landed Alloc 0x00D9EA80, the "MC_MinionGuard" binding of CQ_MinionCampScript).
// Identity: Ego_r.pdb ?Main@CMC_MinionGuard@CQ_MinionCampScript@NScript@@EAEXXZ and ?SetTarget@CMC_MinionGuard@CQ_MinionCampScript@NScript@@AAEXABVCScriptThing@@@Z
//   (private), ?IsAssignedAsHeroAttacker@CQ_MinionCampScript@NScript@@QAE_NABVCScriptThing@@@Z (reloc_bindings 0x00D9EC50).
// Callees: CEntityScriptBase::IsActiveThreadTerminating() const 0x00F35B30 (x7), CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CScriptThing slots 21 MsgIsHitBy (+0x54), 26 MsgWhoHitMe (+0x68), 77 IsNull (+0x134), 78 IsEqualTo
//   (+0x138), ~CScriptThing 0x004AA840, CQ_MinionCampScript::IsAssignedAsHeroAttacker 0x00D9EC50, CMC_MinionGuard::SetTarget
//   0x00D9ECF0, Game slots 70 GetHero (+0x118) and 7 NewScriptFrame (+0x1c). Literal "" 0x0122D70E.
// Layout: Ego_r.pdb CMC_MinionGuard (0x28, the landed Alloc's new size): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, Target +0x1C -- engine/CQ_MinionCampScript.h (new FABLE_RETAIL_CQ_MINIONCAMPSCRIPT_MINIONGUARD: the nested
//   CMC_MinionGuard in its PDB spelling), engine/CEntityScriptBase.h, engine/CScriptThing.h (FABLE_RETAIL_NSCRIPT), fable_string.h
//   CCharString (FABLE_RETAIL_CCHARSTRING; there is no engine/CCharString.h).
// Shape: a rotated `while (!IsActiveThreadTerminating())`; one whole `if (attacker != X) { check; SetTarget(attacker); }` per
//   Target.IsNull() arm: VC7.1 cross-jumps the two compare+SetTarget tails into one, and the merged call block loads
//   `this` before the argument push (`lea eax ; mov ecx,esi ; push eax`, 0x00D9EC0D) as retail; one shared `bool same` gives
//   `lea ; push ; mov ecx`; the "" CCharString temporary dies before the hit test; CScriptThing::
//   operator== is expanded inline here (`PImp->IsEqualTo(*other.PImp)`: the other side's Data pushed, ecx = attacker's Data).
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CEntityScriptBase.h"
// Retail expands these at this row's sites: defined inline after the header.
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
// operator!= 0x008AAE40 is the negation of operator== (the landed 0x008AAE40 / 0x00E6B400 spelling).
inline bool CScriptThing::operator!=(const CScriptThing& other) const { return !(*this == other); }

#define FABLE_RETAIL_CQ_MINIONCAMPSCRIPT_MINIONGUARD
#include "engine/CQ_MinionCampScript.h"

namespace NScript {

void CQ_MinionCampScript::CMC_MinionGuard::Main()
{
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsHitBy("")) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->IsAssignedAsHeroAttacker(Me)) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptThing attacker = Me.MsgWhoHitMe();
                if (Target.IsNull()) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (attacker != Game->GetHero()) {
                        if (IsActiveThreadTerminating())
                            return;
                        SetTarget(attacker);
                    }
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    if (attacker != Target) {
                        if (IsActiveThreadTerminating())
                            return;
                        SetTarget(attacker);
                    }
                }
            }
        }
        Game->NewScriptFrame();
    }
}
}