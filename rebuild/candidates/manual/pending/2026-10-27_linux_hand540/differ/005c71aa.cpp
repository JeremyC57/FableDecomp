// CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight(CWideString& description, long defIndex, float damage, bool fromDef) const -- appends "<augmentations><space>" and the weapon's augmentation names (from the def's initial augmentations or from the inventory item's per-item data), comma separated, or the "none" text; then a newline and the item's details
// Grade: DIFFER(680v680) -- residue: two scheduling swaps in the TEXT_GUI_MENU_NONE block only (retail push -1 / lea ecx,[ebp+14h] / push str and lea eax,[ebp-14h] / mov ecx,esi / push eax; ours the reverse order); lane gap: instruction scheduling in the none-text block | tried: ~20 none-block forms (found flag, goto, braces, named temp, c_str, else-arm, if<=0, helper, wide temp, ref, arrow) all inert | next: none known
// Leaf: ?ConstructWeaponDescriptionNoDamageNorWeight@CTCInventoryWeapons@@QBEXAAVCWideString@@JM_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C71AA, 680 bytes, `ret 10h`, ebp frame + size TU (optimize "y" off + "s" on, below).
// Identity: Ego_r.pdb ?ConstructWeaponDescriptionNoDamageNorWeight@CTCInventoryWeapons@@QBEXAAVCWideString@@JM_N@Z (one of the
//   ConstructWeaponDescription* family of CTCInventoryWeapons); `damage` is unused (its slot is reused for temporaries).
// Callees (PDB spellings, two sources each; see NOTES.md):
//   CTCInventoryWeapons::RefreshActiveWeaponData() const 0x005C30D3; CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0;
//   CGameDefinitionManager::Get() 0x0044C6B0 (static __fastcall); CDefinitionManager::GetGlobalIndexFromClassIndex(const
//   CCharString&, long) const 0x009AD390 (Ego_r donor order GetPDefClassInfo / GetGlobalIndexFromClassIndex /
//   GetNoInstantiatedDefsOfType = retail 0x009AD2E0 / 0x009AD390 / 0x009AD3B0, and the body returns the class's
//   index table entry); CDefinitionManager::GetDef<NUISystem::CUIMiscThingsDef> 0x0042B312; CTCBase::GetDefinitionManager()
//   const 0x006869D0 (protected); CDefinitionManager::GetPDefFromGlobalIndex(long) const 0x009AD9E0;
//   CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6 ('CWeaponDef' literal) and GetSubDef<CObjectAugmentationsDef> 0x0042AF9E
//   ('CObjectAugmentationsDef' literal); std::vector<long>::resize(size_type, const long&) 0x004428F0 (STLport 4.6);
//   NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) const 0x005BDF08 and ::GetPerItemDataForItem(long)
//   const 0x005BE8D3 (it forwards to CCategory::GetPerItemDataForItem 0x005BE8B1; donor order GetBoughtPrice /
//   GetSlotWithItemsOfType / GetPerItemDataForItem matches retail 0x005BE839 / 0x005BE85B / 0x005BE8B1);
//   NGameText::CDataBank::GetTextBySymbol(const CCharString&) const 0x009C95E0; operator+(const CWideString&, const
//   CWideString&) 0x0099BE70 (__fastcall); CWideString::operator+= 0x0099B8D0 / ~ 0x0099B510;
//   CTCObjectAugmentations::GetAugmentationName(long) 0x00765F90 (static __fastcall); CTCInventoryItem::GetDetails(long)
//   0x005D9630 (static __fastcall; Ego_r donor order CacheDef / GetDescription / GetDetails / GetEngineGraphic at the same
//   0x70 spacing as retail 0x005D94B0 / 0x005D95C0 / 0x005D9630 / 0x005D96A0); free 0x00BFEA14 (the local CPerItemData's
//   vector buffer).
// Globals: CMainGameComponent::Get()'s instance 0x013B86A0 (PTextBank +0x14, Ego_r); the CTCInventoryWeapons TU's
//   anonymous-namespace NEWLINE (CWideString L"\n") 0x013B8BA8 (FableWin.pdb dynamic initializer / atexit destructor for
//   'NEWLINE' between BANK_HANDLE and WEAPON_DESCRIPTION_STRING; retail 0x013B8BA4 / 0x013B8BA8 / 0x013B8BAC).
// Layout: CObjectAugmentationsDef NumberOfSlots +0x28, InitialAugmentations +0x40 (Ego_r +0x44 less 4 for the STLport map);
//   CUIMiscThingsDef SpaceSeparator +0x3C / CommaSeparator +0x40 (Ego_r +0x40 / +0x44 less 4, as TotalSpellsInContainer
//   +0x14C in the landed 0x005F694D); NInventory::CPerItemData as Ego_r with a 12-byte STLport vector.
// Shape: CDefPointer<const T> locals (misc, def, weapon; the CObjectAugmentationsDef pointer in the dead `damage` slot);
//   the def pointer from GetPDefFromGlobalIndex releases through the two-step count test and is nulled (as the landed
//   GetSubDef models do for the manager's result), the typed pointers through `if (--IVRefCount == 0) Delete()`.
//   STLport `operator[]` is `*(begin() + n)`: that keeps retail's `cmp [eax+esi*4],ebx ; mov edx,[eax+esi*4]` in the
//   name loop. The same-TU noinline GetDef / GetSubDef bodies are the landed retail bodies (0x0042B312 / 0x0042B1A6 /
//   0x0042AF9E), ported under their PDB template names; with only declarations VC spills the def pointers.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                  // 0x0099B6B0
    ~CWideString();                                    // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70

namespace std {
template <class T> class allocator { };
template <class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    T* begin() { return _M_start; }
    const T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
    void resize(unsigned int new_size, const T& x);    // 0x004428F0
    __forceinline void resize(unsigned int new_size) { resize(new_size, T()); }
};
}

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();                  // slot 0
    virtual void Delete() const;                       // slot 1
    mutable unsigned long IVRefCount;                  // +0x04
};
class CResource : public CIVCountedPointeeBase {       // retail slot order (vtable 0x01230BCC, FableWin.pdb names)
public:
    virtual bool UnloadResource();                     // slot 2
    virtual unsigned long GetUnloadableMemory() const; // slot 3
    virtual bool IsResourceLoaded() const;             // slot 4
    virtual void OnResourceLoaded();                   // slot 5
    virtual bool CanAbortResourceLoad(long frames) const;   // slot 6
    virtual void OnResourceLoadAborted();              // slot 7
    virtual unsigned long GetDebugMemoryInUse() const; // slot 8
    virtual void ResetDebugMemoryInUse();              // slot 9
    unsigned char _pad08[0x1c - 0x08];
};
class CDefPointeeBase : public CResource { };
template <class T> class CDefPointer;
class CDefClassBase;
template <> class CDefPointer<const CDefClassBase> {   // the manager's result: released through the two-step test, then nulled
public:
    CDefPointeeBase* Object;
    CDefPointer() { }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            CDefPointeeBase* p = Object;
            --p->IVRefCount;
            if (p->IVRefCount == 0)
                p->Delete();
            Object = 0;
        }
    }
    const CDefClassBase* operator->() const { return (const CDefClassBase*)Object; }
};
template <> class CDefPointer<CDefClassBase> {         // the protected overload's result (as the landed GetSubDef models)
public:
    CDefPointeeBase* Object;
    CDefPointer() { }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            CDefPointeeBase* p = Object;
            --p->IVRefCount;
            if (p->IVRefCount == 0)
                p->Delete();
            Object = 0;
        }
    }
};

template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer<CDefClassBase>& other);
    __forceinline ~CDefPointer() { if (Object) { if (--Object->IVRefCount == 0) Object->Delete(); } }
    T* operator->() const { return (T*)Object; }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->IVRefCount == 0)
                    Object->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};
class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;               // 0x009AD9E0
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);                           // 0x009ADA10
    friend class CDefClassBase;
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

class CDefString {
public:
    long Index;
};
class CDefClassBase : public CDefPointeeBase {
public:
    class CSubDefInfo {
    public:
        long DefIndex;                                 // +0x00
        long OriginalParentDefIndex;                   // +0x04
    };
    virtual CDefString GetInstantiationName() const;   // slot 10
    virtual void SetInstantiationName(CDefString name);    // slot 11
    virtual void AddSubDef(const CCharString& name, long defIndex, long parentIndex);   // slot 12
    virtual void RemoveSubDef(const CCharString& name);    // slot 13
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString& name);               // slot 15
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString& name) const;   // slot 14 (+0x38)
    CDefinitionManager* PDefManager;                   // +0x1C
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};
namespace NUISystem {
class CUIMiscThingsDef : public CDefClassBase {
public:
    unsigned char _pad20[0x3c - 0x20];
    CWideString SpaceSeparator;                        // +0x3C
    CWideString CommaSeparator;                        // +0x40
};
}
class CWeaponDef : public CDefClassBase { };
class CObjectAugmentationsDef : public CDefClassBase {
public:
    unsigned char _pad20[0x28 - 0x20];
    long NumberOfSlots;                                // +0x28
    unsigned char _pad2c[0x40 - 0x2c];
    std::vector<long> InitialAugmentations;            // +0x40
};

template <class T>
inline CDefPointer<T>::CDefPointer(const CDefPointer<CDefClassBase>& other) : Object(other.Object)
{
    if (Object)
        ++Object->IVRefCount;
}

// Same-TU bodies of the retail callees (the landed 0x0042B312 / 0x0042B1A6 / 0x0042AF9E bodies, PDB template names).
#pragma auto_inline(off)
template <>
__declspec(noinline) bool CDefinitionManager::GetDef<NUISystem::CUIMiscThingsDef>(long index, CDefPointer<const NUISystem::CUIMiscThingsDef>* out) const   // 0x0042B312
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.Object) {
        CDefPointer<const NUISystem::CUIMiscThingsDef>& o = *out;
        if (o.Object != def.Object) {
            if (o.Object) {
                if (--o.Object->IVRefCount == 0)
                    o.Object->Delete();
                o.Object = 0;
            }
            o.Object = def.Object;
            if (o.Object)
                ++o.Object->IVRefCount;
        }
        return true;
    }
    return false;
}
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer<const CWeaponDef>* out) const   // 0x0042B1A6
{
    const CSubDefInfo* info;
    {
        CCharString name("CWeaponDef", -1);
        info = GetPSubDefInfo(name);
    }
    if (info) {
        CDefPointer<const CWeaponDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));
        *out = result;
        return true;
    }
    return false;
}
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CObjectAugmentationsDef>(CDefPointer<const CObjectAugmentationsDef>* out) const   // 0x0042AF9E
{
    const CSubDefInfo* info;
    {
        CCharString name("CObjectAugmentationsDef", -1);
        info = GetPSubDefInfo(name);
    }
    if (info) {
        CDefPointer<const CObjectAugmentationsDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
class CMainGameComponent;
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0 (CMainGameComponent::Get()'s instance; descriptive)
class CMainGameComponent {
public:
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;   // +0x14
    static CMainGameComponent* __fastcall Get() { return g_pMainGameComponent; }
};
namespace NInventory {
class CPerItemData {
public:
    std::vector<long> Augmentations;                   // +0x00
    long  NumberOfKills;                               // +0x0C
    long  TimeWielded;                                 // +0x10
    long  BestTrophyWitnessesAheadToDate;              // +0x14
    bool  QuestPersistent;                             // +0x18
    long  QuestPersistentRefCount;                     // +0x1C
    float BestFishWeight;                              // +0x20 (not set by the inline constructor)
    CPerItemData()
        : NumberOfKills(0), TimeWielded(0), BestTrophyWitnessesAheadToDate(0), QuestPersistent(true),
          QuestPersistentRefCount(-1) { }
};
}
class CTCObjectAugmentations {
public:
    static CWideString __fastcall GetAugmentationName(long type);   // 0x00765F90
};
class CTCInventoryItem {
public:
    static CWideString __fastcall GetDetails(long defIndex);        // 0x005D9630
};
namespace {
CWideString NEWLINE(L"\n");                           // 0x013B8BA8
}

class CTCBase : public CTCBaseLayout {
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;     // 0x006869D0
};
namespace NInventory {
class CTCInventoryBase : public CTCBase {
public:
    long GetNumberOfItemsOfTypeInInventory(long defIndex) const;    // 0x005BDF08
    const CPerItemData* GetPerItemDataForItem(long defIndex) const; // 0x005BE8D3
};
}
class CTCInventoryWeapons : public NInventory::CTCInventoryBase {
public:
    void RefreshActiveWeaponData() const;              // 0x005C30D3
    void ConstructWeaponDescriptionNoDamageNorWeight(CWideString& description, long defIndex, float damage, bool fromDef) const;
};

void CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight(CWideString& description, long defIndex, float damage, bool fromDef) const
{
    RefreshActiveWeaponData();
    CDefPointer<const NUISystem::CUIMiscThingsDef> misc;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &misc);
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(defIndex);
    CDefPointer<const CWeaponDef> weapon;
    def->GetSubDef(&weapon);
    NInventory::CPerItemData data;
    const std::vector<long>* slots;
    if (fromDef) {
        CDefPointer<const CObjectAugmentationsDef> augmentations;
        if (def->GetSubDef(&augmentations)) {
            data.Augmentations.resize(augmentations->NumberOfSlots);
            unsigned int i = 0;
            if (i < augmentations->InitialAugmentations.size()) {
                unsigned int count = data.Augmentations.size();
                do {
                    if (i >= count)
                        break;
                    data.Augmentations[i] = augmentations->InitialAugmentations[i];
                    ++i;
                } while (i < augmentations->InitialAugmentations.size());
            }
        }
        slots = &data.Augmentations;
    } else {
        if (GetNumberOfItemsOfTypeInInventory(defIndex) == 0)
            return;
        slots = &GetPerItemDataForItem(defIndex)->Augmentations;
    }
    NGameText::CDataBank* bank = CMainGameComponent::Get()->PTextBank.operator->();
    description += bank->GetTextBySymbol("TEXT_GUI_MENU_AUGMENTATIONS") + misc->SpaceSeparator;
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
                    description += CTCObjectAugmentations::GetAugmentationName((*slots)[i]);
                    if (i < last)
                        description += misc->CommaSeparator;
                }
            }
        }
    }
    if (none)
        description += bank->GetTextBySymbol("TEXT_GUI_MENU_NONE");
    description += NEWLINE;
    description += CTCInventoryItem::GetDetails(defIndex);
}
