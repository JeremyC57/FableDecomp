// CTCInventoryWeapons::CheckForSoldWieldedWeapons() -- while a melee (then a ranged) weapon is wielded, re-arm from the first carried item (count > 0) whose weapon sub-def has a melee (non-melee) weapon class
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CB780, 500 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) +
//   ("s",on). Ego_r.pdb: ?CheckForSoldWieldedWeapons@CTCInventoryWeapons@@QAEXXZ (name per the catalog).
// Callees: IS_THING_ALIVE @ 0x005CC1B0 (`__fastcall`, const ref), CTCInventoryBase::GetAllItems(vector&) @ 0x005BF482,
//   CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex @ 0x009AD9E0 (hidden result),
//   CDefClassBase::GetSubDef @ 0x0042B1A6, IsMeleeWeaponClass @ 0x005DA510, SetThingAsActiveWeapon(long, CComponent*)
//   @ 0x005CB2B0, CRT free @ 0x00BFEA14 (the STLport vector's storage).
// Shape: a function-scope STLport vector<pair<long,long> > (12 bytes, zeroed start/finish/end) that GetAllItems fills (declared
//   only: the vector escapes, so its words are re-read each iteration); two `for (i < size())` loops, each `break`ing after the
//   re-arm. The temporary def pointer keeps the nulling forced-inline dtor (`add eax,4 ; dec [eax] ; cmp [eax],0`), the
//   weapon sub-def pointer an `if (p) p->Release()` dtor whose null test VC threads into the `weapon &&` test. GetSubDef has a
//   same-TU noinline stand-in (it only writes the out slot) so the sub-def is read once into esi.
// Layout: retail PActiveMeleeWeapon +0x134 / PActiveRangedWeapon +0x141 (packed, as the landed AugmentWeapon 0x005C44FA);
//   weapon def class +0x2C (retail offset).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* p);          // 0x00BFEA14
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
struct CWeaponDef : CDefPointeeBase {
    unsigned char _pad08[0x2c - 0x08];
    long WeaponClass;                           // +0x2c
};
struct CWeaponDefPointer {
    CWeaponDef* Object;
    CWeaponDefPointer() : Object(0) {}
    ~CWeaponDefPointer()
    {
        if (Object)
            Object->Release();
    }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CWeaponDefPointer* out);     // 0x0042B1A6
};
struct CDefClassPointer {                       // CDefPointer<CDefClassBase>: GetPDefFromGlobalIndex's by-value result
    CDefClassBase* Object;
    __forceinline ~CDefClassPointer()
    {
        if (Object) {
            CDefPointeeBase* p = Object;
            --p->m_refCount;
            if (p->m_refCount == 0)
                p->Destroy();
            Object = 0;
        }
    }
    CDefClassBase* operator->() const { return Object; }
};
struct CDefinitionManager {
    CDefClassPointer GetPDefFromGlobalIndex(long index);   // 0x009AD9E0
};
void* FindSubDef(const CDefClassBase* def);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CWeaponDefPointer* out)
{
    void* def = FindSubDef(this);
    if (!def)
        return false;
    out->Object = (CWeaponDef*)def;
    return true;
}
#pragma auto_inline(on)

struct CItemCount { long defIndex; long count; };
struct CItemVector {                            // STLport vector<pair<long,long> > (12 bytes)
    CItemCount* _M_start;
    CItemCount* _M_finish;
    CItemCount* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CItemVector()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return _M_finish - _M_start; }
    CItemCount& operator[](unsigned int n) { return *(_M_start + n); }
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { };
#pragma pack(pop)
class CThing;
class CComponent;
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x005CC1B0
bool __fastcall IsMeleeWeaponClass(long weaponClass);       // 0x005DA510

namespace NInventory {
struct CTCInventoryBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[0x134 - 0x08];
    CDefinitionManager* GetDefinitionManager() const;       // 0x006869D0 (CTCBase)
    void GetAllItems(CItemVector& items) const;             // 0x005BF482
};
}
struct CTCInventoryWeapons : NInventory::CTCInventoryBase {
    CIntelligentPointer<CThing> PActiveMeleeWeapon;         // +0x134
    long          ActiveMeleeWeaponDefIndex;                // +0x13C
    bool          MeleeWeaponCarriedOnLoad;                 // +0x140
    CIntelligentPointer<CThing> PActiveRangedWeapon;        // +0x141 (packed in retail)
    void SetThingAsActiveWeapon(long defIndex, CComponent* thing);   // 0x005CB2B0
    void CheckForSoldWieldedWeapons();
};

void CTCInventoryWeapons::CheckForSoldWieldedWeapons()
{
    CItemVector items;
    if (IS_THING_ALIVE(PActiveMeleeWeapon) || IS_THING_ALIVE(PActiveRangedWeapon))
        GetAllItems(items);
    if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
        for (unsigned int i = 0; i < items.size(); ++i) {
            if (items[i].count > 0) {
                CDefClassPointer def = GetDefinitionManager()->GetPDefFromGlobalIndex(items[i].defIndex);
                CWeaponDefPointer weapon;
                def->GetSubDef(&weapon);
                if (weapon.Object && IsMeleeWeaponClass(weapon.Object->WeaponClass)) {
                    SetThingAsActiveWeapon(items[i].defIndex, 0);
                    break;
                }
            }
        }
    }
    if (IS_THING_ALIVE(PActiveRangedWeapon)) {
        for (unsigned int i = 0; i < items.size(); ++i) {
            if (items[i].count > 0) {
                CDefClassPointer def = GetDefinitionManager()->GetPDefFromGlobalIndex(items[i].defIndex);
                CWeaponDefPointer weapon;
                def->GetSubDef(&weapon);
                if (weapon.Object && !IsMeleeWeaponClass(weapon.Object->WeaponClass)) {
                    SetThingAsActiveWeapon(items[i].defIndex, 0);
                    break;
                }
            }
        }
    }
}
