// CCS_OakValeRevisitedScript::OakValeFire() -- Oakvale-revisited cutscene thread: wait until the "fire" flag is set,
// gather the Q_REVISITED_FIREPOINT things, size Fires to match and create an OAKVALE_BURNING_PATCH effect at every fire
// point; then wait until the flag is cleared again and remove every fire effect
// Leaf: ?OakValeFire@CCS_OakValeRevisitedScript@NScript@@QAEXXZ
// Flags: /d2inlT76
// Inline-budget: /d2inlT76 -- the implicit CScriptThing::operator= at the CreateEffect store (the CCountedPointer ShareData body, retail
//   0x00EE89F5..0x00EE8A4E) is a call at the default threshold T74 and expanded at T76 / T78 / T82 / T90 (each RELOCATION_MATCH); smallest tested.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE8870, 862 bytes (866 with padding).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0, map<CCharString,bool>::operator[] 0x008ADF10, CScriptThing
//   default ctor 0x006E7B40 / dtor 0x004AA840 / slot 6 GetPos, vector<CScriptThing>::resize(n, const T&) 0x00D34AC0
//   (the one-argument resize is inline), CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots
//   7 NewScriptFrame, 75 GetAllThingsWithScriptName, 100 CreateEffect, 108 RemoveThing. The by-value CreateEffect result is
//   assigned with CScriptThing's implicit operator= (CCountedPointer::operator= expanded inline).
//   Literals "fire" 0x012C1978, "Q_REVISITED_FIREPOINT" 0x012F5458, "FirePoints" 0x012F544C, "OAKVALE_BURNING_PATCH" 0x012F5434.
// Shape: retail expands CScriptThing::operator= here although its out-of-line copy exists (0x008AB980). Element access is *(begin() + i): the start pointer is read before the CreateEffect call, the index added after.
// Layout: Ego_r.pdb CCS_OakValeRevisitedScript - 0x14 (retail CScriptBase is 0x40): FirePoint +0x48, Fires +0x54,
//   OakValeFlag +0x60 (map<CCharString,bool>), v1..v7 handles +0x78.. (GSI +0x40, Owner +0x44).
// NF87 (2026-11-17): moved onto the shared engine/CCS_OakValeRevisitedScript.h (CLevelScriptBase<CQ_SunnyvaleMasterData>, Game
//   +0x40) with the FABLE_RETAIL_CCS_OAKVALEREVISITEDSCRIPT_OAKVALEFLAG_MAP opt-in (OakValeFlag is the STLport
//   map<CCharString, bool>, so the call binds 0x008ADF10 under its real operator[] spelling) and CScriptThing's implicit assignment.
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CCS_OAKVALEREVISITEDSCRIPT_OAKVALEFLAG_MAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CCS_OakValeRevisitedScript.h"

void NScript::CCS_OakValeRevisitedScript::OakValeFire()
{
    while (OakValeFlag["fire"] != true) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GetAllThingsWithScriptName("Q_REVISITED_FIREPOINT", FirePoint);
    Fires.resize(FirePoint.size());
    for (unsigned int i = 0; i < FirePoint.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Fires[i] = Game->CreateEffect("OAKVALE_BURNING_PATCH", FirePoint[i].GetPos(), "FirePoints", 0.0f, false, false);
    }
    if (IsActiveThreadTerminating())
        return;
    while (OakValeFlag["fire"] != false) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Fires.resize(FirePoint.size());
    for (unsigned int j = 0; j < Fires.size(); ++j) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Fires[j], false, true);
    }
    if (IsActiveThreadTerminating())
        return;
}
