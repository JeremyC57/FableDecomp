// CTCEnemy::OnInitialActivate @ 0076c830
// Leaf: ?CTCEnemy_OnInitialActivate@@YIXPAVCTCEnemy@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
class CTCEnemy;

// the player GUI at 0x013B8790; CPlayerGui::UpdateMiniMapMarkers(bool) 0x004378C0 (Ego_r.pdb / FableWin.pdb, chunk 563)
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool force);   // 0x004378C0
};

// base OnInitialActivate (tail jmp 0x686870), __fastcall(this)
void __fastcall CTCEnemy_base_OnInitialActivate(CTCEnemy* enemy);

void __fastcall CTCEnemy_OnInitialActivate(CTCEnemy* enemy)
{
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
    CTCEnemy_base_OnInitialActivate(enemy);
}