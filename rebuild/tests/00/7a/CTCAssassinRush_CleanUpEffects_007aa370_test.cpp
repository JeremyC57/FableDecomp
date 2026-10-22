// Behaviour test for 007aa370: final.cpp copied verbatim, the declared callees stubbed below so every call and the
// list clearing are observable. CTCAssassinRush::CleanUpEffects
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define _snprintf snprintf
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[4096];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CTCAssassinRush::CleanUpEffects() -- end the rush effects: drop the parent's rush creature mode (0x25), restore full alpha on the parent and on every living faded item, clear the faded-item list, and end the player's special-move mode when the parent is a living player-controlled thing
// Leaf: ?CleanUpEffects@CTCAssassinRush
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AA370, 268 bytes (+4 int3 padding), `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined lookup of CTCCreatureModeManager, type
//   0x31), CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0, CTCGraphicAppearance::SetAlpha(unsigned char)
//   0x004BFAC0, CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90, CThing::GetPlayer 0x004C7A30, the
//   player's mode-0x12 exit 0x00487150 (RemoveMode(0x12) plus a flag clear; descriptive name, manifest label
//   CHTG_Executioner::OnPredicateFail is propagated).
// Layout: CTCBase ParentThing +4; ItemAlphaList +0x64 (12-byte STLport vector of 8-byte CIntelligentPointer<CThing>;
//   Ego_r.pdb +0x64); CThing state byte +0x10 (2 = player controlled), graphic appearance +0x64, dead bit +0x91 bit 0.
// Shape: the parent is a named local (spilled for the final test); the mode lookup is the out-parameter GetInterface;
//   the alpha restore is an inline CThing::SetAlpha (one appearance load for the test and the call); the alpha loop
//   caches the end pointer and uses the inline IS_THING_ALIVE; copy/_Destroy are `__forceinline`; the list is cleared through STLport
//   erase(begin(), end()) = the inlined counted copy (intelligent-pointer operator=), the virtual destructor of each
//   dropped element (`push 0 ; call [vtbl]`) and the finish store.
// Count shape: clear() names its two iterator locals in evaluation order (`iterator last = end(); iterator first =
//   begin();`) and copies from `last.p` to `_M_finish`: the finish load then defines the copy-source variable (esi) and the
//   `finish - last` count is a copy of it (`mov esi,[ebx+68h] ; mov edi,[ebx+64h] ; mov eax,esi ; sub eax,esi`); through
//   erase(begin(), end()) the load is the count temp and the variable its copy (`mov eax,.. ; mov esi,eax ; sub eax,eax`).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCGraphicAppearance {
    void SetAlpha(unsigned char alpha);         // 0x004BFAC0
};
struct CPlayer {
    void EndSpecialMoveMode();                  // 0x00487150
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char m_state;                      // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* m_pGraphicAppearance; // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_stateFlags;                 // +0x91 (bit 0: dead)
    CPlayer* GetPlayer();                       // 0x004C7A30
    void SetAlpha(unsigned char alpha)
    {
        if (m_pGraphicAppearance)
            m_pGraphicAppearance->SetAlpha(alpha);
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCCreatureModeManager {
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_stateFlags & 1))
            return true;
    }
    return false;
}
template <class T> __forceinline T* copy(T* first, T* last, T* result)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class T> __forceinline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    struct iterator {                           // the iterator object (not CSE'd with the member loads)
        T* p;
        explicit iterator(T* q) : p(q) {}
    };
    T* begin_ptr() { return _M_start; }
    T* end_ptr() { return _M_finish; }
    iterator begin() { return iterator(_M_start); }
    iterator end() { return iterator(_M_finish); }
    __forceinline iterator erase(iterator first, iterator last)
    {
        T* i = copy(last.p, _M_finish, first.p);
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
    __forceinline void clear()
    {
        iterator last = end();
        iterator first = begin();
        T* i = copy(last.p, _M_finish, first.p);
        _Destroy(i, _M_finish);
        _M_finish = i;
    }
};

class CTCAssassinRush {
public:
    void*         __vftable;                    // +0x00 (CTCBase)
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x64 - 0x08];
    CRetailVector< CIntelligentPointer<CThing> > ItemAlphaList;   // +0x64
    void CleanUpEffects();
};

void CTCAssassinRush::CleanUpEffects()
{
    CThing* parent = ParentThing;
    CTCCreatureModeManager* modes;
    if (parent->GetInterface(&modes, 0x31))
        modes->RemoveMode(0x25);
    parent->SetAlpha(0xff);
    for (CIntelligentPointer<CThing>* it = ItemAlphaList.begin_ptr(), *last = ItemAlphaList.end_ptr(); it != last; ++it) {
        if (IS_THING_ALIVE(*it))
            it->GetPItem()->SetAlpha(0xff);
    }
    ItemAlphaList.clear();
    if (!(parent->m_stateFlags & 1) && parent->m_state == 2)
        parent->GetPlayer()->EndSpecialMoveMode();
}
// ---- stubs ----
static CThing g_parent, g_itemA, g_itemB;
static CTCGraphicAppearance g_parentApp, g_itemApp;
static CPlayer g_player;
static const char* ThingName(const CThing* t) { return t == &g_parent ? "parent" : t == &g_itemA ? "A" : t == &g_itemB ? "B" : "?"; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
void CTCGraphicAppearance::SetAlpha(unsigned char alpha) { char b[48]; _snprintf(b, sizeof b, "alpha(%s,%d)", this == &g_parentApp ? "parent" : "item", (int)alpha); note(b); }
void CPlayer::EndSpecialMoveMode() { note("endmove"); }
CPlayer* CThing::GetPlayer() { note("player"); return &g_player; }
void CTCCreatureModeManager::RemoveMode(unsigned long mode) { char b[32]; _snprintf(b, sizeof b, "remove(%lx)", mode); note(b); }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { char b[48]; _snprintf(b, sizeof b, "dtor(%s)", m_pItem ? ThingName(m_pItem) : "null"); note(b); }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; note("set"); }
int main()
{
    CTCAssassinRush rush; memset(&rush, 0, sizeof rush); rush.ParentThing = &g_parent;
    memset(&g_parent, 0, sizeof g_parent); memset(&g_itemA, 0, sizeof g_itemA); memset(&g_itemB, 0, sizeof g_itemB);
    CTCCreatureModeManager modes;
    CInterfaceMap::SEntry entries[1]; entries[0].type = 0x31; entries[0].component = (CTCBase*)&modes;
    g_parent.m_interfaces.first = entries; g_parent.m_interfaces.last = entries + 1;
    // 1. everything: mode manager present, appearances, three items (A alive with appearance, B dead, one empty), player-controlled parent
    g_parent.m_presentBits[1] |= 1UL << 17; g_parent.m_pGraphicAppearance = &g_parentApp; g_parent.m_state = 2;
    g_itemA.m_pGraphicAppearance = &g_itemApp; g_itemB.m_stateFlags = 1; g_itemB.m_pGraphicAppearance = &g_itemApp;
    {
        CIntelligentPointer<CThing> items[3]; items[0].m_pItem = &g_itemA; items[1].m_pItem = &g_itemB; items[2].m_pItem = 0;
        rush.ItemAlphaList._M_start = items; rush.ItemAlphaList._M_finish = items + 3; rush.ItemAlphaList._M_end_of_storage = items + 3;
        g_log[0] = 0; rush.CleanUpEffects();
        CHECKLOG("remove(25);alpha(parent,255);alpha(item,255);dtor(A);dtor(B);dtor(null);player;endmove;");
        CHECK(rush.ItemAlphaList._M_finish == items && rush.ItemAlphaList._M_start == items);
        g_log[0] = 0;
    }
    // 2. nothing present: no mode manager, no appearance, empty list, dead parent
    g_parent.m_presentBits[1] = 0; g_parent.m_pGraphicAppearance = 0; g_parent.m_stateFlags = 1;
    rush.ItemAlphaList._M_start = rush.ItemAlphaList._M_finish = 0;
    g_log[0] = 0; rush.CleanUpEffects(); CHECKLOG("");
    // 3. alive parent that is not player controlled: no player call; an item without an appearance restores nothing
    g_parent.m_stateFlags = 0; g_parent.m_state = 1; g_itemA.m_pGraphicAppearance = 0;
    {
        CIntelligentPointer<CThing> items[1]; items[0].m_pItem = &g_itemA;
        rush.ItemAlphaList._M_start = items; rush.ItemAlphaList._M_finish = items + 1;
        g_log[0] = 0; rush.CleanUpEffects(); CHECKLOG("dtor(A);"); CHECK(rush.ItemAlphaList._M_finish == items);
        g_log[0] = 0;
    }
    if (fails == 0) printf("007aa370_TEST PASS\n"); else printf("007aa370_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}