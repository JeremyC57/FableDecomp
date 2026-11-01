// CTCInventoryMap::ConstructRegionDescriptionQuests(NUISystem::CComponent* parent, CRegion& region, bool pc) -- for every active quest card whose region is the described region, create the card's icon-and-name container (core, vignette or optional; PC or console layout), set its quest-name text and add it under the parent
// Grade: DIFFER(975v982) -- block placement only: every block of retail is present and the frame (0x38), the list copy, the TC lookup, the inline CCharString equality and the core path byte-match up to the shared tail; retail then keeps that tail (FindChild / SetText / AddChild) right after the core block and moves the whole non-core subtree (IsVignette inline, vignette / optional / second core leaves) after the `ret 0Ch`, its leaves jumping back into the tail; VC7.1 here lays the non-core subtree out in source order between the core block and the tail (one `jmp` over it; -7 bytes of jump forms). 14 probes: `if (!IsCore())` nesting (986), bool local for IsCore, `continue` for the region test, tail duplicated per leaf (1545) or per IsCore arm (1070), goto-merged leaves (866, one core copy), /Oy-. Next: a source shape that makes VC7.1 treat the IsCore()==false arm as out of line (retail duplicates the core leaves, so the source has two core copies), or a sibling CTCInventoryMap builder with the same layout once one lands.
// Leaf: ?ConstructRegionDescriptionQuests@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FD5E0, 982 bytes, thiscall `ret 0Ch`, esp frame (Ego_r.pdb protected
//   ?ConstructRegionDescriptionQuests@CTCInventoryMap@@IAEXPAVCComponent@NUISystem@@AAVCRegion@@_N@Z: the region is a reference).
// Callees: NUISystem::CFrontEndManager::GetInstance 0x0041E5F2, malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list nodes),
//   list<CIntelligentPointer<CThing> >::_M_insert_dispatch<const_iterator> 0x00618100 (the real STLport body in this TU, not inlined:
//   VC7.1 then sees `this` is unread and homes the list and the tag in the dead `pc` slot as retail does), CBaseIntelligentPointer::
//   GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (CTCQuestCard type 0x6C), CTCQuestCard::
//   GetRegionName 0x007025A0 / IsCore 0x00702280 / IsOptional 0x007022C0 / GetQuestName 0x00702310, CBasicString<char>::Compare
//   0x00411570, CCharString(const char*, long) 0x0099EBF0 / (const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0,
//   ~CWideString 0x0099B510, NUISystem::CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D,
//   CCountedPointer<CComponent>::Reset 0x00429C6F (engine header COMDAT), the component's FindChild (slot 3, by value),
//   AddChild (slot 59, CCountedPointer&) and SetText (slot 144), operator delete 0x00BFE9BC.
// Layout: g_pQuestManager 0x013B89FC ActiveQuestCards +0x5C (Ego_r CQuestManager); CRegion::Name +0x18; CTCQuestCard PDef +0x0C;
//   CQuestCardDef::IsVignette +0x59 (Ego_r +0x5D). Header debt: engine/CQuestManager.h / CTCQuestCard.h flatten these templates.
// Shape: the FableWin.pdb inline CTCQuestCard::IsVignette (CDefPointer copy), CCharString::operator== over
//   CBasicString<char>::operator== (it gives retail's separate length-mismatch `xor al,al`), and GetTC passing
//   StatGetInterfaceType() as a temporary (so the out pointer shares the key's frame word: frame 0x38, not 0x3C).
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);           // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* p);                         // 0x00BFE9BC
inline void* __cdecl operator new(size_t, void* where) { return where; }
template <class T> class CBasicString {                 // Ego_r.pdb CBasicString<char> (13 bytes)
public:
    T*            PData;                               // +0x00
    unsigned long StringLength;                        // +0x04
    unsigned long DataLength : 31;                     // +0x08
    bool          UseFastExtend : 1;
    bool operator==(const CBasicString<T>& other) const   // FableWin ??8?$CBasicString@D@@QBE_NABV0@@Z (inline)
    {
        if (StringLength != other.StringLength)
            return false;
        return Compare(PData, other.PData) == 0;
    }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // <char> 0x00411570 (FableWin ?Compare@?$CBasicString@D@@KIJPBD0@Z)
};
class CCharStringData {
public:
    CBasicString<char> Data;                           // +0x00
    long               NoRefs;                         // +0x0D
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    bool operator==(const CCharString& other) const    // FableWin ??8CCharString@@QBE_NABV0@@Z (inline)
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return PStringData->Data == other.PStringData->Data;
    }
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseIntelligentPointer();                         // 0x00A01B10
    CBaseObject* GetPItem() const;                     // 0x00A01B50
    void SetPItem(CBaseObject* item);                  // 0x00A01B90
    void* PData;                                       // +0x4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for T = CThing
public:
    virtual ~CIntelligentPointer();
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(size_t __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, size_t) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(size_t __n) { return _MaybeReboundAlloc::allocate(__n); }
    void deallocate(_Tp* __p, size_t __n) { _MaybeReboundAlloc::deallocate(__p, __n); }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp* pointer; typedef const _Tp& reference; };
template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::pointer pointer;
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    typedef _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _AllocProxy;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type((const _Node_allocator_type&)_M_node); }
    _List_base(const allocator_type& __a) : _M_node(_Node_allocator_type(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear()
    {
        _Node* __cur = (_Node*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
    _AllocProxy _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
public:
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    template <class _InputIter>                                  // 0x00618100 (the STLport COMDAT, not inlined)
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for (; __first != __last; ++__first)
            insert(__pos, *__first);
    }
};
}   // namespace std
void operator delete(void* p);
#include "engine/CCountedPointer.h"

enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
template <class T> class CArray {                                // STLport vector (12 bytes)
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* end() { return _M_finish; }
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                                     // +0x00
    TCompare           Compare;                                  // +0x0C
    bool               Dirty;                                    // +0x0D
    value_type* LowerBound(const K& key);                        // 0x0040F020
};
class CTCBase;
class CBaseObject { };
class CThing : public CBaseObject {
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      _pad00[0x20];
    unsigned long      UsingComponents[9];                       // +0x20 std::bitset<274>
    TCMap              Components;                               // +0x44
    __forceinline bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::StatGetInterfaceType()))
            return false;
        TCMap* map = &Components;
        TCMap::value_type* it = map->LowerBound(T::StatGetInterfaceType());
        *out = (T*)((it == map->Data.end() || T::StatGetInterfaceType() < it->first) ? map->Data.end() : it)->second;
        return true;
    }
};
class CDefPointeeBase {
public:
    virtual ~CDefPointeeBase();                        // slot 0
    virtual void Delete() const;                       // slot 1
    void AddRef() const { ++RefCount; }
    long Release() const { return --RefCount; }
    mutable long RefCount;                             // +0x04
};
class CQuestCardDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x59 - 0x08];
    bool IsVignette;                                   // +0x59 (Ego_r +0x5D: two 16-byte vectors there are 12 in retail)
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { T* object = Object; if (object->Release() == 0) object->Delete(); }
    T* operator->() const { return Object; }
};
class CTCQuestCard {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x6c; }
    unsigned char _pad00[0x0c];
    CDefPointer<const CQuestCardDef> PDef;             // +0x0C
    bool IsCore() const;                               // 0x00702280
    bool IsOptional() const;                           // 0x007022C0
    bool IsVignette() const { return CDefPointer<const CQuestCardDef>(PDef)->IsVignette; }   // inline (FableWin QBE_NXZ)
    CWideString GetQuestName() const;                  // 0x00702310
    CCharString GetRegionName() const;                 // 0x007025A0
};
class CQuestManager {
public:
    unsigned char _pad00[0x5c];
    std::list<CIntelligentPointer<CThing> > ActiveQuestCards;   // +0x5C
};
extern CQuestManager* g_pQuestManager;                 // 0x013B89FC
namespace NUISystem {
class CComponent {
public:
    virtual void c00(); virtual void c01(); virtual void c02();
    virtual CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
    virtual void c04(); virtual void c05(); virtual void c06(); virtual void c07(); virtual void c08(); virtual void c09();
    virtual void c10(); virtual void c11(); virtual void c12(); virtual void c13(); virtual void c14(); virtual void c15();
    virtual void c16(); virtual void c17(); virtual void c18(); virtual void c19(); virtual void c20(); virtual void c21();
    virtual void c22(); virtual void c23(); virtual void c24(); virtual void c25(); virtual void c26(); virtual void c27();
    virtual void c28(); virtual void c29(); virtual void c30(); virtual void c31(); virtual void c32(); virtual void c33();
    virtual void c34(); virtual void c35(); virtual void c36(); virtual void c37(); virtual void c38(); virtual void c39();
    virtual void c40(); virtual void c41(); virtual void c42(); virtual void c43(); virtual void c44(); virtual void c45();
    virtual void c46(); virtual void c47(); virtual void c48(); virtual void c49(); virtual void c50(); virtual void c51();
    virtual void c52(); virtual void c53(); virtual void c54(); virtual void c55(); virtual void c56(); virtual void c57();
    virtual void c58();
    virtual void AddChild(CCountedPointer<CComponent>& child);   // slot 59 (+0xEC)
    virtual void c60(); virtual void c61(); virtual void c62(); virtual void c63(); virtual void c64(); virtual void c65();
    virtual void c66(); virtual void c67(); virtual void c68(); virtual void c69(); virtual void c70(); virtual void c71();
    virtual void c72(); virtual void c73(); virtual void c74(); virtual void c75(); virtual void c76(); virtual void c77();
    virtual void c78(); virtual void c79(); virtual void c80(); virtual void c81(); virtual void c82(); virtual void c83();
    virtual void c84(); virtual void c85(); virtual void c86(); virtual void c87(); virtual void c88(); virtual void c89();
    virtual void c90(); virtual void c91(); virtual void c92(); virtual void c93(); virtual void c94(); virtual void c95();
    virtual void c96(); virtual void c97(); virtual void c98(); virtual void c99(); virtual void c100(); virtual void c101();
    virtual void c102(); virtual void c103(); virtual void c104(); virtual void c105(); virtual void c106(); virtual void c107();
    virtual void c108(); virtual void c109(); virtual void c110(); virtual void c111(); virtual void c112(); virtual void c113();
    virtual void c114(); virtual void c115(); virtual void c116(); virtual void c117(); virtual void c118(); virtual void c119();
    virtual void c120(); virtual void c121(); virtual void c122(); virtual void c123(); virtual void c124(); virtual void c125();
    virtual void c126(); virtual void c127(); virtual void c128(); virtual void c129(); virtual void c130(); virtual void c131();
    virtual void c132(); virtual void c133(); virtual void c134(); virtual void c135(); virtual void c136(); virtual void c137();
    virtual void c138(); virtual void c139(); virtual void c140(); virtual void c141(); virtual void c142(); virtual void c143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
};
class CManager {
public:
    CComponent* CreateComponent(const CCharString& name, bool b);   // 0x0041DB1D
};
class CFrontEndManager : public CManager {
public:
    static CFrontEndManager* __fastcall GetInstance(); // 0x0041E5F2
};
}
class CRegion {
public:
    unsigned char _pad00[0x18];
    CCharString   Name;                                // +0x18
};
class CTCInventoryMap {
protected:
    void ConstructRegionDescriptionQuests(NUISystem::CComponent* parent, CRegion& region, bool pc);
};
void CTCInventoryMap::ConstructRegionDescriptionQuests(NUISystem::CComponent* parent, CRegion& region, bool pc)
{
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> container;
    std::list<CIntelligentPointer<CThing> > cards(g_pQuestManager->ActiveQuestCards);
    for (std::list<CIntelligentPointer<CThing> >::iterator it = cards.begin(); it != cards.end(); ++it) {
        CTCQuestCard* card;
        (*it)->GetTC(&card);
        if (card->GetRegionName() == region.Name) {
            NUISystem::CComponent* text;
            if (card->IsCore()) {
                if (pc) {
                    container.Reset(manager->CreateComponent("PC_CORE_ICON_AND_NAME", false));
                    text = container->FindChild("PC_QUEST_NAME");
                } else {
                    container.Reset(manager->CreateComponent("UI_CORE_ICON_AND_NAME", false));
                    text = container->FindChild("UI_QUEST_NAME");
                }
            } else if (card->IsVignette()) {
                if (pc) {
                    container.Reset(manager->CreateComponent("PC_VIGNETTE_ICON_AND_NAME", false));
                    text = container->FindChild("PC_QUEST_NAME");
                } else {
                    container.Reset(manager->CreateComponent("UI_VIGNETTE_ICON_AND_NAME", false));
                    text = container->FindChild("UI_QUEST_NAME");
                }
            } else if (card->IsOptional()) {
                if (pc) {
                    container.Reset(manager->CreateComponent("PC_OPTIONAL_ICON_AND_NAME", false));
                    text = container->FindChild("PC_QUEST_NAME");
                } else {
                    container.Reset(manager->CreateComponent("UI_OPTIONAL_ICON_AND_NAME", false));
                    text = container->FindChild("UI_QUEST_NAME");
                }
            } else {
                if (pc) {
                    container.Reset(manager->CreateComponent("PC_CORE_ICON_AND_NAME", false));
                    text = container->FindChild("PC_QUEST_NAME");
                } else {
                    container.Reset(manager->CreateComponent("UI_CORE_ICON_AND_NAME", false));
                    text = container->FindChild("UI_QUEST_NAME");
                }
            }
            if (text)
                text->SetText(card->GetQuestName(), true, true);
            parent->AddChild(container);
        }
    }
}
