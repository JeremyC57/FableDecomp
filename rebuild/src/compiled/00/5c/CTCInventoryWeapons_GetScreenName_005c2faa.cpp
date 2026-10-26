// CTCInventoryWeapons::GetScreenName() const -- the screen's title for the hero-information screen list: the inventory def's screen-name text entry, fetched through the base's GetInventoryText
// Leaf: ?GetScreenName@CTCInventoryWeapons@@UBE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2FAA, 53 bytes (+ int3 padding to 55), thiscall on the
//   NHeroInformationScreens::CBase subobject (+0x130), `ret 4` (hidden CWideString result).
// Identity (proven): CTCInventoryWeapons::GetScreenName; evidence: the only reference to 0x005C2FAA is slot 0 of the vtable at 0x01253FE0 (RTTI .?AVCTCInventoryWeapons@@), the NHeroInformationScreens::CBase secondary vtable whose slot 0 is GetScreenName (landed CTCInventoryMagic::GetScreenName 0x0072DB00 at slot 0 of the magic screen's); the PDB name sets hold ?GetScreenName@CTCInventoryWeapons@@UBE?AVCWideString@@XZ; it adjusts this by -0x130 and calls NInventory::CTCInventoryBase::GetInventoryText.
// Label: the manifest's "CTCInventoryWeapons::vfunc_0" is a slot placeholder (family H013: seven inventory screens whose GetScreenName
//   overrides compile to the same bytes; retail has no identical-code folding, so each address is one class's override).
// Callees: NInventory::CTCInventoryBase::GetInventoryText(long) const 0x005BD273 (landed; hidden result passed through),
//   the def's slot 1 on the last release.
// Shape: the landed CTCInventoryMagic::GetScreenName 0x0072DB00 pattern: a copied CDefPointer local (add-ref behind a
//   null test, release without one once the def has been dereferenced); PDef is the base's +0x68, so it is read at
//   [ecx-0C8h] from the CBase subobject. The tag at CInventoryDef +0x1EC is the retail offset (no PDB member resolved;
//   descriptive name ScreenNameTag).
// Pragma: optimize("s", on) -- the inventory screen TUs are size TUs (family H012 sits next to each row).
#pragma optimize("s", on)
class CWideString {
public:
    void* PStringData;
};
class CInventoryDef {                               // def object: vptr, reference count +4
public:
    virtual void v00();
    virtual void Destroy();                         // slot 1 (last release)
    long RefCount;                                  // +0x04
    unsigned char _pad08[0x1EC - 0x08];
    long ScreenNameTag;                             // +0x1EC
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
namespace NInventory {
class CTCInventoryBase {                            // 0x130 bytes in retail (vptr first)
public:
    virtual void OnKill();
    unsigned char _pad04[0x68 - 0x04];
    CDefPointer<CInventoryDef> PDef;                // +0x68
    unsigned char _pad6c[0x130 - 0x6C];
    CWideString GetInventoryText(long textIndex) const;   // 0x005BD273
};
}
namespace NHeroInformationScreens {
class CBase {
public:
    virtual CWideString GetScreenName() const;      // slot 0
};
}
class CTCInventoryWeapons : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    virtual CWideString GetScreenName() const;
};
CWideString CTCInventoryWeapons::GetScreenName() const
{
    CDefPointer<CInventoryDef> def = PDef;
    return GetInventoryText(def->ScreenNameTag);
}