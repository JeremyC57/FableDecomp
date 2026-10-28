// NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon::IsSatisfied() -- satisfied when the creature's def names a ranged weapon (RangedWeaponDef > 0)
// Leaf: ?IsSatisfied@CIsRunnableModuleCarryingProjectileWeapon@NCreatureAI@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00982890, 46 bytes (+ int3 padding), thiscall virtual (slot 0), `ret`.
// Identity (proven): Ego_r.pdb ?IsSatisfied@CIsRunnableModuleCarryingProjectileWeapon@NCreatureAI@@UAE_NXZ (0x009A2E20);
//   RTTI NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon : CIsRunnableModuleBase, vtable slot 0 is this row.
// Callees: the def's Destroy (virtual slot 1) when the temporary's release drops the count to zero.
// Layout: Ego_r.pdb CIsRunnableModuleBase World +4 / Creature +8; CThing PBaseDef CDefPointer<const CThingBaseDef> at retail
//   +0x70 (Ego_r +0x80); CThingCreatureDef RangedWeaponDef at retail +0xE0 (probable: Ego_r +0xE8, the field the name tests).
// Shape: CThing::GetPBaseDef() (Ego_r.pdb, by value; inline) read through the temporary's operator-> into a def local
//   (the copy's ++ and the release's -- fuse into `inc ; mov ; dec ; store ; store`), then `if (x > 0) return true;
//   return false;` (folds to `test ; setg`).
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
template <class T> class CDefPointer {                  // the landed 0x00861E60 model
public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
class CDefClassBase {
public:
    virtual ~CDefClassBase();
    virtual void Destroy() const;                       // slot 1
    mutable long RefCount;                              // +0x04
};
class CThingBaseDef : public CDefClassBase { };
class CThingCreatureDef : public CThingBaseDef {
public:
    unsigned char _pad08[0xE0 - 0x08];
    long RangedWeaponDef;                               // +0xE0 (Ego_r.pdb +0xE8)
};
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x70 - 0x04];
    CDefPointer<const CThingBaseDef> PBaseDef;          // +0x70 (Ego_r.pdb +0x80)
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
};
class CThingAICreature : public CThing { };
class CWorld;
namespace NCreatureAI {
class CIsRunnableModuleBase {                           // Ego_r.pdb: vptr, World +4, Creature +8
public:
    virtual bool IsSatisfied() = 0;
protected:
    CWorld* World;                                      // +0x04
    CThingAICreature* Creature;                         // +0x08
};
class CIsRunnableModuleCarryingProjectileWeapon : public CIsRunnableModuleBase {
public:
    virtual bool IsSatisfied();
};

bool CIsRunnableModuleCarryingProjectileWeapon::IsSatisfied()
{
    const CThingCreatureDef* def = static_cast<const CThingCreatureDef*>(Creature->GetPBaseDef().operator->());
    if (def->RangedWeaponDef > 0)
        return true;
    return false;
}
}