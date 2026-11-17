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
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"

struct D {
    void Do(int arg);  // __fastcall, one stack arg (masked callee c4)
};

struct Wrapper {
    CThing* p;         // +0x0
    bool Check();      // __fastcall on &wrapper, ret bool (masked callee c3)
};

struct GlobalGui {
    void Notify();     // __fastcall, no args (masked callee c5)
};

extern GlobalGui* g_pGlobalGui;  // ds:0x13b8790

void __fastcall SavedConfirmed()
{
    CThing* c = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPPlayerCharacterThing();

    Wrapper w;
    w.p = c;
    if (w.Check()) {
        ((D*)((char*)c + 0x20))->Do(0xC6);
        g_pGlobalGui->Notify();
    }
}