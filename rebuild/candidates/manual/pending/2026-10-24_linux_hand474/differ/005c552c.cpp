// CTCInventoryWeapons::RemoveWeaponsFromSheathes() -- for each live active weapon (melee, ranged) that is not held in the owner's primary carry slot, take its sheathe object off the owner (the weapon def's SheatheObject: a carried thing, or the weapon itself), resetting its scale and ending its show or killing it
// Grade: DIFFER(551v546) -- loop shape: retail inverts the weapon loop (guard test at the top, `add eax,4; cmp; jne` at the bottom, the iterator in [ebp-0x18] shared with the push_back temporaries) while VC7.1 here cross-jumps to one bottom test (`jmp`; iterator in [ebp-0x10]), which shifts the weapon def / GetPBaseDef temporaries one slot; the conditional temporaries, inline dtors, lookups and calls all match. Codegen experiments: the merge goes away only when the carrying lookup before the loop is unconditional or the showable lookup is removed; `continue` vs nested if, while / do-while, plain vs forceinline lookups, a same-TU LowerBound, GetTC+HasTC all keep it; declaring the vector first inverts the loop but moves its zeroing above the lookup. Also: retail's `else if` is `!= 0` (`je`), this file's `< 0` gives `jge`.
// Leaf: ?RemoveWeaponsFromSheathes@CTCInventoryWeapons@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C552C, 546 bytes (0x222, no padding), thiscall `ret`. Five callers (0x005C8109,
//   0x005C99B7 in RefreshSheathedWeapons, 0x005CB1D8 / 0x005CB282 in CloseInventory, 0x0069FB23 in
//   CCreatureAction_UnsheatheItemFromInventory::PerformAction).
// Identity: Ego_r.pdb ?RemoveWeaponsFromSheathes@CTCInventoryWeapons@@QAEXXZ: the caller at 0x005C8109 runs it before
//   the two anonymous-namespace PutSheatheObjectInCarrySlot calls (0x005C36C2), i.e. it is PutWeaponsInSheathes'
//   clean-up step; the catalog's "CTCCarryable_PrepareWeaponSheathing" is descriptive.
// Callees: std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (the inline CThing::GetInterface / HasTC),
//   IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340 and IS_THING_ALIVE<CThing*> 0x0040DCE0 (__fastcall, by
//   reference), CBaseIntelligentPointer::GetPItem 0x00A01B50, vector<CThing*>::push_back 0x00436C80 (out of line),
//   CTCCarrying::IsCarryingThing(const CThing&) 0x006A1060 / IsCarriedThingInPrimarySlot 0x006A1490 / GetCarriedThing(long)
//   0x006A1120 / RemoveThing 0x006A19B0, CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6, NGraphicAppearance::
//   CTCGraphicAppearance::SetScale 0x004BFA50, CTCShowableItem::OnShowEnded 0x005D8E50, CThing::Kill(bool) 0x004C9B80,
//   the defs' virtual slot 1 on release, free 0x00BFEA14.
// Layout: retail CTCInventoryWeapons (Ego_r.pdb - 0x18; the class is byte-packed): ParentThing +4, PActiveMeleeWeapon
//   +0x134, PActiveRangedWeapon +0x141 (13-byte pointer + def index + flag records). Retail CThing: interface bitset +0x20,
//   map +0x44, PTCGraphicAppearance +0x64, PBaseDef +0x70, dead bit +0x91. CWeaponDef SheatheObject +0x38 (PDB);
//   CTCGraphicAppearance MainGraphic.RenderSizeX +0x4c (landed layout). Interfaces: 0x46 carrying, 0x48 (the weapon's
//   sheathing TC), 0x1a showable item.
// Shape: /Os frame-pointer TU, no EH. The two GetPBaseDef() temporaries of the && condition are conditionally destroyed
//   through VC's flag local ([ebp-4]); the weapon def is declared after the primary-slot skip; the iterator, the push_back
//   argument and the carrying key share one slot; the sheathe pointer and the showable key share another.
#pragma optimize("s", on)
#pragma optimize("y", off)

extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14

class CThing;

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef typename _Vector_base<_Tp, _Alloc>::allocator_type allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void push_back(const _Tp& __x);                                                    // 0x00436C80 (out of line)
};

}

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

class CDefClassBase : public CDefPointee {
public:
    bool GetSubDef(CDefPointer<CWeaponDef>* out) const;  // 0x0042B1A6 (GetSubDef<CWeaponDef>)
};
class CThingBaseDef : public CDefClassBase { };

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
    unsigned char                            _pad_0x74[0x91 - 0x74];
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
};

class CBaseIntelligentPointer {
public:
    unsigned char _data[0x08];                           // opaque handle record (byte-aligned: the owner is packed)
    CThing* GetPItem() const;                            // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
    operator T*() const { return (T*)GetPItem(); }
};

// 0x004CC340 / 0x0040DCE0: the header template, visible here and out of line as in retail (VC7.1 then knows the
// argument is only read).
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && (thing->DeadFlags & 1) == 0;
}

class CTCCarrying {                                      // interface 0x46
public:
    bool IsCarryingThing(const CThing& thing) const;     // 0x006A1060
    bool IsCarriedThingInPrimarySlot(const CThing& thing) const;   // 0x006A1490
    CThing* GetCarriedThing(long index) const;           // 0x006A1120
    void RemoveThing(CThing& thing);                     // 0x006A19B0
};

class CTCShowableItem {                                  // interface 0x1a
public:
    void OnShowEnded();                                  // 0x005D8E50
};

class CTCInventoryWeapons {
public:
    void*                       __vftable;               // +0x00
    CThing*                     ParentThing;             // +0x04
    unsigned char               _pad_0x08[0x134 - 0x08];
    CIntelligentPointer<CThing> PActiveMeleeWeapon;      // +0x134
    unsigned char               _pad_0x13c[0x141 - 0x13c];   // ActiveMeleeWeaponDefIndex, MeleeWeaponCarriedOnLoad
    CIntelligentPointer<CThing> PActiveRangedWeapon;     // +0x141
    void RemoveWeaponsFromSheathes();
};

void CTCInventoryWeapons::RemoveWeaponsFromSheathes()
{
    CTCCarrying* carrying = 0;
    ParentThing->GetInterface(&carrying, 0x46);
    std::vector<CThing*> weapons;
    if (IS_THING_ALIVE(PActiveMeleeWeapon))
        weapons.push_back(PActiveMeleeWeapon.GetPItem());
    if (IS_THING_ALIVE(PActiveRangedWeapon))
        weapons.push_back(PActiveRangedWeapon.GetPItem());
    for (std::vector<CThing*>::iterator it = weapons.begin(); it != weapons.end(); ++it) {
        CThing* weapon = *it;
        if (!carrying->IsCarryingThing(*weapon) || !carrying->IsCarriedThingInPrimarySlot(*weapon)) {
            CDefPointer<CWeaponDef> weaponDef;
            if (weapon->GetPBaseDef() && weapon->GetPBaseDef()->GetSubDef(&weaponDef) && weapon->HasTC(0x48)) {
                CThing* sheathe = 0;
                long sheatheObject = weaponDef->SheatheObject;
                if (sheatheObject > 0)
                    sheathe = carrying->GetCarriedThing(sheatheObject);
                else if (sheatheObject < 0)
                    sheathe = weapon;
                if (IS_THING_ALIVE(sheathe)) {
                    sheathe->PTCGraphicAppearance->SetScale(sheathe->PTCGraphicAppearance->MainGraphic.RenderSizeX);
                    if (carrying->IsCarryingThing(*sheathe))
                        carrying->RemoveThing(*sheathe);
                    CTCShowableItem* showable;
                    if (sheathe->GetInterface(&showable, 0x1a))
                        showable->OnShowEnded();
                    else
                        sheathe->Kill(true);
                }
            }
        }
    }
}
