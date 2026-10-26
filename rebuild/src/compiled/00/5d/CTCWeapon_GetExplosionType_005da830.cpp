// CTCWeapon::GetExplosionType() const -- the CWeaponDef's Explosion, read through an add-ref'd copy of the def pointer
// Leaf: ?GetExplosionType@CTCWeapon@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA830, 32 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (proven): CTCWeapon::GetExplosionType() const; evidence: CTCWeapon's accessor block, after GetDamageType 0x005DA810 and before GetAnimationSpeedValue 0x005DA850 (Ego_r.pdb order); it reads WeaponDef (+0x0C) -> +0x4C = Ego_r.pdb CWeaponDef::Explosion (long); CThingGameObject::OnHit 0x00839940 calls it on the TC it fetches by interface type 0x48 (CTCWeapon); both PDB name sets hold ?GetExplosionType@CTCWeapon@@QBEJXZ.
// Label: the manifest's "GetCategoryIdentifier" is propagated (family J099: five byte-similar def accessors that differ only in the
//   field offset; retail has no identical-code folding).
// Callees: CWeaponDef's virtual Delete (CIVCountedPointeeBase slot 1) when the copy drops the last reference.
// Shape: the landed CTCWeapon::GetWeaponType 0x005DA7D0: a CDefPointer<const CWeaponDef> copy of WeaponDef (+0x0C; inc
//   IVRefCount when non-null), the field read, then the copy's inline release (dec; Delete on zero -- its null test folds into
//   the dereference).
// Layout: retail RTTI CTCWeapon <- CTCBase (engine/CTCBase.h, data-only, included under a layout name); CWeaponDef <- CSubDefClassBase
//   <- CDefClassBase <- CDefPointeeBase <- CResource <- CIVCountedPointeeBase (Ego_r.pdb members; CDefClassBase is 0x25 bytes).
#define CTCBase CTCBasePdb
#include "engine/CTCBase.h"
#undef CTCBase
class CTCBase : public CTCBasePdb { };                      // engine/CTCBase.h is data-only; nothing virtual is needed here
class CIVCountedPointeeBase {                               // Ego_r.pdb 8 bytes: vptr, IVRefCount
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (FableWin.pdb ?Delete@CIVCountedPointeeBase@@UBEXXZ)
    __forceinline void AddRef() const { ++IVRefCount; }      // FableWin.pdb ?AddRef@CIVCountedPointeeBase@@QBEXXZ (inline here)
    __forceinline long Release() const                      // FableWin.pdb ?Release@CIVCountedPointeeBase@@QBEJXZ (inline here)
    {
        if (--IVRefCount == 0) {
            Delete();
            return 0;
        }
        return (long)IVRefCount;
    }
    mutable unsigned long IVRefCount;                       // +0x04
};
class CResourceList;
class CResource : public CIVCountedPointeeBase {            // Ego_r.pdb 0x1C
public:
    CResourceList* ResourceList;                            // +0x08
    CResource*     PrevResource;                            // +0x0C
    CResource*     NextResource;                            // +0x10
    unsigned long  ResourceSize;                            // +0x14
    unsigned long  LastUsedFrame;                           // +0x18
};
class CDefPointeeBase : public CResource { };               // Ego_r.pdb 0x1C, no members of its own
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {              // Ego_r.pdb 0x25 bytes
public:
    CDefinitionManager* PDefManager;                        // +0x1C
    unsigned long       GlobalIndex;                        // +0x20
    unsigned char       Setup : 1;                          // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
class CSubDefClassBase : public CDefClassBase { };          // Ego_r.pdb 0x25 bytes, no members of its own
template <class T> class CDefPointer {                      // Ego_r.pdb 4 bytes
public:
    CDefPointer(const CDefPointer<T>& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }   // the IV release, inline (as the landed 0x005DA7D0)
    T* operator->() const { return static_cast<T*>(Object); }   // FableWin.pdb ??C?$CDefPointer@$$CB...@@QBEPB...@@XZ
    CDefPointeeBase* Object;                                // +0x00
};
enum EWeaponType { };                                       // Ego_r.pdb (enumerators not needed)
enum EWeaponClass { };
enum EDamageAttribute { };
class CCharString {                                         // 4 bytes (TLC)
public:
    void* PStringData;                                      // +0x00
};
class CWeaponDef : public CSubDefClassBase {                // RTTI: CWeaponDef <- CSubDefClassBase (vtable 0x0124291C); members to +0xB0
public:
    EWeaponType      Type;                                  // +0x28
    EWeaponClass     Class;                                 // +0x2C
    unsigned long    Property;                              // +0x30
    CCharString      AnimationGroupName;                    // +0x34
    long             SheatheObject;                         // +0x38
    bool             SheatheObjectAlwaysVisible;            // +0x3C
    float            AnimationSpeedVal;                     // +0x40
    float            Damage;                                // +0x44
    EDamageAttribute DamageType;                            // +0x48
    long             Explosion;                             // +0x4C
    unsigned char    FlourishParticlesToWeaponTrails[0x40]; // +0x50 FlourishParticles, FlourishSpecialWeaponTrail, WeaponTrails
                                                            //   (STLport containers: 0xC shorter than Ego_r.pdb's Dinkumware ones)
    bool             UseAnalogueLoading;                    // +0x90 (Ego_r.pdb +0x9C)
    long             MissileLoadCarrySlot;                  // +0x94
    CCharString      ProjectileWeaponHelperNameTip;         // +0x98
    CCharString      ProjectileWeaponHelperNameBase;        // +0x9C
    long             Shot;                                  // +0xA0
    long             Ammo;                                  // +0xA4
    long             TargetingRange;                        // +0xA8 (Ego_r.pdb +0xB4)
    float            TargetingFOV;                          // +0xAC
};
class CTCWeapon : public CTCBase {
public:
    CDefPointer<const CWeaponDef> WeaponDef;                // +0x0C
    long GetExplosionType() const;
};

long CTCWeapon::GetExplosionType() const
{
    CDefPointer<const CWeaponDef> def(WeaponDef);
    return def->Explosion;
}