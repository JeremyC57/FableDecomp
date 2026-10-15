// CTCInventoryWeapons::CloseInventory() -- clears every active-weapon primitive handle, re-seats the active weapons on the carrier and marks the inventory closed
// Leaf: ?CloseInventory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CAC8D, 44 bytes. rtti-vtables: CTCInventoryWeapons 0x01253FF4 slot 31
// (catalog label "vfunc_31"; slot 31 is CloseInventory in CTCInventoryStats 0x005CC6F3, which also ends by clearing
// InventoryOpen).
// Callees: SGI for_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle>> @ 0x0043F360 (`__fastcall`,
//          hidden functor result in ecx, first in edx, last and the functor on the stack, `ret 8`; manifest label
//          "_For_each<pair<long,CCharString> const*, CAddDefNamesToComboBox>" is a propagated twin), the member
//          CEnginePrimitiveHandle::Clear @ 0x0041BE70 (pushed as the 4-byte pointer-to-member),
//          CTCInventoryWeapons::SetupActiveWeaponsOnCarrier @ 0x005CA68E.
//
// Shape: `push ecx` reserves the discarded functor result, whose address goes in ecx (`lea ecx,[esp+0Ch]`); the
// vector's first is loaded into edx, last and `offset Clear` pushed -- `std::for_each(v.begin(), v.end(),
// std::mem_fun_ref(&CEnginePrimitiveHandle::Clear))` with a non-POD functor (user constructor => hidden return) and a
// single-inheritance complete class (4-byte member pointer). Size TU (`#pragma optimize("s", on)`). Members:
// ActiveWeaponPrimitiveHandles PDB +0x16C -> retail +0x154, CTCInventoryBase::InventoryOpen PDB +0x60 -> retail +0x58.
// The SGI algorithm and the functor are reconstructed by hand (the DDK STL is not the retail revision). Local model.
#pragma optimize("s", on)
struct CEnginePrimitiveHandle {
    long m_id;
    void* m_pPrimitive;
    void Clear();                             // 0x0041BE70
};

template <class R, class T> class mem_fun_ref_t {
public:
    explicit mem_fun_ref_t(R (T::*pfn)()) : m_pfn(pfn) {}
    R operator()(T& object) const { return (object.*m_pfn)(); }
private:
    R (T::*m_pfn)();
};

template <class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pfn)())
{
    return mem_fun_ref_t<R, T>(pfn);
}

template <class It, class F> F __fastcall for_each(It first, It last, F f);   // 0x0043F360

struct CPrimitiveHandleVector {               // retail vector<CEnginePrimitiveHandle>, 12 bytes
    CEnginePrimitiveHandle* _First;
    CEnginePrimitiveHandle* _Last;
    CEnginePrimitiveHandle* _End;
    CEnginePrimitiveHandle* begin() { return _First; }
    CEnginePrimitiveHandle* end() { return _Last; }
};

struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x58 - 4];
    bool InventoryOpen;                       // +0x58
    unsigned char _pad_0x59[0x130 - 0x59];
};
namespace NHeroInformationScreens {
struct CBase { virtual void v00(); };
}
struct CTCInventoryWeapons : CTCInventoryBase_Retail, NHeroInformationScreens::CBase {
    unsigned char _pad_0x134[0x154 - 0x134];
    CPrimitiveHandleVector ActiveWeaponPrimitiveHandles;   // +0x154
    void SetupActiveWeaponsOnCarrier();       // 0x005CA68E
    void CloseInventory();
};
void CTCInventoryWeapons::CloseInventory()
{
    for_each(ActiveWeaponPrimitiveHandles.begin(), ActiveWeaponPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    SetupActiveWeaponsOnCarrier();
    InventoryOpen = false;
}
