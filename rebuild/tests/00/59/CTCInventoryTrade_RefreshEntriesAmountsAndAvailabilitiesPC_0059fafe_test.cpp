// Behaviour test for 0059fafe: src/0059fafe.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCInventoryTrade::RefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryTrade::RefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists) -- count a category component's entries for the PC trade screen: for a list component (type 8) with a mapped list, the type-7 entries with nothing left (both counts); otherwise recurse into each type-7 child, adding one plus its counts
// Leaf: RefreshEntriesAmountsAndAvailabilitiesPC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059FAFE, length 279 bytes, `ret 0Ch` (hidden pair result). Frame-pointer size TU:
// optimize("y",off) + ("s",on). FableWin.pdb ?RefreshEntriesAmountsAndAvailabilitiesPC@CTCInventoryTrade@@AAE?AU?$pair@KK@std@@
// PAVCComponent@NUISystem@@AAV?$map@...CList...@std@@@Z.
// Callees: NUISystem::CComponent virtual slots 53 (+0xD4, the child vector), 65 (+0x104, the component type) and 63 (+0xFC, a
// pointer to the entry's remaining amount; names descriptive), map<CComponent*, CList*>::find @ 0x005B1A43 (catalog
// "std::set_FindLE" is propagated; STLport node: value +0x14, end() = the header at +0), CTCInventoryTrade::
// RecurseRefreshEntriesAmountsAndAvailabilitiesPC @ 0x0059E6BD (hidden pair result).
// Shape: the counts are one named result pair (its first lives in ebx, its second in the pair's frame home, which is why
// retail keeps a 0x14 frame with an unused word at [ebp-0Ch]) returned by copy through the hidden pointer; the map find is
// defined in this TU with its real STLport body (out of line, auto_inline off) so the caller hands it the component
// argument slot itself instead of homing a key copy there (GOTCHAS 24l lever);
// the mapped-list loop re-fetches the list's children through the virtual for every access, the recursion loop reads the
// component's vector once (begin loaded before end for the size); `this` is parked in [ebp-4] across the virtual calls.
// Layout: STLport vector<CCountedPointer<CComponent> > (8-byte elements, object word first).
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) { }
};
}
namespace NUISystem {
struct CComponent;
struct SChildPointer {                          // CCountedPointer<NUISystem::CComponent>
    CComponent* m_pObject;
    void*       m_pCount;
    CComponent* operator->() const { return m_pObject; }
};
struct CChildVector {                           // vector<CCountedPointer<CComponent> > (12 bytes)
    SChildPointer* _M_start;
    SChildPointer* _M_finish;
    SChildPointer* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const SChildPointer& operator[](unsigned long i) const { return _M_start[i]; }
};
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52();
    virtual const CChildVector& GetChildren();  // slot 53 (+0xD4)
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual const long* GetAmountLeft();        // slot 63 (+0xFC)
    virtual void v64();
    virtual long GetType();                     // slot 65 (+0x104)
};
struct CList : CComponent {
};
}
struct SListMapNode {                           // STLport _Rb_tree_node<pair<CComponent* const, CList*> >
    long          _M_color;
    SListMapNode* _M_parent;
    SListMapNode* _M_left;
    SListMapNode* _M_right;
    NUISystem::CComponent* Key;                 // +0x10
    NUISystem::CList*      List;                // +0x14
};
struct CListMap {                               // std::map<NUISystem::CComponent*, NUISystem::CList*> (12 bytes)
    SListMapNode* _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    SListMapNode* find(NUISystem::CComponent* const& key);   // 0x005B1A43
    SListMapNode* end() const { return _M_header; }
};
#pragma auto_inline(off)
__declspec(noinline) SListMapNode* CListMap::find(NUISystem::CComponent* const& key)
{
    SListMapNode* y = _M_header;
    SListMapNode* x = _M_header->_M_parent;
    while (x != 0) {
        if (!(x->Key < key)) { y = x; x = x->_M_left; }
        else x = x->_M_right;
    }
    if (y == _M_header || key < y->Key) y = _M_header;
    return y;
}
#pragma auto_inline(on)

struct CTCInventoryTrade {
    std::pair<unsigned long, unsigned long> RecurseRefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists);   // 0x0059E6BD
    std::pair<unsigned long, unsigned long> RefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists);
};

std::pair<unsigned long, unsigned long> CTCInventoryTrade::RefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists)
{
    std::pair<unsigned long, unsigned long> result(0, 0);
    const NUISystem::CChildVector& children = component->GetChildren();
    if (component->GetType() == 8) {
        SListMapNode* found = lists.find(component);
        if (found != lists.end()) {
            NUISystem::CList* list = found->List;
            for (unsigned long i = 0; i < list->GetChildren().size(); ++i) {
                if (list->GetChildren()[i]->GetType() == 7 && *list->GetChildren()[i]->GetAmountLeft() == 0) {
                    ++result.first;
                    ++result.second;
                }
            }
        } else {
            for (unsigned long i = 0; i < children.size(); ++i) {
                if (children[i]->GetType() == 7) {
                    std::pair<unsigned long, unsigned long> sub = RecurseRefreshEntriesAmountsAndAvailabilitiesPC(children[i].m_pObject, lists);
                    result.first += sub.first + 1;
                    result.second += sub.second;
                }
            }
        }
    }
    return result;
}

// ---- stubs and driver ----
using NUISystem::CComponent;
using NUISystem::SChildPointer;
#define V1(n) void CComponent::v##n() { }
V1(00) V1(01) V1(02) V1(03) V1(04) V1(05) V1(06) V1(07) V1(08) V1(09) V1(10) V1(11) V1(12) V1(13) V1(14) V1(15) V1(16)
V1(17) V1(18) V1(19) V1(20) V1(21) V1(22) V1(23) V1(24) V1(25) V1(26) V1(27) V1(28) V1(29) V1(30) V1(31) V1(32) V1(33)
V1(34) V1(35) V1(36) V1(37) V1(38) V1(39) V1(40) V1(41) V1(42) V1(43) V1(44) V1(45) V1(46) V1(47) V1(48) V1(49) V1(50)
V1(51) V1(52) V1(54) V1(55) V1(56) V1(57) V1(58) V1(59) V1(60) V1(61) V1(62) V1(64)
struct TestInfo { CComponent* c; NUISystem::CChildVector kids; long type; long amount; };
static TestInfo g_info[24]; static int g_n = 0;
static TestInfo* Info(CComponent* c) { for (int i = 0; i < g_n; ++i) if (g_info[i].c == c) return &g_info[i]; return 0; }
static void Set(CComponent* c, long type, long amount, SChildPointer* kb = 0, int kn = 0)
{
    TestInfo* t = Info(c); if (!t) { t = &g_info[g_n++]; t->c = c; }
    t->type = type; t->amount = amount; t->kids._M_start = kb; t->kids._M_finish = kb + kn; t->kids._M_end_of_storage = kb + kn;
}
static int g_childCalls = 0;
const NUISystem::CChildVector& CComponent::GetChildren() { ++g_childCalls; return Info(this)->kids; }
long CComponent::GetType() { return Info(this)->type; }
const long* CComponent::GetAmountLeft() { return &Info(this)->amount; }

static int g_rec = 0; static CComponent* g_recArg[8]; static CListMap* g_recLists = 0; static CTCInventoryTrade* g_recThis = 0;
std::pair<unsigned long, unsigned long> CTCInventoryTrade::RecurseRefreshEntriesAmountsAndAvailabilitiesPC(CComponent* c, CListMap& lists)
{
    g_recArg[g_rec++] = c; g_recLists = &lists; g_recThis = this;
    unsigned long a = (unsigned long)Info(c)->amount;
    return std::pair<unsigned long, unsigned long>(a, a * 2);
}

static CComponent g_comp[12];
static NUISystem::CList g_list[2];
static SListMapNode g_header, g_n1, g_n2, g_n3;
static SChildPointer g_listKids[5], g_compKids[4];
int main()
{
    // map {g_comp[1] -> g_list[0], g_comp[3] -> g_list[1], g_comp[6] -> g_list[0]} as a three-node tree rooted at g_comp[3]
    memset(&g_header, 0, sizeof(g_header));
    g_n2.Key = &g_comp[3]; g_n2.List = &g_list[1]; g_n2._M_left = &g_n1; g_n2._M_right = &g_n3; g_n2._M_parent = &g_header;
    g_n1.Key = &g_comp[1]; g_n1.List = &g_list[0]; g_n1._M_left = 0; g_n1._M_right = 0; g_n1._M_parent = &g_n2;
    g_n3.Key = &g_comp[6]; g_n3.List = &g_list[0]; g_n3._M_left = 0; g_n3._M_right = 0; g_n3._M_parent = &g_n2;
    g_header._M_parent = &g_n2; g_header._M_left = &g_n1; g_header._M_right = &g_n3;
    CListMap lists; lists._M_header = &g_header; lists._M_node_count = 3; lists._M_key_compare = 0;

    // the found list's children: type 7 with 0 left (x2), type 7 with 3 left, type 6 with 0 left, type 7 with 0 left
    for (int i = 0; i < 5; ++i) { g_listKids[i].m_pObject = &g_comp[7 + (i % 5)]; g_listKids[i].m_pCount = 0; }
    g_listKids[4].m_pObject = &g_comp[11];
    Set(&g_comp[7], 7, 0); Set(&g_comp[8], 7, 3); Set(&g_comp[9], 6, 0); Set(&g_comp[10], 7, 0); Set(&g_comp[11], 7, 0);
    Set(&g_list[0], 8, 0, g_listKids, 5);
    Set(&g_list[1], 8, 0, 0, 0);

    CTCInventoryTrade trade;
    // 1: list component mapped (leaf of the tree) -> the three exhausted type-7 entries, both counts
    Set(&g_comp[6], 8, 0, g_compKids, 0);
    g_rec = 0;
    std::pair<unsigned long, unsigned long> r = trade.RefreshEntriesAmountsAndAvailabilitiesPC(&g_comp[6], lists);
    CHECK(r.first == 3 && r.second == 3 && g_rec == 0);
    // 1b: the left leaf maps to the same list
    Set(&g_comp[1], 8, 0, 0, 0);
    r = trade.RefreshEntriesAmountsAndAvailabilitiesPC(&g_comp[1], lists);
    CHECK(r.first == 3 && r.second == 3);

    // 2: the root maps to an empty list -> nothing counted, no recursion
    Set(&g_comp[3], 8, 0, g_compKids, 2);
    r = trade.RefreshEntriesAmountsAndAvailabilitiesPC(&g_comp[3], lists);
    CHECK(r.first == 0 && r.second == 0 && g_rec == 0);

    // 3: list component not in the map -> recurse into its type-7 children (amounts 4 and 9; the type-5 child skipped)
    g_compKids[0].m_pObject = &g_comp[0]; g_compKids[1].m_pObject = &g_comp[5]; g_compKids[2].m_pObject = &g_comp[4];
    Set(&g_comp[0], 7, 4); Set(&g_comp[5], 5, 100); Set(&g_comp[4], 7, 9);
    Set(&g_comp[2], 8, 0, g_compKids, 3);
    g_rec = 0; g_childCalls = 0;
    r = trade.RefreshEntriesAmountsAndAvailabilitiesPC(&g_comp[2], lists);
    CHECK(g_rec == 2 && g_recArg[0] == &g_comp[0] && g_recArg[1] == &g_comp[4]);
    CHECK(g_recLists == &lists && g_recThis == &trade);
    CHECK(r.first == (4 + 1) + (9 + 1) && r.second == 8 + 18);
    CHECK(g_childCalls == 1);

    // 4: not a list component -> nothing, even with type-7 children
    Set(&g_comp[2], 3, 0, g_compKids, 3);
    g_rec = 0;
    r = trade.RefreshEntriesAmountsAndAvailabilitiesPC(&g_comp[2], lists);
    CHECK(r.first == 0 && r.second == 0 && g_rec == 0);

    // 5: empty map (root null) -> falls back to recursion
    g_header._M_parent = 0;
    Set(&g_comp[6], 8, 0, g_compKids, 1);
    g_rec = 0;
    r = trade.RefreshEntriesAmountsAndAvailabilitiesPC(&g_comp[6], lists);
    CHECK(g_rec == 1 && g_recArg[0] == &g_comp[0] && r.first == 5 && r.second == 8);
    if (fails == 0) printf("0059fafe_TEST PASS\n");
    return fails;
}
