// CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight(CWideString& description, long defIndex, float damage, bool fromDef) const -- appends the augmentation list (from the def or from the inventory copy), a separator and the item description
// Grade: DIFFER(681v680) -- one addressing-mode choice: retail tests cmp [eax+esi*4],ebx and reloads [eax+esi*4]; ours materialises lea eax,[eax+esi*4]; plus two push/lea schedule swaps
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString& operator+=(const CWideString& other);  // 0x0099B8D0
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
struct CDataBank { CWideString GetTextBySymbol(const CCharString& symbol); };   // 0x009C95E0
struct CMainGameComponent { unsigned char _pad[0x14]; CDataBank* TextBank; };   // +0x14
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
template <class T> struct CDefPointer {
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
    CWideString Colon;                                  // +0x3c
    CWideString Separator;                              // +0x40
};
struct CWeaponDef : CDefBase {
    unsigned char _pad08[0x2c - 0x08];
    long WeaponClass;                                   // +0x2c
};
struct CAugmentationDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    long NumberOfSlots;                                 // +0x28
    unsigned char _pad2c[0x40 - 0x2c];
    vector<long> SlotValues;                            // +0x40
};
struct CDefClassBase : CDefBase {
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
struct CGameDefinitionManager {
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
struct CDefinitionManager {
    CDefBasePointer GetPDefFromGlobalIndex(long index);   // 0x009AD9E0
};
CWideString __fastcall GetAugmentationName(long type);   // 0x00765F90
CWideString __fastcall GetItemDefDescription(long defIndex);   // 0x005D9630
extern CWideString g_NewLine;                           // 0x013B8BA8
struct CPerItemData {
    vector<long> SlotValues;                            // +0x00
    long A, B, C;                                       // +0x0C
    bool Flag;                                          // +0x18
    long Index;                                         // +0x1C
    long Extra;                                         // +0x20 (not set by the inline ctor)
    CPerItemData() : A(0), B(0), C(0), Flag(true), Index(-1) {}
};
struct CTCInventoryWeapons {
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0
    void RefreshActiveWeaponData() const;               // 0x005C30D3
    long GetNumberOfItemsOfTypeInInventory(long defIndex) const;   // 0x005BDF08 (CTCInventoryBase)
    const vector<long>* GetItemSlotValues(long defIndex) const;    // 0x005BE8D3 (CTCInventoryBase; ICF-shared body)
    void ConstructWeaponDescriptionNoDamageNorWeight(CWideString& description, long defIndex, float damage, bool fromDef) const;
};
void CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight(CWideString& description, long defIndex, float damage, bool fromDef) const
{
    RefreshActiveWeaponData();
    CDefPointer<CMiscThingsDef> misc;
    long miscIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("UI_MISC_THINGS_DEF", true);
    CGameDefinitionManager::Get().GetDef(miscIndex, misc);
    CDefBasePointer def = GetDefinitionManager()->GetPDefFromGlobalIndex(defIndex);
    CDefPointer<CWeaponDef> weapon;
    ((CDefClassBase*)def.Object)->GetSubDef(&weapon);
    CPerItemData data;
    const vector<long>* slots;
    if (fromDef) {
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
        slots = &data.SlotValues;
    } else {
        if (GetNumberOfItemsOfTypeInInventory(defIndex) == 0)
            return;
        slots = GetItemSlotValues(defIndex);
    }
    CDataBank* bank = g_pMainGameComponent->TextBank;
    description += bank->GetTextBySymbol("TEXT_GUI_MENU_AUGMENTATIONS") + misc->Colon;
    bool none = true;
    if (slots->_M_start != slots->_M_finish) {
        long last = -1;
        long size = slots->size();
        long used = 0;
        for (long i = 0; i < size; ++i) {
            if ((*slots)[i] > 0) {
                ++used;
                last = i;
            }
        }
        if (used > 0) {
            none = false;
            for (long i = 0; i < size; ++i) {
                if ((*slots)[i] > 0) {
                    description += GetAugmentationName(slots->_M_start[i]);
                    if (i < last)
                        description += misc->Separator;
                }
            }
        }
    }
    if (none)
        description += bank->GetTextBySymbol("TEXT_GUI_MENU_NONE");
    description += g_NewLine;
    description += GetItemDefDescription(defIndex);
}
