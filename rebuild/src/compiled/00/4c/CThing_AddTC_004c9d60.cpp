// CThing::AddTC(const CCharString& name, bool replace, const CTCParamsBase* params) -- create the named thing component and attach it: refused unless TCs may be added; an interface already present is kept (the new TC deleted) unless replacing; the physics / appearance / driver shortcuts are updated; a replaced TC is dropped from the update list and the pending-removal list and deleted, otherwise the TC is added to the sorted interface map; then the presence bit is set, the params applied, the TC initialised and queued for updates when it wants them
// Leaf: AddTC
// Callees: CreateTC @ 0x00686F10 (__fastcall name, thing), puts @ 0x00BFEAF2 / abort @ 0x00BFEAEC (bitset range check),
//          CVectorMap::LowerBound @ 0x0040F020, list<CTCBase*>::remove @ 0x004CC150, ~CBaseIntelligentPointer @ 0x00A01C10,
//          free @ 0x00BFEA14, vector<pair>::_M_insert_overflow @ 0x004CCCE0, sort @ 0x004CD5D0, bitset::set @ 0x004CC090,
//          CThing::InitTC @ 0x004C73E0, list<CTCBase*>::push_back @ 0x004CC820; global TCsToBeRemoved list @ 0x013B8A24.
// Layout: retail CThing (16 bytes narrower than the PDB view): UsingComponents bitset<274> +0x20, Components CVectorMap
//          {vector +0x44, Compare +0x50, Dirty +0x51}, UpdateComponentsList +0x54, PTCDriver +0x5C, PhysicsTC +0x60,
//          PTCGraphicAppearance +0x64, AddingTCsPermitted bit 6 of +0x93 (PDB +0xa3).
extern "C" int  __cdecl puts(const char* text);         // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();                      // 0x00BFEAEC
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

struct CCharString { void* m_data; };
struct CTCParamsBase;
struct CThing;

struct CTCBase {
    virtual ~CTCBase();                                 // slot 0 (scalar deleting dtor)
    virtual void v01(); virtual void v02();
    virtual void ApplyParams(const CTCParamsBase* params);   // slot 3 (+0x0c)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual bool WantsUpdate();                         // slot 8 (+0x20)
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual long GetInterfaceType();                    // slot 21 (+0x54)
    virtual void v22(); virtual void v23();
    virtual bool IsDriver();                            // slot 24 (+0x60)
};

CTCBase* __fastcall CreateTC(const CCharString& name, CThing* thing);   // 0x00686F10

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};
struct STCRemoval {
    CBaseIntelligentPointer PThing;                     // +0x00
    CTCBase*                PTC;                        // +0x08
};
struct STCRemovalNode { STCRemovalNode* next; STCRemovalNode* prev; STCRemoval value; };
struct STCRemovalIterator {
    STCRemovalNode* node;
    STCRemovalIterator(STCRemovalNode* n) : node(n) { }
    STCRemovalIterator(const STCRemovalIterator& o) : node(o.node) { }
    STCRemoval* operator->() const { return &node->value; }
    bool operator!=(const STCRemovalIterator& o) const { return node != o.node; }
    STCRemovalIterator& operator++() { node = node->next; return *this; }
};
struct CTCRemovalList {
    STCRemovalNode* head;
    STCRemovalIterator begin() { return STCRemovalIterator(head->next); }
    STCRemovalIterator end() { return STCRemovalIterator(head); }
    STCRemovalIterator erase(STCRemovalIterator where)
    {
        STCRemovalNode* next = where.node->next;
        STCRemovalNode* prev = where.node->prev;
        prev->next = next;
        next->prev = prev;
        where.node->value.~STCRemoval();
        free(where.node);
        return STCRemovalIterator(next);
    }
};
extern CTCRemovalList g_TCRemovalList;                  // 0x013B8A24

struct CTCBitset {                                      // std::bitset<274>
    unsigned long m_words[9];
    __forceinline bool test(unsigned long position) const
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (m_words[position >> 5] & (1UL << (position & 31))) != 0;
    }
    CTCBitset& set(unsigned long position);             // 0x004CC090
};

struct __false_type { };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
#ifndef __PLACEMENT_NEW_INLINE                          // the CRT <new> placement form (STLport _Construct)
#define __PLACEMENT_NEW_INLINE
inline void* operator new(unsigned int, void* p) throw() { return p; }
#endif

typedef pair<long, CTCBase*> STCEntry;
void* GrowVectorStorage_9d60(void* vector, unsigned long count);
struct CTCEntryVector {
    STCEntry* _M_start;
    STCEntry* _M_finish;
    STCEntry* _M_end_of_storage;
    void _M_insert_overflow(STCEntry* pos, const STCEntry& x, const __false_type& tag, unsigned long fill_len, bool atend);   // 0x004CCCE0
    __forceinline void push_back(const STCEntry& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) STCEntry(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
#pragma auto_inline(off)
void CTCEntryVector::_M_insert_overflow(STCEntry* pos, const STCEntry& x, const __false_type& tag, unsigned long fill_len, bool atend)
{
    STCEntry* p = (STCEntry*)GrowVectorStorage_9d60(this, fill_len);
    if (p)
        new (p) STCEntry(x);
}
#pragma auto_inline(on)

struct CKeyPairCompareLess { };
void __fastcall sort(STCEntry* first, STCEntry* last, CKeyPairCompareLess comp);   // 0x004CD5D0

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    CTCEntryVector      Data;                           // +0x00
    CKeyPairCompareLess Compare;                        // +0x0c
    bool                Dirty;                          // +0x0d
    STCEntry* LowerBound(const long* type);             // 0x0040F020
    STCEntry* find(const long& type)
    {
        STCEntry* it = LowerBound(&type);
        if (it == Data._M_finish || type < it->first)
            it = Data._M_finish;
        return it;
    }
    STCEntry* end() const { return Data._M_finish; }
    __forceinline void Add(long key, CTCBase* value) { Data.push_back(STCEntry(key, value)); Dirty = true; }
    void Resort()
    {
        sort(Data._M_start, Data._M_finish, Compare);
        Dirty = false;
    }
};
#pragma auto_inline(off)
STCEntry* CInterfaceMap::LowerBound(const long* type)
{
    STCEntry* it = Data._M_start;
    int len = int(Data._M_finish - Data._M_start);
    while (len > 0) {
        int half = len / 2;
        STCEntry* middle = it + half;
        if (middle->first < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CTCList {                                        // list<CTCBase*>
    void* _M_node;
    void remove(CTCBase* const& value);                 // 0x004CC150
    void push_back(CTCBase* const& value);              // 0x004CC820
};

struct CThing {
    void*         __vftable;                            // +0x00
    unsigned char _pad04[0x1c];
    CTCBitset     UsingComponents;                      // +0x20
    CInterfaceMap Components;                           // +0x44
    CTCList       UpdateComponentsList;                 // +0x54
    unsigned char _pad58[0x04];
    CTCBase*      PTCDriver;                            // +0x5c
    CTCBase*      PhysicsTC;                            // +0x60
    CTCBase*      PTCGraphicAppearance;                 // +0x64
    unsigned char _pad68[0x93 - 0x68];
    unsigned char _bits93 : 6;
    unsigned char AddingTCsPermitted : 1;               // +0x93 bit 6
    void InitTC(CTCBase* tc);                           // 0x004C73E0
    void AddToUpdateList(CTCBase* component) { UpdateComponentsList.push_back(component); }
    CTCBase* AddTC(const CCharString& name, bool replace, const CTCParamsBase* params);
};

CTCBase* CThing::AddTC(const CCharString& name, bool replace, const CTCParamsBase* params)
{
    if (!AddingTCsPermitted)
        return 0;
    CTCBase* tc = CreateTC(name, this);
    CTCBase* updateEntry = tc;
    if (!tc)
        return 0;
    if (UsingComponents.test(tc->GetInterfaceType()) && !replace) {
        delete tc;
        return 0;
    }
    if (tc->GetInterfaceType() == 2)
        PhysicsTC = tc;
    else if (tc->GetInterfaceType() == 0x5b)
        PTCGraphicAppearance = tc;
    if (tc->IsDriver())
        PTCDriver = tc;
    STCEntry* it;
    if (replace && (it = Components.find(tc->GetInterfaceType())) != Components.end()) {
        CTCBase*& old = it->second;
        UpdateComponentsList.remove(old);
        for (STCRemovalIterator i = g_TCRemovalList.begin(); i != g_TCRemovalList.end(); ) {
            if (i->PTC == old)
                i = g_TCRemovalList.erase(i);
            else
                ++i;
        }
        if (old)
            delete old;
        old = tc;
    } else {
        Components.Add(tc->GetInterfaceType(), tc);
        Components.Resort();
    }
    UsingComponents.set(tc->GetInterfaceType());
    if (params)
        tc->ApplyParams(params);
    InitTC(tc);
    if (tc->WantsUpdate())
        UpdateComponentsList.push_back(updateEntry);
    return tc;
}