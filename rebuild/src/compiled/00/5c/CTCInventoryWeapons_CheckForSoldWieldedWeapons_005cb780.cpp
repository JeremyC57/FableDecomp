// CTCInventoryWeapons::CheckForSoldWieldedWeapons() -- when the wielded melee (then ranged) weapon is gone (IS_THING_DEAD: null or dead, e.g. sold), re-arm from the first carried item (count > 0) whose weapon sub-def has a melee (non-melee) weapon class
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CB780, 500 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) +
//   ("s",on). Ego_r.pdb: ?CheckForSoldWieldedWeapons@CTCInventoryWeapons@@QAEXXZ (name per the catalog).
// Callees: IS_THING_DEAD<CIntelligentPointer<CThing> > @ 0x005CC1B0 (`__fastcall`, const ref; true for a null or dead thing),
//   NInventory::CTCInventoryBase::GetAllItems(vector&) @ 0x005BF482, CTCBase::GetDefinitionManager const @ 0x006869D0,
//   CDefinitionManager::GetPDefFromGlobalIndex const @ 0x009AD9E0 (hidden result), CDefClassBase::GetSubDef<CWeaponDef> const @ 0x0042B1A6,
//   CTCWeapon::IsMeleeWeaponClass @ 0x005DA510, SetThingAsActiveWeapon(long, NUISystem::CComponent*)
//   @ 0x005CB2B0, CRT free @ 0x00BFEA14 (the STLport vector's storage).
// Shape: a function-scope STLport vector<pair<long,long> > (12 bytes, zeroed start/finish/end) that GetAllItems fills (declared
//   only: the vector escapes, so its words are re-read each iteration); two `for (i < size())` loops, each `break`ing after the
//   re-arm. The temporary def pointer keeps the nulling forced-inline dtor (`add eax,4 ; dec [eax] ; cmp [eax],0`), the
//   weapon sub-def pointer an `if (p) p->Release()` dtor whose null test VC threads into the `weapon &&` test. GetSubDef has a
//   same-TU noinline stand-in (it only writes the out slot) so the sub-def is read once into esi.
// Layout: retail PActiveMeleeWeapon +0x134 / PActiveRangedWeapon +0x141 (packed, as the landed AugmentWeapon 0x005C44FA);
//   weapon def class +0x2C (retail offset).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* p);          // 0x00BFEA14
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:  // the weapon sub-def out pointer (released by its destructor)
    CDefPointeeBase* Object;                    // the pointee as its non-const base (engine/CDefPointer.h)
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return static_cast<T*>(Object); }
};
class CDefClassBase;
template <> class CDefPointer<const CDefClassBase> { public:   // GetPDefFromGlobalIndex's by-value result (released and cleared)
    CDefPointeeBase* Object;
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (Object) {
            CDefPointeeBase* p = Object;
            --p->m_refCount;
            if (p->m_refCount == 0)
                p->Destroy();
            Object = 0;
        }
    }
    CDefClassBase* operator->() const { return (CDefClassBase*)Object; }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0042B1A6 GetSubDef<CWeaponDef> (Ego_r / FableWin template)
};
class CWeaponDef : public CDefClassBase { public:
    unsigned char _pad08[0x2c - 0x08];
    long Class;                                 // +0x2c EWeaponClass
};
class CDefinitionManager { public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0 (Ego_r.pdb QBE)
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
void* FindSubDef(const CDefClassBase* def);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindSubDef(this);
    if (!def)
        return false;
    out->Object = (CDefPointeeBase*)def;
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)
class CThing;
namespace NUISystem { class CComponent; }
template <class T> bool __fastcall IS_THING_DEAD(const T& thing);   // 0x005CC1B0 IS_THING_DEAD<CIntelligentPointer<CThing> > (Ego_r.pdb 0x0062E159)
enum EWeaponClass { };
class CTCWeapon { public:
    static bool __fastcall IsMeleeWeaponClass(EWeaponClass weaponClass);   // 0x005DA510 (Ego_r.pdb SI)
};

class CTCBase { public:                                     // the component base (members this row reads)
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
namespace NInventory {
class CTCInventoryBase : public CTCBase { public:
    unsigned char _pad08[0x134 - 0x08];
    void GetAllItems(CItemVector& items) const;             // 0x005BF482
};
}
class CTCInventoryWeapons : public NInventory::CTCInventoryBase { public:
    CIntelligentPointer<CThing> PActiveMeleeWeapon;         // +0x134
    long          ActiveMeleeWeaponDefIndex;                // +0x13C
    bool          MeleeWeaponCarriedOnLoad;                 // +0x140
    CIntelligentPointer<CThing> PActiveRangedWeapon;        // +0x141 (packed in retail)
    void SetThingAsActiveWeapon(long defIndex, NUISystem::CComponent* component);   // 0x005CB2B0 (Ego_r/FableWin ?SetThingAsActiveWeapon@CTCInventoryWeapons@@QAEXJPAVCComponent@NUISystem@@@Z)
    void CheckForSoldWieldedWeapons();
};

void CTCInventoryWeapons::CheckForSoldWieldedWeapons()
{
    CItemVector items;
    if (IS_THING_DEAD(PActiveMeleeWeapon) || IS_THING_DEAD(PActiveRangedWeapon))
        GetAllItems(items);
    if (IS_THING_DEAD(PActiveMeleeWeapon)) {
        for (unsigned int i = 0; i < items.size(); ++i) {
            if (items[i].count > 0) {
                CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(items[i].defIndex);
                CDefPointer<const CWeaponDef> weapon;
                def->GetSubDef(&weapon);
                if (weapon.Object && CTCWeapon::IsMeleeWeaponClass((EWeaponClass)weapon->Class)) {
                    SetThingAsActiveWeapon(items[i].defIndex, 0);
                    break;
                }
            }
        }
    }
    if (IS_THING_DEAD(PActiveRangedWeapon)) {
        for (unsigned int i = 0; i < items.size(); ++i) {
            if (items[i].count > 0) {
                CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(items[i].defIndex);
                CDefPointer<const CWeaponDef> weapon;
                def->GetSubDef(&weapon);
                if (weapon.Object && !CTCWeapon::IsMeleeWeaponClass((EWeaponClass)weapon->Class)) {
                    SetThingAsActiveWeapon(items[i].defIndex, 0);
                    break;
                }
            }
        }
    }
}