#pragma optimize("s",on)
// SavedConfirmed @ 0x0047db73  (69 bytes)  __fastcall, void, 0 params.
//
// Retail idiom:
//   eax = g_A;                          ; a1 ds:0x13b86a0
//   ecx = g_A->f1c;                     ; mov ecx,[eax+0x1c]  -> this for c1
//   B*  b = (that)->GetB();             ; call c1, ret eax
//   C*  c = b->GetC();                  ; mov ecx,eax; call c2, ret eax -> esi
//   Wrapper w;                          ; local at [ebp-4]
//   w.p = c;                            ; mov [ebp-4],esi
//   if (w.Check()) {                    ; lea ecx,[ebp-4]; call c3 -> al
//       ((D*)((char*)c + 0x20))->Do(0xC6);   ; push 0xC6; lea ecx,[esi+0x20]; call c4
//       g_pGlobalGui->Notify();         ; mov ecx,ds:0x13b8790; call c5
//   }

// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its PPlayerManager +0x1C through the inline PeekPlayerManager(); the first two callees are
//   Ego_r/FableWin ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ 0x00449960 (engine/CPlayerManager.h) and
//   ?GetPPlayerCharacterThing@CPlayer@@QBEPAVCThing@@XZ 0x00487B70 (engine/CPlayer.h). The later callees (Wrapper::Check 0x0040DCE0,
//   D::Do 0x00410DE0, GlobalGui::Notify 0x00438310 on 0x013B8790) are still the row's stand-ins; was the stand-in A* g_A with its BSrc::GetB / C::GetC stand-ins.
// NF200 (2026-11-20, q4522): the stand-in Wrapper::Check (?Check@Wrapper@@QAE_NXZ, a thiscall on a one-pointer struct) is the shared
//   IS_THING_ALIVE<CThing*> of engine/ThingAlive.h (FableWin.pdb 0x018962E0 ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z, __fastcall by
//   const reference, out of line at 0x0040DCE0; FABLE_RETAIL_IS_THING_ALIVE_NOINLINE keeps the visible body out of line as retail calls it;
//   its pick-any COMDAT is byte-identical to the owner row 0x0040DCE0, cmpbody). The local Wrapper model is gone.
// Inferred: the argument is an rvalue copy of the pointer (static_cast to its own type): retail stores esi to [ebp-4], passes that slot
//   and keeps using esi after the call (no reload), so the callee's reference is to a temporary, not to the named local.
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
// NF209 (2026-11-20, q11772): the stand-in D::Do(int) on `(char*)c + 0x20` (?Do@D@@QAEXH@Z at 0x00410DE0) is the component-presence test on
//   CThing::UsingComponents (std::bitset<274> +0x20): Ego_r.pdb + FableWin.pdb ?test@?$bitset@$0BBC@@std@@QBE_NI@Z, called out of line in this
//   size-optimised TU (FABLE_RETAIL_STLP_BITSET_TEST_DECL, as the other 337 landed callers), reached through the inline CThing::HasTC;
//   0xC6 = TCI_IN_GAME_MENU (engine/ETCInterfaceType.h). Inferred: retail discards the result (no test after the call), so the statement
//   is the bare HasTC call; the local struct D is gone.
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
// NF211 (2026-11-21, q11772): landed as NF209's draft unchanged (coordinator ruling, batches/2026-11-20_landing_nf210/README.md):
//   the object's 8-byte inline ?HasTC@CThing@@QBE_NW4ETCInterfaceType@@@Z COMDAT differs from landed 0x00488620's 58-byte COMDAT
//   (retail 0x004118C0, no landed row); both are pick-any, so it is copy-against-copy debt (the nf198 case), queued.
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#include "engine/CThing.h"
#include "engine/ThingAlive.h"

struct GlobalGui {
};

extern GlobalGui* g_pGlobalGui;  // ds:0x13b8790

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00438310 ?ToggleLiveGui@CPlayerGui@@QAEXXZ (was ?Notify@GlobalGui@@QAEXXZ)
class CPlayerGui { public: void ToggleLiveGui(); };
void __fastcall SavedConfirmed()
{
    CThing* c = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPPlayerCharacterThing();

    if (IS_THING_ALIVE(static_cast<CThing*>(c))) {
        c->HasTC(TCI_IN_GAME_MENU);
        reinterpret_cast<CPlayerGui*>(g_pGlobalGui)->ToggleLiveGui();
    }
}