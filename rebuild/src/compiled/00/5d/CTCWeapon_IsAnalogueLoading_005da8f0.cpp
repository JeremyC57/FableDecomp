// CTCWeapon::IsAnalogueLoading() const -- the weapon definition's UseAnalogueLoading flag
// Leaf: ?IsAnalogueLoading@CTCWeapon@@QBE_NXZ
// Retail: Fable.exe @ 0x005DA8F0, 35 bytes; Ego_r.pdb ?IsAnalogueLoading@CTCWeapon@@QBE_NXZ. Body: a CDefPointer copy of
// WeaponDef (+0x0c; add-ref), read the byte at +0x90, release the copy. Ego_r.pdb CWeaponDef UseAnalogueLoading is +0x9c;
// retail CWeaponDef matches the PDB up to +0x50 (landed IsMeleeWeapon 0x005DAA00: Class +0x2c; GetAnimationSpeedValue
// 0x005DA850: AnimationSpeedVal +0x40) and sits 0x0c lower from +0x50 (the WeaponTrails map is a 12-byte STLport tree
// and FlourishParticles shrinks too), the layout landed IsMeleeWeapon documents.
// Layout: Ego_r.pdb CTCWeapon WeaponDef +0x0c.
// Shape: /O2 (`mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax`, result held in bl), as sibling 0x005DA7D0.
struct CDefPointeeBase {
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1 (+4)
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CWeaponDef : CDefPointeeBase {
    unsigned char _pad08[0x1c];                          // +0x08
    unsigned char _pad24[0x6c];                          // +0x24 Type, Class, ... (as Ego_r.pdb up to +0x50)
    bool          UseAnalogueLoading;                    // +0x90
};
class CTCWeapon {
public:
    unsigned char           _pad00[0xc];                 // +0x00 CTCBase
    CDefPointer<CWeaponDef> WeaponDef;                   // +0x0c
    bool IsAnalogueLoading() const;
};
bool CTCWeapon::IsAnalogueLoading() const
{
    CDefPointer<CWeaponDef> def(WeaponDef);
    return def->UseAnalogueLoading;
}