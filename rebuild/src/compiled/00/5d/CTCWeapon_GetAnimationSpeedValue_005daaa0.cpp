// CTCWeapon::GetAnimationSpeedValue() -- the weapon def's animation speed, scaled by the parent's object augmentations
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DAAA0, 109 bytes (+3 int3). __fastcall, float in st(0).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0; CTCObjectAugmentations speed multiplier getter
//          0x00766140 (catalog "GetDamageMultiplier"; which multiplier is not verified); def release via vtable slot 1.
// Shape: the def speed (+0x44) is read through a by-value GetWeaponDef() into a float local (dword copy to [esp+4]);
//        interface 0xBF (CTCObjectAugmentations, bit 31 of CThing bitset word 5 at +0x34 -> `jns`) looked up with the
//        inline GetInterface; `speed * mult` gives `call ; fmul [speed]`.
// Layout: CTCWeapon ParentThing +0x04, WeaponDef +0x0C; retail CThing interface bitset +0x20, interface map +0x44.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0 (another TU's copy is 0x0040F020)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CWeaponDef { public:
    virtual void v00();
    virtual void Destroy();                      // slot 1
    long RefCount;                               // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
    unsigned char _pad_0x08[0x44 - 0x08];
    float AnimationSpeed;                        // +0x44
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
class CTCObjectAugmentations { public:
public:
    float GetDamageMultiplier() const;   // 0x00766140
};
class CTCWeapon { public:
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[4];
    CDefPointer<CWeaponDef> WeaponDef;           // +0x0c
    __forceinline CDefPointer<CWeaponDef> GetWeaponDef() const { return WeaponDef; }
    float GetAnimationSpeedValue() const;
};
float CTCWeapon::GetAnimationSpeedValue() const
{
    float speed = GetWeaponDef()->AnimationSpeed;
    CTCObjectAugmentations* augmentations;
    if (ParentThing->GetInterface(&augmentations, 0xbf))
        return speed * augmentations->GetDamageMultiplier();
    return speed;
}