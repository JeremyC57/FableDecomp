// CTCInventoryWeapons::ConstructWeaponDescriptionWeight(CWideString& description, long defIndex, float damage) const -- appends the weapon-weight line (and heavy/light for melee weapons) to the item description
// Leaf: ConstructWeaponDescriptionWeight
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C770C, 519 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y",off) + ("s",on).
//   Ego_r.pdb ?ConstructWeaponDescriptionWeight@CTCInventoryWeapons@@QBEXAAVCWideString@@JM@Z (the float is unused).
// Callees: RefreshActiveWeaponData 0x005C30D3, CGameDefinitionManager::Get 0x0044C6B0 / GetDefGlobalIndexFromName 0x009AD390 /
//   GetDef<NUISystem::CUIMiscThingsDef> const 0x0042B312, CTCBase::GetDefinitionManager const 0x006869D0, GetPDefFromGlobalIndex
//   const 0x009AD9E0, CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6 / GetSubDef<CObjectAugmentationsDef> 0x0042AF9E, vector<unsigned
//   long>::resize 0x004428F0, free 0x00BFEA14, CTCWeapon::IsMeleeWeaponClass 0x005DA510, CDataBank::GetTextBySymbol 0x009C95E0, operator+ 0x0099BE70,
//   CWideString::operator+= 0x0099B8D0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Shape: GetDef / GetSubDef get same-TU noinline stand-ins (they only write the out slot), so the out slots are register-promoted
//   and their frame words reused (the augmentation pointer and the resize zero share the dead defIndex slot). The augmentation
//   copy is the landed CItem::AddItem 0x005BEC92 loop, written as a guarded do-while with the destination size read once (retail
//   keeps it in the dead argument slot); the resize count goes through a raw-pointer local so the zero temp is stored last.
// Layout: the local per-item data is 0x24 bytes (vector + three longs + bool + index -1 + one word the inline ctor leaves unset);
//   misc things def +0x3C weight-units string; weapon def class +0x2C; augmentation def slot count +0x28, slot values +0x40.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    const CWideString& operator+=(const CWideString& other);  // 0x0099B8D0
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
namespace NGameText {
class CDataBank { public: const CWideString GetTextBySymbol(const CCharString& symbol) const; };
}
using NGameText::CDataBank;   // 0x009C95E0
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public: unsigned char _pad[0x14]; CDataBank* TextBank; };   // +0x14
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
    void resize(unsigned long count, const T& value);   // 0x004428F0
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();
    long m_refCount;
};
template <class T> class CDefPointer { public:
    CDefBase* Object;                                   // the pointee as its non-const base (engine/CDefPointer.h)
    CDefPointer() : Object(0) { }
    FABLE_FORCEINLINE ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return static_cast<T*>(Object); }
    CDefBase* Detach() { CDefBase* p = Object; Object = 0; return p; }
};
class CDefClassBase;
template <> class CDefPointer<const CDefClassBase> { public:   // the manager's by-value def reference (released and cleared by its destructor)
    CDefBase* Object;
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (Object) {
            CDefBase* p = Object;
            --p->m_refCount;
            if (p->m_refCount == 0)
                p->Destroy();
            Object = 0;
        }
    }
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // GetSubDef<CWeaponDef> 0x0042B1A6 / <CObjectAugmentationsDef> 0x0042AF9E (Ego_r / FableWin templates)
};
namespace NUISystem {
class CUIMiscThingsDef : public CDefClassBase { public:
    unsigned char _pad08[0x3c - 0x08];
    CWideString WeightUnits;                            // +0x3c
};
}
class CWeaponDef : public CDefClassBase { public:
    unsigned char _pad08[0x2c - 0x08];
    long Class;                                         // +0x2c EWeaponClass
};
class CObjectAugmentationsDef : public CDefClassBase { public:   // Ego_r.pdb layout, retail STLport 12-byte map
    unsigned char _pad08[0x28 - 0x08];
    long NumberOfSlots;                                 // +0x28
    unsigned char _pad2c[0x40 - 0x2c];
    vector<unsigned long> InitialAugmentations;         // +0x40 (PDB +0x44)
};
void* FindDef_a();
void* FindDef_b();
void* FindDef_c();
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer<const CWeaponDef>* out) const
{
    void* def = FindDef_a();
    if (!def)
        return false;
    out->Object = (CDefBase*)def;
    return true;
}
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CObjectAugmentationsDef>(CDefPointer<const CObjectAugmentationsDef>* out) const
{
    void* def = FindDef_b();
    if (!def)
        return false;
    out->Object = (CDefBase*)def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(CCharString const&a0, long a1) const;   // 0x009AD390 (Ego_r.pdb ?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z)
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0 (Ego_r.pdb QBE)
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042B312 GetDef<NUISystem::CUIMiscThingsDef> (FableWin template)
};
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefinitionManager::GetDef<NUISystem::CUIMiscThingsDef>(long index, CDefPointer<const NUISystem::CUIMiscThingsDef>* out) const
{
    void* def = FindDef_c();
    if (!def)
        return false;
    out->Object = (CDefBase*)def;
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public:   // RTTI CGameDefinitionManager : CDefinitionManager
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
enum EWeaponClass { };
class CTCWeapon { public:
    static bool __fastcall IsMeleeWeaponClass(EWeaponClass weaponClass);   // 0x005DA510 (Ego_r.pdb SI)
};
struct CPerItemData {
    vector<unsigned long> SlotValues;                   // +0x00
    long A, B, C;                                       // +0x0C
    bool Flag;                                          // +0x18
    long Index;                                         // +0x1C
    long Extra;                                         // +0x20 (not set by the inline ctor)
    CPerItemData() : A(0), B(0), C(0), Flag(true), Index(-1) {}
};
class CTCBase { public:                                 // the component base (members this row reads)
    virtual ~CTCBase();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
class CTCInventoryWeapons : public CTCBase { public:
    void RefreshActiveWeaponData() const;               // 0x005C30D3
    void ConstructWeaponDescriptionWeight(CWideString& description, long defIndex, float damage) const;
};
void CTCInventoryWeapons::ConstructWeaponDescriptionWeight(CWideString& description, long defIndex, float damage) const
{
    RefreshActiveWeaponData();
    CDefPointer<const NUISystem::CUIMiscThingsDef> misc;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", true);
    CGameDefinitionManager::Get().GetDef(miscIndex, &misc);
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(defIndex);
    CWeaponDef* weaponDef;
    {
        CDefPointer<const CWeaponDef> weapon;
        ((CDefClassBase*)def.Object)->GetSubDef(&weapon);
        weaponDef = static_cast<CWeaponDef*>(weapon.Detach());
    }
    {
        CPerItemData data;
        {
            CDefPointer<const CObjectAugmentationsDef> augmentation;
            if (((CDefClassBase*)def.Object)->GetSubDef(&augmentation)) {
                CObjectAugmentationsDef* a = static_cast<CObjectAugmentationsDef*>(augmentation.Object);
                data.SlotValues.resize(a->NumberOfSlots, 0);
                unsigned long i = 0;
                if (i < augmentation->InitialAugmentations.size()) {
                    unsigned long count = data.SlotValues.size();
                    do {
                        if (i >= count)
                            break;
                        data.SlotValues[i] = augmentation->InitialAugmentations[i];
                        ++i;
                    } while (i < augmentation->InitialAugmentations.size());
                }
            }
        }
        CDataBank* bank = CMainGameComponent::Get()->TextBank;
        if (CTCWeapon::IsMeleeWeaponClass((EWeaponClass)weaponDef->Class)) {
            description += bank->GetTextBySymbol("TEXT_GUI_MENU_WEAPON_WEIGHT") + misc->WeightUnits;
            if (weaponDef->Class == 2)
                description += bank->GetTextBySymbol("TEXT_GUI_MENU_HEAVY");
            else if (weaponDef->Class == 1)
                description += bank->GetTextBySymbol("TEXT_GUI_MENU_LIGHT");
        }
    }
    if (--weaponDef->m_refCount == 0)
        weaponDef->Destroy();
}