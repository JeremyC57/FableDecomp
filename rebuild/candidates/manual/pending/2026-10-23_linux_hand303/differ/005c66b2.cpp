// CTCInventoryWeapons::ConstructWeaponItemEntries(...) -- splits the weapon items into two lists by class and builds them after the wield entry
// Grade: DIFFER(695v760) -- retail caches this/component pointers in callee-saved registers across many calls (whole-function register caching lane gap); structure otherwise matches
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
namespace std {
template <class A, class B> struct pair { A first; B second; };
template <class T> struct allocator {};
template <class T, class Al = allocator<T> > struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    T& operator[](unsigned int i) { return _M_start[i]; }
    void push_back(const T& v);                         // 0x005947B6
};
}
namespace NUISystem { struct CList; struct CComponent; struct CManager; struct CViewport; struct CUIMiscThingsDef; }
using namespace NUISystem;
template <class T> struct CCountedPointer {
    T* Object;
    ~CCountedPointer();                                 // 0x004291DE
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();
    long m_refCount;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
template <class T> struct CDefRef {
    T* Object;
    CDefRef() : Object(0) { }
    __forceinline ~CDefRef() { if (--Object->m_refCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CWeaponDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    long WeaponType;                                    // +0x28
    long WeaponClass;                                   // +0x2c
};
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefRef<CWeaponDef>* out);           // 0x0042B1A6
};
void* FindDef_a();
void* FindDef_b();
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefRef<CWeaponDef>* out)
{
    void* def = FindDef_a();
    if (!def)
        return false;
    out->Object = (CWeaponDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_b();
    if (!def)
        return false;
    out->Object = (CDefClassBase*)def;
    return true;
}
#pragma auto_inline(on)
bool __fastcall IsMeleeWeaponClass(long weaponClass);    // 0x005DA510
typedef std::vector<std::pair<long, long> > CItemVector;
struct CTCInventoryWeapons {
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0
    CCountedPointer<CComponent> ConstructWieldEntry(bool melee, CViewport* viewport);   // 0x005C323A
    void FinalConstructWeaponItemEntries(CItemVector& items, CList* list, CCountedPointer<CComponent>& a, CComponent* parent,
        CManager* manager, CViewport* viewport, CDefPointer<const CUIMiscThingsDef>& misc, float width,
        CCountedPointer<CComponent>& b, CCountedPointer<CComponent>& wield, unsigned long first);   // 0x005C3E91
    void ConstructWeaponItemEntries(CItemVector& items, CList* list, CCountedPointer<CComponent>& a, CComponent* parent,
        CManager* manager, CViewport* viewport, CDefPointer<const CUIMiscThingsDef>& misc, float width,
        CCountedPointer<CComponent>& b);
};
void CTCInventoryWeapons::ConstructWeaponItemEntries(CItemVector& items, CList* list, CCountedPointer<CComponent>& a,
    CComponent* parent, CManager* manager, CViewport* viewport, CDefPointer<const CUIMiscThingsDef>& misc, float width,
    CCountedPointer<CComponent>& b)
{
    if (items.empty())
        return;
    CDefPointer<CDefClassBase> def;
    GetDefinitionManager()->GetDef(items[0].first, &def);
    CDefRef<CWeaponDef> weapon;
    def->GetSubDef(&weapon);
    bool melee = IsMeleeWeaponClass(weapon->WeaponClass);
    CItemVector second;
    CItemVector first;
    if (melee) {
        for (unsigned int i = 0; i < items.size(); ++i) {
            CDefPointer<CDefClassBase> itemDef;
            GetDefinitionManager()->GetDef(items[i].first, &itemDef);
            CDefRef<CWeaponDef> itemWeapon;
            itemDef->GetSubDef(&itemWeapon);
            if (itemWeapon->WeaponClass == 1)
                first.push_back(items[i]);
            else
                second.push_back(items[i]);
        }
        CCountedPointer<CComponent> wield = ConstructWieldEntry(true, viewport);
        FinalConstructWeaponItemEntries(first, list, a, parent, manager, viewport, misc, width, b, wield, 0);
        FinalConstructWeaponItemEntries(second, list, a, parent, manager, viewport, misc, width, b, wield, first.size());
    } else {
        for (unsigned int i = 0; i < items.size(); ++i) {
            CDefPointer<CDefClassBase> itemDef;
            GetDefinitionManager()->GetDef(items[i].first, &itemDef);
            CDefRef<CWeaponDef> itemWeapon;
            itemDef->GetSubDef(&itemWeapon);
            if (itemWeapon->WeaponType == 3)
                second.push_back(items[i]);
            else
                first.push_back(items[i]);
        }
        CCountedPointer<CComponent> wield = ConstructWieldEntry(false, viewport);
        FinalConstructWeaponItemEntries(second, list, a, parent, manager, viewport, misc, width, b, wield, 0);
        FinalConstructWeaponItemEntries(first, list, a, parent, manager, viewport, misc, width, b, wield, first.size());
    }
}
