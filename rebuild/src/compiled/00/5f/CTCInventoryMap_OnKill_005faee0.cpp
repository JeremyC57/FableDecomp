// CTCInventoryMap::OnKill() -- deregister the inventory's hero-information screen (its NHeroInformationScreens::CBase subobject) from the player GUI's screen manager, then run the base NInventory::CTCInventoryBase::OnKill
// Leaf: ?OnKill@CTCInventoryMap@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FAEE0, 44 bytes, thiscall, tail `jmp` to the base OnKill.
// Identity (proven): the only reference is slot 14 (CTCBase OnKill) of the CTCInventoryMap vtable 0x01257A9C (0x01257AD4; RTTI
//   .?AVCTCInventoryMap@@); Ego_r.pdb and FableWin.pdb name sets ?OnKill@CTCInventoryMap@@UAEXXZ (Ego_r donor 0x0064B910, FableWin 0x02083670).
//   Byte twin of CTCInventoryQuests::OnKill 0x0061A1F0: retail has no identical-code folding, so each address is its own class's override, named
//   from its own vtable. The catalog label "CTCInventory::OnKill" names a class with no such override.
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0 (Ego_r ?GetPHeroInfoScreensManager@CPlayerGui@@QBEPAV
//   CManager@NHeroInformationScreens@@XZ), NHeroInformationScreens::CManager::DeregisterScreen(const CBase&) 0x0066EB78 (landed),
//   NInventory::CTCInventoryBase::OnKill 0x005BC9C8 (tail jump); global CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass
//   0x013B8790 (private static, read through the inline Get(), as the landed twins 0x005CCEAB / 0x005B3509).
// Shape: the screen argument is the null-checked this -> CBase conversion (CBase at +0x130) evaluated before the singleton
//   load; this TU is speed-optimised (test / je / lea / jmp / xor), unlike the size-optimised twins 0x005CCEAB / 0x005B3509 / 0x005C2EDB /
//   0x0056EECA (`neg ; sbb ; and` under `#pragma optimize("s", on)`).
// Layout: NInventory::CTCInventoryBase is 0x130 bytes in retail (vptr first), the screen interface follows at +0x130.
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
class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
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
class CTCInventoryMap : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {   // vtable 0x01257A9C
public:
    virtual void OnKill();
};
void CTCInventoryMap::OnKill()
{
    NHeroInformationScreens::CBase* screen = this;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetPHeroInfoScreensManager()->DeregisterScreen(*screen);
    NInventory::CTCInventoryBase::OnKill();
}