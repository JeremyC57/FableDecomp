// CTCInventoryWeapons::ConstructWeaponDescriptionWeight(CWideString& description, long defIndex, float damage) const -- appends the weapon-weight line (and heavy/light for melee weapons) to the item description
// Leaf: ConstructWeaponDescriptionWeight
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C770C, 519 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y",off) + ("s",on).
//   Ego_r.pdb ?ConstructWeaponDescriptionWeight@CTCInventoryWeapons@@QBEXAAVCWideString@@JM@Z (the float is unused).
// Callees: RefreshActiveWeaponData 0x005C30D3, CGameDefinitionManager::Get 0x0044C6B0 / GetDefGlobalIndexFromName 0x009AD390 /
//   GetDef<misc things def> 0x0042B312, CTCBase::GetDefinitionManager 0x006869D0, GetPDefFromGlobalIndex 0x009AD9E0,
//   CDefClassBase::GetSubDef 0x0042B1A6 (weapon def) / 0x0042AF9E (augmentation def), vector<unsigned long>::resize 0x004428F0,
//   free 0x00BFEA14, IsMeleeWeaponClass 0x005DA510, CDataBank::GetTextBySymbol 0x009C95E0, operator+ 0x0099BE70,
//   CWideString::operator+= 0x0099B8D0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Shape: GetDef / GetSubDef get same-TU noinline stand-ins (they only write the out slot), so the out slots are register-promoted
//   and their frame words reused (the augmentation pointer and the resize zero share the dead defIndex slot). The augmentation
//   copy is the landed CItem::AddItem 0x005BEC92 loop, written as a guarded do-while with the destination size read once (retail
//   keeps it in the dead argument slot); the resize count goes through a raw-pointer local so the zero temp is stored last.
// Layout: the local per-item data is 0x24 bytes (vector + three longs + bool + index -1 + one word the inline ctor leaves unset);
//   misc things def +0x3C weight-units string; weapon def class +0x2C; augmentation def slot count +0x28, slot values +0x40.
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
    CWideString& operator+=(const CWideString& other);  // 0x0099B8D0
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
namespace NGameText {
class CDataBank { public: const CWideString GetTextBySymbol(const CCharString& symbol) const; };
}
using NGameText::CDataBank;   // 0x009C95E0
class CMainGameComponent { public: unsigned char _pad[0x14]; CDataBank* TextBank; };   // +0x14
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
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
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
    T* Detach() { T* p = Object; Object = 0; return p; }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* p) : Object(p) { }
    __forceinline ~CDefHolder() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CDefBasePointer {
    CDefBase* Object;
    __forceinline ~CDefBasePointer()
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
struct CMiscThingsDef : CDefBase {
    unsigned char _pad08[0x3c - 0x08];
    CWideString WeightUnits;                            // +0x3c
};
class CWeaponDef : public CDefBase { public:
    unsigned char _pad08[0x2c - 0x08];
    long WeaponClass;                                   // +0x2c
};
class CAugmentationDef : public CDefBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long NumberOfSlots;                                 // +0x28
    unsigned char _pad2c[0x40 - 0x2c];
    vector<unsigned long> SlotValues;                   // +0x40
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef(CDefPointer<CWeaponDef>* out);       // 0x0042B1A6
    bool GetSubDef(CDefPointer<CAugmentationDef>& out); // 0x0042AF9E
};
void* FindDef_a();
void* FindDef_b();
void* FindDef_c();
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CWeaponDef>* out)
{
    void* def = FindDef_a();
    if (!def)
        return false;
    out->Object = (CWeaponDef*)def;
    return true;
}
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAugmentationDef>& out)
{
    void* def = FindDef_b();
    if (!def)
        return false;
    out.Object = (CAugmentationDef*)def;
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name, bool b);   // 0x009AD390
    bool GetDef(long index, CDefPointer<CMiscThingsDef>& out);         // 0x0042B312
};
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long index, CDefPointer<CMiscThingsDef>& out)
{
    void* def = FindDef_c();
    if (!def)
        return false;
    out.Object = (CMiscThingsDef*)def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    CDefBasePointer GetPDefFromGlobalIndex(long index);   // 0x009AD9E0
};
bool __fastcall IsMeleeWeaponClass(long weaponClass);    // 0x005DA510
struct CPerItemData {
    vector<unsigned long> SlotValues;                   // +0x00
    long A, B, C;                                       // +0x0C
    bool Flag;                                          // +0x18
    long Index;                                         // +0x1C
    long Extra;                                         // +0x20 (not set by the inline ctor)
    CPerItemData() : A(0), B(0), C(0), Flag(true), Index(-1) {}
};
class CTCInventoryWeapons { public:
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0
    void RefreshActiveWeaponData() const;               // 0x005C30D3
    void ConstructWeaponDescriptionWeight(CWideString& description, long defIndex, float damage) const;
};
void CTCInventoryWeapons::ConstructWeaponDescriptionWeight(CWideString& description, long defIndex, float damage) const
{
    RefreshActiveWeaponData();
    CDefPointer<CMiscThingsDef> misc;
    long miscIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_MISC_THINGS_DEF", true);
    CGameDefinitionManager::Get().GetDef(miscIndex, misc);
    CDefBasePointer def = GetDefinitionManager()->GetPDefFromGlobalIndex(defIndex);
    CWeaponDef* weaponDef;
    {
        CDefPointer<CWeaponDef> weapon;
        ((CDefClassBase*)def.Object)->GetSubDef(&weapon);
        weaponDef = weapon.Detach();
    }
    {
        CPerItemData data;
        {
            CDefPointer<CAugmentationDef> augmentation;
            if (((CDefClassBase*)def.Object)->GetSubDef(augmentation)) {
                CAugmentationDef* a = augmentation.Object;
                data.SlotValues.resize(a->NumberOfSlots, 0);
                unsigned long i = 0;
                if (i < augmentation->SlotValues.size()) {
                    unsigned long count = data.SlotValues.size();
                    do {
                        if (i >= count)
                            break;
                        data.SlotValues[i] = augmentation->SlotValues[i];
                        ++i;
                    } while (i < augmentation->SlotValues.size());
                }
            }
        }
        CDataBank* bank = g_pMainGameComponent->TextBank;
        if (IsMeleeWeaponClass(weaponDef->WeaponClass)) {
            description += bank->GetTextBySymbol("TEXT_GUI_MENU_WEAPON_WEIGHT") + misc->WeightUnits;
            if (weaponDef->WeaponClass == 2)
                description += bank->GetTextBySymbol("TEXT_GUI_MENU_HEAVY");
            else if (weaponDef->WeaponClass == 1)
                description += bank->GetTextBySymbol("TEXT_GUI_MENU_LIGHT");
        }
    }
    if (--weaponDef->m_refCount == 0)
        weaponDef->Destroy();
}