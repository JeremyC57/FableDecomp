// CTCWeapon::SetProjectileWeaponLoaded(bool loaded, CThing* ammo) -- tell the ammo's augmentations component to copy the weapon thing's augmentation data, record the loaded flag, then a weapon whose def needs loading and is not loaded stops aiming (AimingTime -1, override cleared) while any other restarts an unset (-1) aiming timer at 0
// Leaf: ?SetProjectileWeaponLoaded@CTCWeapon@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DAC50, 156 bytes (+4 int3), __fastcall `ret 8`.
// Identity: both PDB name sets hold CTCWeapon::SetProjectileWeaponLoaded (catalog row). Callees:
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (landed spelling, inlined GetInterface for interface
//   0xBF = CTCObjectAugmentations, bitset word 5 bit 31); CTCObjectAugmentations::CopyAugmentationDataFromThing(const
//   CThing&) 0x00768210 (Ego_r.pdb ?CopyAugmentationDataFromThing@CTCObjectAugmentations@@QAEXABVCThing@@@Z, landed; the
//   catalog "CTCObjectAugmentations_InitAbilitySlot" is descriptive) with the weapon's ParentThing; the def's virtual
//   slot 1 (Delete) when the copied def pointer drops its last reference.
// Layout: engine/CTCWeapon.h (WeaponDef +0x0C, ProjectileWeaponLoaded +0x10, AimingTime +0x14, AimingTimeOverride
//   +0x18) through the `#define CTCWeapon CTCWeaponLayout` include; ParentThing is the flattened CTCBase +0x04
//   (engine/CTCBase.h). CWeaponDef::NeedsLoading is retail +0xD0 (Ego_r.pdb +0xE0; the STLport maps save 0x10 before it --
//   the landed CTCWeapon::IsCapableOfStealthAttackBy 0x005DAD60 / GetAnimationSpeedValue 0x005DA850 model the same byte).
// Shape: the def flag is read into a named bool through the by-value GetWeaponDef() copy (add-ref null-tested, release
//   unconditional) so the temporary dies before the tests. The aiming decision is a NAMED bool condition: `bool canAim =
//   !needsLoading || ProjectileWeaponLoaded; if (canAim) { if (AimingTime == -1) AimingTime = 0; } else { ...-1 arm }`
//   lays the restart arm first with its own `ret` and the -1 arm last falling into the shared epilogue (GOTCHAS: `bool
//   writing = a || b; if (writing)` lays the then-arm first). Every direct `if` spelling (|| or &&, nested, early return,
//   if/else either way round) lays the -1 arm first (the parked 156v156 block-placement residue).
//   The local CDefPointer release `if (--IVRefCount == 0) Delete();` gives retail's `mov eax,[ecx+4] ; push ebx ; mov bl ;
//   dec eax`; the shared engine/CDefPointer.h (Release() returning the count) grades 156v156 with `dec eax` before `push ebx`.
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCWeapon CTCWeaponLayout
#include "engine/CTCWeapon.h"
#undef CTCWeapon
class CTCBase;

class CDefPointeeBase { public:                             // completes the header's forward declaration (counted pointee)
    virtual void v00();
    virtual void Delete() const;                            // slot 1 (+0x04): called when the count drops to zero
    long IVRefCount;                                        // +0x04
    void AddRef() { ++IVRefCount; }
    void Release() { if (--IVRefCount == 0) Delete(); }
};
class CWeaponDef : public CDefPointeeBase { public:         // retail layout (0x10 below Ego_r.pdb past ArrowTrails)
    unsigned char _pad_0x08[0xd0 - 0x08];
    bool          NeedsLoading;                             // +0xd0 (retail; Ego_r.pdb +0xe0)
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    __forceinline ~CDefPointer() { Object->Release(); }
    T* operator->() const { return static_cast<T*>(Object); }
};

enum ETCInterfaceType { TCI_OBJECT_AUGMENTATIONS = 0xbf };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCObjectAugmentations { public:
    void CopyAugmentationDataFromThing(const CThing& thing);   // 0x00768210
};

class CTCWeapon : public CTCWeaponLayout { public:
    CThing* GetParentThing() const { return reinterpret_cast<const CTCBaseLayout*>(this)->ParentThing; }
    const CDefPointer<const CWeaponDef>& WeaponDef() const
    {
        return *reinterpret_cast<const CDefPointer<const CWeaponDef>*>(&WeaponDef_Object);
    }
    __forceinline CDefPointer<const CWeaponDef> GetWeaponDef() const { return WeaponDef(); }
    void SetProjectileWeaponLoaded(bool loaded, CThing* ammo);
};

void CTCWeapon::SetProjectileWeaponLoaded(bool loaded, CThing* ammo)
{
    if (ammo) {
        CTCObjectAugmentations* augmentations;
        if (ammo->GetInterface(&augmentations, TCI_OBJECT_AUGMENTATIONS))
            augmentations->CopyAugmentationDataFromThing(*GetParentThing());
    }
    ProjectileWeaponLoaded = loaded;
    bool needsLoading = GetWeaponDef()->NeedsLoading;
    bool canAim = !needsLoading || ProjectileWeaponLoaded;
    if (canAim) {
        if (AimingTime == -1)
            AimingTime = 0;
    } else {
        AimingTime = -1;
        AimingTimeOverride = false;
    }
}