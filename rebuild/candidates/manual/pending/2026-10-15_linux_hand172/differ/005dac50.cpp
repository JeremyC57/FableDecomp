// CTCWeapon::SetProjectileWeaponLoaded(bool, CThing*) -- record the loaded flag, tell the ammo's augmentations, and reset the aiming timer
// Grade: DIFFER(156v156) -- block placement only: retail puts the AimingTime=-1/Override=false arm last (je/je into it), ours places it right after the tests (jne over it)
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DAC50, 156 bytes (+4 int3). __thiscall, `ret 8`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020; CTCObjectAugmentations member taking the weapon's
//          parent thing 0x00768210 (catalog "CTCObjectAugmentations_InitAbilitySlot"; exact name unverified); def release
//          via vtable slot 1.
// Shape: a non-null thing's interface 0xBF (CTCObjectAugmentations, bitset word 5 bit 31) is called with ParentThing
//        (out-parameter form: `mov eax,[eax+4] ; push ; mov ecx,eax`); the def's analogue-loading byte (+0xD0, descriptive)
//        is read into a named bool local through a by-value GetWeaponDef() (release = the def's inline Release()) so the
//        temporary dies before the ProjectileWeaponLoaded test; an analogue weapon that is
//        not loaded clears AimingTime to -1 and AimingTimeOverride, otherwise an unset (-1) AimingTime restarts at 0.
// Layout: CTCWeapon (engine header): ParentThing +0x04, WeaponDef +0x0C, ProjectileWeaponLoaded +0x10, AimingTime +0x14,
//         AimingTimeOverride +0x18.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (another TU's copy is 0x004365B0)
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
struct CWeaponDef {
    virtual void v00();
    virtual void Destroy();                      // slot 1
    long RefCount;                               // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
    unsigned char _pad_0x08[0xd0 - 0x08];
    bool IsAnalogueLoading;                      // +0xd0 (descriptive)
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
struct CTCObjectAugmentations {
    void OnProjectileLoaded(CThing* weaponThing); // 0x00768210
};
struct CTCWeapon {
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[4];
    CDefPointer<CWeaponDef> WeaponDef;           // +0x0c
    bool ProjectileWeaponLoaded;                 // +0x10
    long AimingTime;                             // +0x14
    bool AimingTimeOverride;                     // +0x18
    __forceinline CDefPointer<CWeaponDef> GetWeaponDef() const { return WeaponDef; }
    void SetProjectileWeaponLoaded(bool loaded, CThing* ammo);
};
void CTCWeapon::SetProjectileWeaponLoaded(bool loaded, CThing* ammo)
{
    if (ammo) {
        CTCObjectAugmentations* augmentations;
        if (ammo->GetInterface(&augmentations, 0xbf))
            augmentations->OnProjectileLoaded(ParentThing);
    }
    ProjectileWeaponLoaded = loaded;
    bool analogue = GetWeaponDef()->IsAnalogueLoading;
    if (!analogue || ProjectileWeaponLoaded) {
        if (AimingTime == -1)
            AimingTime = 0;
        return;
    }
    AimingTime = -1;
    AimingTimeOverride = false;
}
