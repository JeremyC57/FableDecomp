// CTCInventoryMap::ConstructRegionDescriptionQuests(NUISystem::CComponent* parent, CRegion* region, bool pc) -- for every quest card whose region is the described region, creates the card's icon-and-name container (core, vignette or optional, PC or console layout), sets its quest-name text and adds it under the parent
// Grade: DIFFER(986v982) -- retail places the non-core (optional/vignette) branch after the ret and keeps the merged tail in the core block; ours lays the blocks out in source order (b: 975v982)
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FD5E0, 976 bytes, `ret 0Ch`. Speed TU (no pragmas, no frame pointer).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2, malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list),
//   list<CIntelligentPointer<CThing>>::_M_insert_dispatch(pos, first, last, __false_type) 0x00618100 (same-TU stand-in),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, LowerBound 0x0040F020 (TC 0x6C quest card), CTCQuestCard::GetRegionName
//   0x007025A0 / IsCore 0x00702280 / IsOptional 0x007022C0 / GetQuestName 0x00702310, Compare 0x00411570 (inline
//   CCharString ==), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CWideString 0x0099B510,
//   CManager::CreateComponent 0x0041DB1D, CCountedPointer<CComponent>::Reset 0x00429C6F, the component slots 3 (+0x0C,
//   FindChild by value), 59 (+0xEC, AddChild) and 144 (+0x240, SetText), operator delete 0x00BFE9BC.
// Layout: g_pQuestManager 0x013B89FC quest-card list +0x5C (one-pointer STLport list of 8-byte intelligent pointers);
//   CRegion name +0x18; CTCQuestCard def pointer +0x0C, CQuestCardDef vignette flag +0x59.
// Strings: PC_/UI_CORE_ICON_AND_NAME, PC_/UI_VIGNETTE_ICON_AND_NAME, PC_/UI_OPTIONAL_ICON_AND_NAME, PC_/UI_QUEST_NAME.
#include "engine/CCountedPointer.h"
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* p);                         // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

struct CCharStringData {
    const char*   Data;                                // +0x00
    unsigned long Length;                              // +0x04
    bool IsEqual(const CCharStringData& other) const
    {
        if (Length != other.Length)
            return false;
        return Compare_00411570(Data, other.Data) == 0;
    }
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = PStringData;
        CCharStringData* b = other.PStringData;
        if (a == b)
            return true;
        if (a == 0 || b == 0)
            return false;
        return a->IsEqual(*b);
    }
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
struct __false_type { __false_type() {} };

class CThing;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    CThing* m_pItem;
    CThing* GetPItem() const;                          // 0x00A01B50
};
template <class T>
struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T>
struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* node) : _M_node(node) {}
    _List_iterator(const _List_iterator& o) : _M_node(o._M_node) {}
    T* operator->() const { return &_M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
};
template <class T>
struct _List_const_iterator {
    _List_node<T>* _M_node;
    const T* operator->() const { return &_M_node->_M_data; }
    bool operator!=(const _List_const_iterator& other) const { return _M_node != other._M_node; }
};
template <class T>
struct _STLP_alloc_proxy {
    _List_node<T>* _M_data;
    _STLP_alloc_proxy(_List_node<T>* p) : _M_data(p) {}
};
struct CCardList {                                     // list<CIntelligentPointer<CThing>>
    typedef CBaseIntelligentPointer T;
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    typedef _List_const_iterator<T> const_iterator;
    _STLP_alloc_proxy<T> _M_node;
    CCardList(const CCardList& x) : _M_node(0)
    {
        _Node* node = (_Node*)malloc(sizeof(_Node));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node._M_data = node;
        const_iterator first = { x._M_node._M_data->_M_next };
        const_iterator last = { x._M_node._M_data };
        _M_insert_dispatch(begin(), first, last, __false_type());
    }
    ~CCardList()
    {
        _Node* cur = _M_node._M_data->_M_next;
        while (cur != _M_node._M_data) {
            _Node* tmp = cur;
            cur = cur->_M_next;
            tmp->_M_data.~T();
            free(tmp);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
        free(_M_node._M_data);
    }
    iterator begin() const { return iterator(_M_node._M_data->_M_next); }
    iterator end() const { return iterator(_M_node._M_data); }
    void _M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&);   // 0x00618100
};
CThing* SetItem_s303(CBaseIntelligentPointer* p, CThing* item);
#pragma auto_inline(off)
// Same-TU body of list<CIntelligentPointer<CThing>>::_M_insert_dispatch (range form) 0x00618100: a copy of every element
// inserted before pos; `this` and the tag are not read.
void CCardList::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&)
{
    for (; first != last; first._M_node = first._M_node->_M_next) {
        _Node* tmp = (_Node*)malloc(sizeof(_Node));
        SetItem_s303(&tmp->_M_data, first->GetPItem());
        _Node* n = pos._M_node;
        _Node* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
    }
}
#pragma auto_inline(on)

class CTCBase;
struct CInterfaceMap {                                 // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20 bitset of interface types
    CInterfaceMap m_interfaces;                        // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterface(T** out, long kind)
    {
        if (HasTC(kind)) {
            long key = kind;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct CDefPointee {
    virtual void Destroy(int flags);                   // slot 0
    virtual void Delete();                             // slot 1
    long RefCount;                                     // +0x04
};
struct CQuestCardDef : CDefPointee {
    unsigned char _pad08[0x59 - 0x08];
    bool Vignette;                                     // +0x59
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { if (--Object->RefCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
};
struct CTCQuestCard {
    unsigned char _pad00[0x0c];
    CDefPointer<CQuestCardDef> PDef;                   // +0x0C
    bool IsCore() const;                               // 0x00702280
    bool IsOptional() const;                           // 0x007022C0
    CWideString GetQuestName() const;                  // 0x00702310
    CCharString GetRegionName() const;                 // 0x007025A0
    bool IsVignetteInline() const
    {
        CDefPointer<CQuestCardDef> def(PDef);
        return def->Vignette;
    }
};
struct CQuestManager {
    unsigned char _pad00[0x5c];
    CCardList     QuestCards;                          // +0x5C
};
extern CQuestManager* g_pQuestManager;                 // 0x013B89FC

struct SCountBlock {
    long  m_references;                                // +0
    void (__fastcall* m_pfnDestroy)(void*);            // +4
    void* m_pObject;                                   // +8
};

namespace NUISystem {
struct CComponent {
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
    virtual void AddChild(const CCountedPointer<CComponent>& child);   // slot 59 (+0xEC)
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
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance(); // 0x0041E5F2
};
}
struct CRegion {
    unsigned char _pad00[0x18];
    CCharString   Name;                                // +0x18
};
struct CTCInventoryMap {
    void ConstructRegionDescriptionQuests(NUISystem::CComponent* parent, CRegion* region, bool pc);
};
void CTCInventoryMap::ConstructRegionDescriptionQuests(NUISystem::CComponent* parent, CRegion* region, bool pc)
{
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> container;
    CCardList cards(g_pQuestManager->QuestCards);
    for (CCardList::iterator it = cards.begin(); it != cards.end(); ++it) {
        CThing* thing = it->GetPItem();
        CTCQuestCard* card;
        thing->GetInterface(&card, 0x6c);
        if (card->GetRegionName() == region->Name) {
            NUISystem::CComponent* text;
            if (!card->IsCore()) {
                if (card->IsVignetteInline()) {
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
