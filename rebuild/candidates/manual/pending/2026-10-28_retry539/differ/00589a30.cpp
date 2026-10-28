// CTCInventory::OpenPCMapMenu() -- switches the PC inventory to the map screen (6): finds the "PC_MAP_LIST" component or builds it through the hero's CTCInventoryMap, then hooks its scroll and back/close clickables to the UI manager's input
// Grade: DIFFER(549v549) -- residue (2 mnemonic diffs): after the inline strcmp's repe cmpsb, retail reloads esi (this) before edi (the index) and ours reloads edi first; lane gap: reload order | tried: real names (CTCInventoryMap::ConstructPCMapList(bool, bool*) 0x0060EEB0 -- ret 8, not ConstructMapList 0x0060C900; CBasicString<char> with the inline CCharString::operator==; std::bitset<274>::test; CVectorMap LowerBound; CURRENT_OPEN_MENU at +0x21C), the real same-TU STLport push_back / _M_insert_overflow bodies, CThing::GetTC with a named key, about 20 operator==/strcmp/return forms (inert); 1 RTM grade this pass | next: none in source
// Leaf: ?OpenPCMapMenu@CTCInventory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00589A30, 549 bytes, `ret`; size TU without a frame pointer
//   (`optimize("s", on)`, as the landed ConstructStyleCardsScreenPC 0x0058A729 and CTCInventory ctor 0x00589C55).
// Identity: Ego_r.pdb / FableWin.pdb ?OpenPCMapMenu@CTCInventory@@QAEXXZ; the string "PC_MAP_LIST" (0x01250DC0) and the
//   ConstructPCMapList call.
// Callees: DeleteMenuComponents 0x00587A41; NUISystem::CManager::GetInstance 0x0041E5F2 (static __fastcall; its ctor stores
//   the CManager vtable 0x01230134; the first call's result is unused); CComponent::GetName (slot 84, CCharString by value),
//   the inline CCharString::operator==(const char*) over CBasicString<char>::operator==(const char*) const 0x004115A0 and
//   an intrinsic strcmp, ~CCharString 0x0099EAE0; std::bitset<274>::test 0x00410DE0 and CVectorMap<ETCInterfaceType,
//   CTCBase*, ..>::LowerBound 0x0040F020 (inline CThing::GetTC for interface 0x1B, CTCInventoryMap);
//   CTCInventoryMap::ConstructPCMapList(bool, bool*) 0x0060EEB0 (Ego_r: the PC sibling right after ConstructMapList, which
//   is retail 0x0060C900, `ret 0Ch`; 0x0060EEB0 ends `ret 8`); CComponent::Initialise (slot 43);
//   std::vector<CComponent*>::push_back 0x00486024; CComponent::FindChild(CCharString) (slot 3) with CCharString(const
//   char*, long) 0x0099EBF0; CObserver::ObserveEvent (observer at +4, slot 3) with event 0x19; CObservable::AddObserver
//   (CManager vtable slot 2); CComponent::ChangeState(unsigned long) (slot 48) with 5.
// Layout: CTCBase::ParentThing +0x04, CTCInventoryBase::Components +0x0C (vector<CComponent*>), CTCInventory::CurrentOpenMenu
//   +0x21C (engine/CTCInventory.h names; local model: the generated header leaves +0x04/+0x0C as pads); CThing
//   UsingComponents +0x20 (std::bitset<274>), Components +0x44 (CVectorMap, 0x10 bytes).
#pragma optimize("s", on)
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
template <class T> class CBasicString {
public:
    T*            PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const T* text) const;            // 0x004115A0
};
class CCharStringData {
public:
    CBasicString<char> Data;
    long               NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                   // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);
namespace std {
struct __true_type { };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
    void push_back(const _Tp& __x);                   // 0x00486024 (CComponent*)
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend = false);   // 0x00485DBC
};
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;              // 0x00410DE0 (bitset<274>)
};
}

namespace NUISystem {
enum EEvent { EVENT_25 = 0x19 };
class CObserver {                                     // at +4 of every component
public:
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(EEvent event);          // slot 3 (+0x0C)
};
class CObservable {                                   // CManager vtable 0x01230134
public:
    virtual void b00(); virtual void b01();
    virtual void AddObserver(CObserver* observer);    // slot 2 (+0x08) 0x0055CA40
};
#define V10(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3(); virtual void n##4(); \
    virtual void n##5(); virtual void n##6(); virtual void n##7(); virtual void n##8(); virtual void n##9();
class CUIInterface {
public:
    virtual void u00(); virtual void u01(); virtual void u02();
};
class CComponent : public CUIInterface, public CObserver {   // RTTI: CUIInterface@0, CObserver@4, CActionParam@24
public:
    virtual CComponent* FindChild(CCharString name);  // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    V10(v1) V10(v2) V10(v3)
    virtual void v40(); virtual void v41(); virtual void v42();
    virtual void Initialise();                        // slot 43 (+0xAC)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void ChangeState(unsigned long state);    // slot 48 (+0xC0)
    virtual void v49();
    V10(v5) V10(v6) V10(v7)
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual CCharString GetName();                    // slot 84 (+0x150)
};
class CManager : public CObservable {
public:
    static CManager* __fastcall GetInstance();        // 0x0041E5F2
};
}

// Same-TU bodies of the real retail push_back 0x00486024 and _M_insert_overflow 0x00485DBC (STLport 4.6), kept out of line:
// VC7.1 then sees that the address of the pushed local does not escape (the landed 0x0047F30F model, chunk 537).
#pragma auto_inline(off)
template <> __declspec(noinline) void std::vector<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >::_M_insert_overflow(NUISystem::CComponent** __position, NUISystem::CComponent* const& __x, const std::__true_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = this->_M_finish - this->_M_start;
    const unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    NUISystem::CComponent** __new_start = __len != 0 ? (NUISystem::CComponent**)malloc(__len * sizeof(NUISystem::CComponent*)) : 0;
    NUISystem::CComponent** __new_finish = (NUISystem::CComponent**)((char*)memmove(__new_start, this->_M_start, (char*)__position - (char*)this->_M_start) + ((char*)__position - (char*)this->_M_start));
    for (unsigned int __n = __fill_len; __n > 0; --__n, ++__new_finish)
        *__new_finish = __x;
    if (!__atend)
        __new_finish = (NUISystem::CComponent**)((char*)memmove(__new_finish, __position, (char*)this->_M_finish - (char*)__position) + ((char*)this->_M_finish - (char*)__position));
    if (this->_M_start != 0)
        free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage = __new_start + __len;
}
template <> __declspec(noinline) void std::vector<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >::push_back(NUISystem::CComponent* const& __x)
{
    if (this->_M_finish != this->_M_end_of_storage) {
        *this->_M_finish = __x;
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, std::__true_type(), 1UL, true);
}
#pragma auto_inline(on)

enum ETCInterfaceType { TC_INVENTORY_MAP = 0x1B, ETCInterfaceType_Force32 = 0x7fffffff };
class CTCBase;
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Data;                     // +0x00
    TCompare Compare;                                 // +0x0C
    bool     Dirty;                                   // +0x0D
    value_type* end() { return Data._M_finish; }
    value_type* LowerBound(const K& key);             // 0x0040F020
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> UsingComponents;                 // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    template <class T> __forceinline bool GetTC(T** out, ETCInterfaceType type)
    {
        if (UsingComponents.test(type)) {
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            ETCInterfaceType key = type;
            std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
            *out = (T*)(it == map->end() || type < it->first ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CTCInventoryMap {                               // interface 0x1B
public:
    NUISystem::CComponent* ConstructPCMapList(bool a, bool* b);   // 0x0060EEB0
};

enum CURRENT_OPEN_MENU { CURRENT_OPEN_MENU_PC_MAP = 6 };
class CTCInventory {                                  // local model (see Layout)
public:
    void*         __vftable;                          // +0x00
    CThing*       ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x04];
    std::vector<NUISystem::CComponent*> Components;   // +0x0C (CTCInventoryBase)
    unsigned char _pad_0x18[0x204];
    CURRENT_OPEN_MENU CurrentOpenMenu;                // +0x21C
    void DeleteMenuComponents();                      // 0x00587A41
    void OpenPCMapMenu();
};

void CTCInventory::OpenPCMapMenu()
{
    DeleteMenuComponents();
    CurrentOpenMenu = CURRENT_OPEN_MENU_PC_MAP;
    NUISystem::CComponent* mapMenu = 0;
    NUISystem::CManager::GetInstance();
    for (unsigned long i = 0; i < Components.size() && !mapMenu; ++i) {
        if (Components[i]->GetName() == "PC_MAP_LIST")
            mapMenu = Components[i];
    }
    if (!mapMenu) {
        CTCInventoryMap* map = 0;
        ParentThing->GetTC(&map, TC_INVENTORY_MAP);
        mapMenu = map->ConstructPCMapList(false, 0);
        if (!mapMenu)
            return;
        mapMenu->Initialise();
        Components.push_back(mapMenu);
    }
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("SCROLL_LIST_UP", -1)))
        c->ObserveEvent(NUISystem::EVENT_25);
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("SCROLL_DESCRIPTION_UP", -1)))
        c->ObserveEvent(NUISystem::EVENT_25);
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("SCROLL_LIST_DOWN", -1)))
        c->ObserveEvent(NUISystem::EVENT_25);
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("SCROLL_DESCRIPTION_DOWN", -1)))
        c->ObserveEvent(NUISystem::EVENT_25);
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("PC_BACK_CLICKABLE", -1))) {
        NUISystem::CManager::GetInstance()->AddObserver(c);
        c->ObserveEvent(NUISystem::EVENT_25);
    }
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("PC_CLOSE_CLICKABLE", -1))) {
        NUISystem::CManager::GetInstance()->AddObserver(c);
        c->ObserveEvent(NUISystem::EVENT_25);
    }
    mapMenu->ChangeState(5);
}
