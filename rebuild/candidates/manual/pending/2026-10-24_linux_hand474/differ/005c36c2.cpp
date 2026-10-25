// PutSheatheObjectInCarrySlot(CThing& owner, CThing& weapon, float& offset) -- puts the weapon's sheathe object (a new thing from the weapon def's SheatheObject at the owner's position, or the weapon itself) into its passive carry slot on the owner, at the running offset (advanced by the sheathe's def), scaled by its def's passive-slot scale, replacing what the slot held
// Grade: DIFFER(641v645) -- register/slot allocation only: retail zeroes the weapon def pointer after the carrying lookup (`and [ebp-0x14],0`, two `and` zero stores at entry) yet keeps the weapon in ebx and the && temporaries' flag local in memory; declaring the def pointer first (this file) gives the weapon ebx but hoists its zeroing into a shared `xor eax,eax` store run at entry, and the same-TU GetSubDef<CWeaponDef> stand-in caches the def pointer in esi where retail rereads [ebp-0x14] (an opaque GetSubDef<CWeaponDef> instead hands the flags ebx and spills the weapon). Tried: decl-order permutations x {opaque, escaping, same-TU} GetSubDef<CWeaponDef>, inline interface helper; best 83 normalised diff lines.
// Leaf: ?PutSheatheObjectInCarrySlot@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C36C2, 645 bytes (0x285, no padding), __fastcall `ret 4` (owner in ecx, weapon in
//   edx). Two callers (0x005C8144, 0x005C816D: CTCInventoryWeapons::PutWeaponsInSheathes, after its
//   RemoveWeaponsFromSheathes 0x005C552C call).
// Identity: Ego_r.pdb ?PutSheatheObjectInCarrySlot@?A0x0a34196c@@YIXAAVCThing@@0AAM@Z (anonymous namespace: written
//   with external linkage here so the unreferenced body stays in the object; the leaf is the bare name).
// Callees: std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (the inline CThing::GetInterface for
//   interfaces 0x46 carrying, 0x47 carryable, 0x1a showable item), CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6 /
//   <CCarryableDef> 0x005CC10E, CTCCarrying::IsCarryingThing(const CThing&) 0x006A1060 / GetThingInCarrySlot 0x006A11F0 /
//   PutThingInCarrySlot 0x006A1F80, CThing::GetPosition 0x004C73D0 / SetInLimbo 0x004C8CF0 / Kill 0x004C9B80,
//   CThingObject::Create 0x00703210 (__fastcall), CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E) /
//   ~CCharString 0x0099EAE0, IS_THING_ALIVE<CThing*> 0x0040DCE0, CTCCarryable::GetCarrySlotPassive 0x00705D00,
//   NGraphicAppearance::CTCGraphicAppearance::SetScale 0x004BFA50, CTCShowableItem::OnShowEnded 0x005D8E50, the defs'
//   virtual slot 1 on release.
// Layout: retail CThing as RemoveWeaponsFromSheathes 0x005C552C (+ PlayerNumber +0x90); CWeaponDef SheatheObject +0x38,
//   CCarryableDef OffsetAmountForThingsOnTopOfMe +0x44 / PassiveCarrySlotScale +0x48, CTCCarryable OffsetAmount +0x20
//   (Ego_r.pdb; defs are laid out alike).
// Shape: /Os frame-pointer TU, no EH. The weapon-def test is an && of two GetPBaseDef() temporaries (VC's flag local,
//   later reused for the sheathe pointer); the carryable-def lookup's GetPBaseDef() temporary is unconditional.
#pragma optimize("s", on)
#pragma optimize("y", off)


class CThing;

struct CDefPointee {                                     // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                           // slot 1 (+4)
    long m_refCount;                                     // +4
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            if (--Object->m_refCount == 0)
                Object->DestroyDef();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
    operator T*() const { return Object; }
};

class CWeaponDef : public CDefPointee {
public:
    unsigned char _pad_0x08[0x38 - 0x08];
    long          SheatheObject;                         // +0x38
};

class CCarryableDef : public CDefPointee {
public:
    unsigned char _pad_0x08[0x44 - 0x08];
    float         OffsetAmountForThingsOnTopOfMe;        // +0x44
    float         PassiveCarrySlotScale;                 // +0x48
};

class CDefClassBase : public CDefPointee {
public:
    bool GetSubDef(CDefPointer<CWeaponDef>* out) const;     // 0x0042B1A6 (GetSubDef<CWeaponDef>)
    bool GetSubDef(CDefPointer<CCarryableDef>* out) const;  // 0x005CC10E (GetSubDef<CCarryableDef>)
};
class CThingBaseDef : public CDefClassBase { };

void* FindSubDef_0042b1a6();                             // stand-in callees of the stand-in bodies below
void* FindSubDef_005cc10e();
#pragma auto_inline(off)
// Same-TU stand-ins for the GetSubDef<T> instantiations: they write the out pointer only, so VC7.1 knows its address
// does not escape the call (the landed CCombatManager::IsTargetOverriddenByAttemptedFacingDirection idiom).
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CWeaponDef>* out) const
{
    void* def = FindSubDef_0042b1a6();
    if (!def)
        return false;
    out->Object = (CWeaponDef*)def;
    return true;
}
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out) const
{
    void* def = FindSubDef_005cc10e();
    if (!def)
        return false;
    out->Object = (CCarryableDef*)def;
    return true;
}
#pragma auto_inline(on)

class C3DVector;
class CRightHandedSet;
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);                                   // 0x0099EBF0
    ~CCharString();                                                                    // 0x0099EAE0
};

struct CPresenceBits {
    bool test(long bit) const;                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                       // +0x00
    SEntry* last;                                        // +0x04
    SEntry* LowerBound(const long* type);                // 0x0040F020
};

struct CEngineGraphic {
    unsigned long BankIndex;                             // +0x00
    float         AnimStep;                              // +0x04
    float         RenderSizeX;                           // +0x08
};

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    unsigned char  _pad_0x00[0x44];
    CEngineGraphic MainGraphic;                          // +0x44
    void SetScale(float scale);                          // 0x004BFA50
};
}

class CThing {
public:
    unsigned char                            _pad_0x00[0x20];
    CPresenceBits                            UsingComponents;   // +0x20
    unsigned char                            _pad_0x21[0x23];
    CInterfaceMap                            Components;        // +0x44
    unsigned char                            _pad_0x4c[0x64 - 0x4c];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char                            _pad_0x68[0x70 - 0x68];
    CDefPointer<CThingBaseDef>               PBaseDef;          // +0x70
    unsigned char                            _pad_0x74[0x90 - 0x74];
    signed char                              PlayerNumber;      // +0x90
    unsigned char                            DeadFlags;         // +0x91 (bit 0: dead)
    CDefPointer<CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    bool HasTC(long type) const { return UsingComponents.test(type); }
    template <class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (UsingComponents.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* found = map->LowerBound(&type);
            CInterfaceMap::SEntry* it;
            if (found != map->last && !(wanted < found->type))
                it = found;
            else
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    void Kill(bool immediately);                         // 0x004C9B80
    void SetInLimbo(bool inLimbo);                       // 0x004C8CF0
    const C3DVector& GetPosition() const;                // 0x004C73D0
};

// 0x0040DCE0 (IS_THING_ALIVE<CThing*>): the header template, visible here and out of line as in retail (VC7.1 then knows the
// argument is only read).
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && (thing->DeadFlags & 1) == 0;
}

class CTCCarrying {                                      // interface 0x46
public:
    bool IsCarryingThing(const CThing& thing) const;     // 0x006A1060
    CThing* GetThingInCarrySlot(long slot) const;        // 0x006A11F0
    void PutThingInCarrySlot(CThing& thing, long slot, bool immediately);   // 0x006A1F80
};

class CTCShowableItem {                                  // interface 0x1a
public:
    void OnShowEnded();                                  // 0x005D8E50
};

class CThingObject : public CThing {
public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long playerNumber,
                                           const CRightHandedSet* orientation, unsigned long flags,
                                           const CCharString& scriptName);   // 0x00703210
};

class CTCCarryable {                                     // interface 0x47
public:
    unsigned char _pad_0x00[0x20];
    float         OffsetAmount;                          // +0x20
    long GetCarrySlotPassive() const;                    // 0x00705D00
};

void __fastcall PutSheatheObjectInCarrySlot(CThing& owner, CThing& weapon, float& offset)
{
    CDefPointer<CWeaponDef> weaponDef;
    CTCCarrying* carrying = 0;
    owner.GetInterface(&carrying, 0x46);
    long sheatheObject = -1;
    bool hasWeaponDef = weapon.GetPBaseDef() && weapon.GetPBaseDef()->GetSubDef(&weaponDef);
    if (hasWeaponDef)
        sheatheObject = weaponDef->SheatheObject;
    if (carrying->IsCarryingThing(weapon) && sheatheObject <= 0)
        return;
    CThing* sheathe = &weapon;
    if (sheatheObject > 0)
        sheathe = CThingObject::Create(weaponDef->SheatheObject, owner.GetPosition(), owner.PlayerNumber, 0, 0,
                                       CCharString(""));
    else if (sheatheObject == 0)
        sheathe = 0;
    CTCCarryable* carryable;
    if (IS_THING_ALIVE(sheathe) && sheathe->GetInterface(&carryable, 0x47)) {
        carryable->OffsetAmount = offset;
        CDefPointer<CCarryableDef> carryableDef;
        if (sheathe->GetPBaseDef()->GetSubDef(&carryableDef)) {
            offset += carryableDef->OffsetAmountForThingsOnTopOfMe;
            sheathe->PTCGraphicAppearance->SetScale(sheathe->PTCGraphicAppearance->MainGraphic.RenderSizeX *
                                                    carryableDef->PassiveCarrySlotScale);
        }
        long slot = carryable->GetCarrySlotPassive();
        CThing* existing = carrying->GetThingInCarrySlot(slot);
        if (IS_THING_ALIVE(existing)) {
            CTCShowableItem* showable;
            if (existing->GetInterface(&showable, 0x1a))
                showable->OnShowEnded();
            else
                existing->Kill(true);
        }
        sheathe->SetInLimbo(false);
        carrying->PutThingInCarrySlot(*sheathe, slot, true);
    }
}
