// Behaviour test for 004b2b70: src/004b2b70.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CQuestManager::GetAllActiveQuestInfo(vector<CCharString>& scriptNames, vector<CCharString>& defNames)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQuestManager::GetAllActiveQuestInfo(vector<CCharString>& scriptNames, vector<CCharString>& defNames) -- clear both lists, then for every active quest card holding a quest-card component append its script quest name and its definition's display name, both to scriptNames (retail's own behaviour: defNames is only cleared)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B2B70, length 288 bytes, `ret 8`.
// Callees: vector<CCharString>::erase(first, last) 0x0043336A (the inline clear, x2), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (TC 0x6c), CCharString(const
// CCharString&) 0x0099EC30 (the placement copies), vector<CCharString>::_M_insert_overflow 0x00433530, CThing::GetDefName
// 0x004C7CC0 (CDefString by hidden pointer), CDefStringTable::GetString 0x009D49B0 on g_DefStringTable 0x013CA828,
// ~CCharString 0x0099EAE0.
// Note: retail appends BOTH strings to the first vector (esi) -- the second list is only cleared; the source keeps
// that behaviour.
// Layout (retail): ActiveQuestCards (STLport list<CIntelligentPointer<CThing>>, head pointer, node data +8) +0x5c;
// CThing TC mask +0x20 (0x6c = word 3 bit 12), Components +0x44; Ego_r.pdb CTCQuestCard ScriptQuestName +0x28.
// Shape: the uninitialised `CTCQuestCard* tc; if (HasTC) tc = GetTC;` of the landed 0x004B0C80 family (VC keeps the
// first argument's word as the stale value); STLport push_back with a placement copy under VC's null test and a
// `__false_type` tag with a user ctor; the key, the tag and the def-name temporary all reuse the dead argument
// slots; `this` is parked in the entry frame word for the head re-read.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
struct __false_type { __false_type() {} };             // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct vector {                     // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                       // vector<CCharString>: 0x0043336A
    void clear() { erase(begin(), end()); }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type& p2, unsigned long fill_len, bool atend)
{
    T* p = (T*)GrowVectorStorage_s7(this, fill_len);
    if (p)
        new (p) T(x);
    
}
#pragma auto_inline(on)

class CDefString { public:                                    // a definition-string handle: one table index
    long m_index;
};
struct CDefStringTable {
    CCharString GetString(CDefString name);            // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;               // 0x013CA828

class CTCBase;
struct CTCEntry {
    long     Type;
    CTCBase* TC;
};
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase* Find(long type) const
    {
        CTCEntry* it = LowerBound((unsigned long)type);
        if (it == _M_finish || type < it->Type)
            it = _M_finish;
        return it->TC;
    }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                  // +0x20
    unsigned char _pad40[4];
    CTCMap        Components;                          // +0x44
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const { return Components.Find(type); }
    CDefString GetDefName() const;                     // 0x004C7CC0
};
class CTCQuestCard { public:                                  // TC type 0x6c (Ego_r.pdb)
    unsigned char _pad00[0x28];
    CCharString   ScriptQuestName;                     // +0x28
};

struct CBaseIntelligentPointer {
    void* Info;
    CThing* GetPItem() const;                          // 0x00A01B50
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};

class CQuestManager { public:
    unsigned char _pad00[0x5c];
    _List_node<CBaseIntelligentPointer>* ActiveQuestCards;   // +0x5c (list head)
    void GetAllActiveQuestInfo(vector<CCharString>& scriptNames, vector<CCharString>& defNames);
};

void CQuestManager::GetAllActiveQuestInfo(vector<CCharString>& scriptNames, vector<CCharString>& defNames)
{
    scriptNames.clear();
    defNames.clear();
    for (_List_node<CBaseIntelligentPointer>* n = ActiveQuestCards->_M_next; n != ActiveQuestCards; n = n->_M_next) {
        CThing* card = n->_M_data.GetPItem();
        CTCQuestCard* tc;
        if (card->HasTC(0x6c))
            tc = (CTCQuestCard*)card->GetTC(0x6c);
        if (tc) {
            scriptNames.push_back(tc->ScriptQuestName);
            {
                CCharString name = g_DefStringTable.GetString(card->GetDefName());
                scriptNames.push_back(name);
            }
        }
    }
}

// ---- stubs and driver ----
static void* g_sraw[8]; static void* g_draw[8]; static CCharString* const g_s = (CCharString*)g_sraw; static CCharString* const g_d = (CCharString*)g_draw; static int g_grow = 0, g_copy = 0, g_dtor = 0, g_erase = 0, g_get = 0, g_lb = 0, g_name = 0, g_str = 0;
static long g_lastIndex = -1; static unsigned long g_lbType = 0;
CDefStringTable g_DefStringTable;
CCharString::CCharString(const CCharString& other) { ++g_copy; PStringData = other.PStringData; }
CCharString::~CCharString() { ++g_dtor; }
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{
    ++g_grow; vector<CCharString>* v = (vector<CCharString>*)storage; CCharString* p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
template <> CCharString* vector<CCharString>::erase(CCharString* first, CCharString* last) { ++g_erase; _M_finish = first + (_M_finish - last); return first; }
CCharString CDefStringTable::GetString(CDefString name)
{ ++g_str; CHECK(this == &g_DefStringTable); g_lastIndex = name.m_index; static void* raw[1]; CCharString* r = (CCharString*)raw; r->PStringData = (void*)(0x1000 + name.m_index); return *r; }
CTCEntry* CTCMap::LowerBound(const unsigned long& type) const { ++g_lb; g_lbType = type; CTCEntry* e = _M_start; while (e != _M_finish && (unsigned long)e->Type < type) ++e; return e; }
CDefString CThing::GetDefName() const { ++g_name; CDefString d; d.m_index = (long)_pad00[0]; return d; }
CThing* CBaseIntelligentPointer::GetPItem() const { ++g_get; return (CThing*)Info; }
int main()
{
    static CThing things[3]; memset(things, 0, sizeof(things));
    static unsigned char cardRaw[2 * sizeof(CTCQuestCard)]; CTCQuestCard* cards = (CTCQuestCard*)cardRaw; cards[0].ScriptQuestName.PStringData = (void*)0xA0; cards[1].ScriptQuestName.PStringData = (void*)0xB0;
    CTCEntry e0[2] = { { 0x10, 0 }, { 0x6c, (CTCBase*)&cards[0] } };
    CTCEntry e2[1] = { { 0x6c, (CTCBase*)&cards[1] } };
    things[0]._pad00[0] = 7; things[0].UsingComponents[3] |= 1UL << 12; things[0].Components._M_start = e0; things[0].Components._M_finish = e0 + 2;
    things[2]._pad00[0] = 9; things[2].UsingComponents[3] |= 1UL << 12; things[2].Components._M_start = e2; things[2].Components._M_finish = e2 + 1;
    _List_node<CBaseIntelligentPointer> head, n[2];
    head._M_next = &n[0]; n[0]._M_next = &n[1]; n[1]._M_next = &head; n[0]._M_data.Info = &things[0]; n[1]._M_data.Info = &things[2];
    CQuestManager qm; qm.ActiveQuestCards = &head;
    vector<CCharString> s, d; s._M_start = g_s; s._M_finish = g_s + 1; s._M_end_of_storage = g_s + 3; d._M_start = g_d; d._M_finish = g_d + 2; d._M_end_of_storage = g_d + 8;
    qm.GetAllActiveQuestInfo(s, d);
    CHECK(g_erase == 2 && d._M_finish == g_d); CHECK(g_get == 2 && g_lb == 2 && g_lbType == 0x6c && g_name == 2 && g_str == 2);
    CHECK(s._M_finish == g_s + 4 && g_grow == 1);
    CHECK(g_s[0].PStringData == (void*)0xA0 && g_s[1].PStringData == (void*)0x1007 && g_s[2].PStringData == (void*)0xB0 && g_s[3].PStringData == (void*)0x1009);
    CHECK(g_dtor == 2);   // the def-name temporaries
    if (fails == 0) printf("004b2b70_TEST PASS\n"); else printf("004b2b70_TEST FAIL\n");
    return fails;
}