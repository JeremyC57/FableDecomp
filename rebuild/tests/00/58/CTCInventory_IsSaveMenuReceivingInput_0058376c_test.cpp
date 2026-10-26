// Behaviour test for retail 0x0058376c: the candidate source below is embedded verbatim (from src/0058376c.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CTCInventory::IsSaveMenuReceivingInput() -- scans each inventory screen's COMPONENT_SAVE entries for input states 1 or 2
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058376C, 169 bytes. The two-state query and list lifetime are kept inline to reproduce both exits.
// Callees: STLport list<entry*> default ctor 0x0042ABAF and dtor 0x0042ABCA (manifest "CActiveFile::OnReadFinished" is a
//   propagated twin), CCharString(const char*, long) 0x0099EBF0 for the by-value name argument, the screen's virtual
//   slot 5 (+0x14) entry search, and the entry's second-base virtual slot 2 (+0x08) state query.
// Local model: the CTCInventory screen vector sits at +0x0c (retail); no engine/ header declares these members.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);       // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
struct CStateQuery {
    virtual void v00(); virtual void v01();
    virtual bool IsInState(long state);              // slot 2
};
struct CComponentBase { virtual void v00(); };
struct CMenuEntry : CComponentBase, CStateQuery {};
template<class T> struct CListAllocator { CListAllocator() {} };
struct CEntryList {
    struct SNode { SNode* next; SNode* prev; CMenuEntry* data; };
    SNode* node;
    explicit CEntryList(const CListAllocator<CMenuEntry*>& a = CListAllocator<CMenuEntry*>()); // 0x0042ABAF
    ~CEntryList(); // 0x0042ABCA
};
struct CMenuScreen {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void FindEntries(CCharString name, CEntryList& out); // slot 5, 0x14
};
struct CScreenVector {
    CMenuScreen** first; CMenuScreen** last;
    unsigned long size() const { return (unsigned long)(last - first); }
    CMenuScreen*& operator[](unsigned long n) const { return first[n]; }
};
class CTCInventory { public:
    unsigned char _pad[0x0c];
    CScreenVector Screens;
    bool IsSaveMenuReceivingInput();
};
bool CTCInventory::IsSaveMenuReceivingInput()
{
    unsigned long i = 0;
    for (; i < Screens.size(); ++i) {
        CEntryList entries;
        Screens[i]->FindEntries(CCharString("COMPONENT_SAVE", -1), entries);
        for (CEntryList::SNode* node = entries.node->next; node != entries.node; node = node->next) {
            CMenuEntry* entry = node->data;
            if (entry) {
                CStateQuery* query = entry;                      // second base, +0x04
                if (query->IsInState(1) || query->IsInState(2)) {
                    return true;
                }
            }
        }
    }
    return false;
}
// ===== harness =====
#include <stdio.h>
#include <string.h>
static int g_strings, g_lists, g_queries; static const char* g_lastname;
CCharString::CCharString(const char* t, long) { PStringData = (CCharStringData*)t; ++g_strings; }
CCharString::CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_strings; }
CCharString::~CCharString() { --g_strings; }
CEntryList::CEntryList(const CListAllocator<CMenuEntry*>&) { node = new SNode; node->next = node->prev = node; node->data = 0; ++g_lists; }
CEntryList::~CEntryList() { SNode* n = node->next; while (n != node) { SNode* x = n->next; delete n; n = x; } delete node; --g_lists; }
void CStateQuery::v00() {} void CStateQuery::v01() {} bool CStateQuery::IsInState(long) { return false; }
void CComponentBase::v00() {}
struct TEntry : CMenuEntry { long state; TEntry(long s) : state(s) {} virtual bool IsInState(long s) { ++g_queries; return s == state; } };
void CMenuScreen::v00() {} void CMenuScreen::v01() {} void CMenuScreen::v02() {} void CMenuScreen::v03() {} void CMenuScreen::v04() {}
void CMenuScreen::FindEntries(CCharString, CEntryList&) {}
struct TScreen : CMenuScreen {
    CMenuEntry* items[3]; int n;
    virtual void FindEntries(CCharString name, CEntryList& out) {
        g_lastname = (const char*)name.PStringData;
        for (int i = 0; i < n; ++i) { CEntryList::SNode* x = new CEntryList::SNode; x->data = items[i]; x->next = out.node; x->prev = out.node->prev; out.node->prev->next = x; out.node->prev = x; }
    }
};
int main() {
    TEntry idle(0), busy(2), active(1);
    TScreen s0; s0.n = 2; s0.items[0] = 0; s0.items[1] = &idle;           /* null entry skipped, idle answers no to 1 and 2 */
    TScreen s1; s1.n = 1; s1.items[0] = &busy;
    CMenuScreen* screens[2] = { &s0, &s1 };
    CTCInventory inv; inv.Screens.first = screens; inv.Screens.last = screens + 1;
    int ok = !inv.IsSaveMenuReceivingInput() && g_queries == 2 && g_lists == 0 && g_strings == 0 && !strcmp(g_lastname, "COMPONENT_SAVE");
    inv.Screens.last = screens + 2; g_queries = 0;
    ok = ok && inv.IsSaveMenuReceivingInput() && g_queries == 4 && g_lists == 0 && g_strings == 0;   /* state 2 hit on second query */
    s1.items[0] = &active; g_queries = 0;
    ok = ok && inv.IsSaveMenuReceivingInput() && g_queries == 3 && g_lists == 0;                    /* state 1 hit on first query */
    inv.Screens.last = screens; ok = ok && !inv.IsSaveMenuReceivingInput();
    printf(ok ? "0058376c_TEST PASS\n" : "0058376c_TEST FAIL\n"); return ok ? 0 : 1;
}