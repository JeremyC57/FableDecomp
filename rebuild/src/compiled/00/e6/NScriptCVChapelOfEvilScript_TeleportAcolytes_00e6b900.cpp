// NScript::CV_ChapelOfEvilScript::TeleportAcolytes(const CScriptThing& acolyte) -- send the evil acolytes to their teleport
//   markers: if the given acolyte lives, it goes to M_COE_AcolyteTeleport1 and every other acolyte to M_COE_AcolyteTeleport2;
//   otherwise the first two live acolytes go to markers 1 and 2. Each step returns early when the script thread terminates.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6B900, length 841 bytes, __fastcall, `ret 4`.
// Leaf: ?TeleportAcolytes@CV_ChapelOfEvilScript@NScript@@QAEXABVCScriptThing@@@Z
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CBaseClass() 0x0099A2D0 / ~CBaseClass
//   0x0099A2E0 (inline CScriptThing ctor / dtor); ~CScriptThing 0x004AA840 (by-value temporaries and the early returns);
//   CScriptThing::operator= 0x008AB980; ~vector<CScriptThing> 0x008AC970 (early returns; inline at the end: element
//   virtual dtor slot 0, free 0x00BFEA14); operator delete 0x00BFE9BC; CScriptBase::IsActiveThreadTerminating 0x00CB7940;
//   CScriptThing slots 75 IsAlive (+0x12c) and 78 IsEqualTo (+0x138, via the inline operator==);
//   CGameScriptInterfaceBase slots 72 GetThingWithScriptName (+0x120), 75 GetAllThingsWithScriptName (+0x12c),
//   472 EntityTeleportToThing (+0x760).
// Shape: the early terminate returns and the marker reassignments sit under `#pragma inline_depth(0)` so the destructors
//   there go out of line (retail calls 0x004AA840 / 0x008AC970 there); the function end expands both destructors.
//   operator== is spelled through CCountedPointer::operator-> (PImp->IsEqualTo(*other.PImp)); the raw .Data spelling
//   swaps eax/edx in the loop.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_ChapelOfEvilScript.h"
// Callee bodies of landed rows (real bodies, not stand-ins; operator== is RESPELLED from its landed row, see below):
//   CScriptThing::CScriptThing() = 0x006E7B40, rebuild/src/compiled/00/6e/CScriptThing_CScriptThing_006e7b40.cpp.
//     Inlined in retail: no call to 0x006E7B40; instead `call 0x0099A2D0` (CBaseClass()) at 0xE6B94F and the CScriptThing
//     vptr 0x01238C8C stored inline (`mov [esp+0x1c], 0x1238c8c` at 0xE6B95C), so the body is visible here (`inline`).
//   CScriptThing::~CScriptThing() = 0x004AA840, rebuild/src/compiled/00/4a/CScriptThing_CScriptThing_004aa840.cpp
//     (called out of line for the temporaries / early returns, inline at the end via ~CBaseClass 0x0099A2E0).
//   CScriptThing::operator== = 0x008AAE20, rebuild/src/compiled/00/8a/CScriptThing_operator_008aae20.cpp.
//     Inlined in retail: no call to 0x008AAE20; the IsEqualTo virtual (slot 78) is called directly as
//     `call dword ptr [edx+0x138]` at 0xE6B9BE.
//     The landed row spells the same body with made-up names (`IsSameThing` on a local CScriptThingImp_Retail, defined as
//     CScriptThing_Methods::operator==); the PDB names are ?IsEqualTo@CScriptThing@@MBE_NABV1@@Z (FableWin 0x017940FA /
//     0x018CD700; vtable 0x01238C8C slot 78 = 0x004AA8A0) and CScriptThing::operator== (Ego_r + FableWin). NAME_FIXES hands it back.
// Inline-budget: retail expands ~CScriptThing at the end of 0x00E6B900 (the ~CBaseClass 0x0099A2E0 call is in line), which
//   VC7.1 only does for the forced-inline spelling of the empty destructor; the temporaries call 0x004AA840 out of line.
inline CScriptThing::CScriptThing() {}
FABLE_FORCEINLINE CScriptThing::~CScriptThing() {}
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }

void NScript::CV_ChapelOfEvilScript::TeleportAcolytes(const CScriptThing& acolyte)
{
    std::vector<CScriptThing> acolytes;
    long count = Game->GetAllThingsWithScriptName("COE_AcolyteEvil", acolytes);
    CScriptThing marker;
    if (acolyte.IsAlive()) {
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
        for (long i = 0; i < count; ++i) {
            if (acolytes[i] == acolyte) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
                marker = Game->GetThingWithScriptName("M_COE_AcolyteTeleport1");
#pragma inline_depth()
                Game->EntityTeleportToThing(acolyte, marker, false);
            } else {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
                marker = Game->GetThingWithScriptName("M_COE_AcolyteTeleport2");
#pragma inline_depth()
                Game->EntityTeleportToThing(acolytes[i], marker, false);
            }
        }
    } else {
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
        if (acolytes.size() != 0 && acolytes[0].IsAlive()) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
            marker = Game->GetThingWithScriptName("M_COE_AcolyteTeleport1");
#pragma inline_depth()
            Game->EntityTeleportToThing(acolytes[0], marker, false);
        }
        if (acolytes.size() > 1 && acolytes[1].IsAlive()) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
            marker = Game->GetThingWithScriptName("M_COE_AcolyteTeleport2");
#pragma inline_depth()
            Game->EntityTeleportToThing(acolytes[1], marker, false);
        }
    }
}