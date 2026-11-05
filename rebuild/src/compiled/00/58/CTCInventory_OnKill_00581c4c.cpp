// CTCInventory::OnKill() -- deregister the inventory's hero-information screen (its NHeroInformationScreens::CBase subobject) from the player GUI's screen manager, then run the base NInventory::CTCInventoryBase::OnKill
// Leaf: ?OnKill@CTCInventory@@UAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581C4C, 40 bytes (+ int3 padding to 44), thiscall, tail-jumps to the base.
// Identity (proven): CTCInventory::OnKill; evidence: the only reference to 0x00581C4C is slot 14 (OnKill, as in every CTCBase vtable) of the vtable at 0x01250DE4 whose RTTI complete-object locator names .?AVCTCInventory@@; the PDB name sets hold ?OnKill@CTCInventory@@UAEXXZ; the body deregisters the CBase subobject at +0x130 and tail-jumps to NInventory::CTCInventoryBase::OnKill 0x005BC9C8.
// Label: the manifest's "CTCInventoryBase::OnClose_Alt" is propagated across byte-identical bodies (family H012: seven inventory screens
//   whose OnKill overrides compile to the same bytes; retail has no identical-code folding, so each address is the
//   override of one class).
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0, NHeroInformationScreens::CManager::DeregisterScreen
//   0x0066EB78 (landed), NInventory::CTCInventoryBase::OnKill 0x005BC9C8 (tail jump); global (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790.
// Shape: the screen argument is the null-checked this -> CBase conversion (`neg ; sbb ; and`, CBase at +0x130), which
//   VC7.1 evaluates before loading (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()).
// Pragma: optimize("s", on) -- a size TU (as the base's 0x005BC9C8): the branchless null-checked conversion.
#pragma optimize("s", on)
namespace NHeroInformationScreens {
class CBase {                                       // the screen interface (own vtable at +0x130 of the inventory)
public:
    virtual void OnScreenDraw();
};
class CManager {
public:
    void DeregisterScreen(const CBase& screen);     // 0x0066EB78
};
}
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui {
public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
namespace NInventory {
class CTCInventoryBase {                            // 0x130 bytes in retail (vptr first)
public:
    virtual void OnKill();                          // 0x005BC9C8 (vtable slot 14)
    unsigned char _body[0x130 - 4];
};
}
class CTCInventory : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    virtual void OnKill();
};
void CTCInventory::OnKill()
{
    NHeroInformationScreens::CBase* screen = this;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetPHeroInfoScreensManager()->DeregisterScreen(*screen);
    NInventory::CTCInventoryBase::OnKill();
}