// Behaviour test for 0x00591B7F CTCInventory::UpdateContextSensitiveItems (links the REAL row object: its vector erase / push_back /
// overflow / destructor COMDATs, the pair constructor and destructor, CDefinitionManager::GetDef<CThingBaseDef> and
// CDefClassBase::GetSubDef<CContextSensitiveItemDef> bodies and every CDefPointer expansion run as compiled).
// Fixtures (raw native layout, every offset read by an instruction of the row): CTCInventory ParentThing +0x04 (0x00591D91),
// ContextSensitiveItems +0x154 / +0x158 (0x00591DB4, 0x00591E07), ContextSensitiveFadeTimes +0x160 (0x1C-byte records, Opening +0x0D,
// 0x00591DAE), ContextSensitiveUpdate +0x1D4 (0x00591B8B), NumberOfCSOpinionExpressions +0x1D8 (0x00591BF8); the static
// ContextSensitivePreferredItems vector (12-byte records, ThingDefIndex +0); defs: vptr (slot 1 = Delete, slot 14 = the const
// GetPSubDefInfo), IVRefCount +0x04, PDefManager +0x1C, Type +0x28, SlotIndex +0x30, ExtraData1 +0x34; CSubDefInfo DefIndex +0.
// The out-of-line engine callees are provided here and log every call (receiver, arguments); std::sort is provided too (it reverses
// the range, so the row's later loop order is checked against a non-trivial permutation).
// Reference model (independent): see Model() below. After every case each def's reference count must be back at its baseline and
// no def may have been deleted.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class CCharString { public: char* P; CCharString(const char* s, long n); ~CCharString(); };
CCharString::CCharString(const char* s, long) { P = (char*)malloc(strlen(s) + 1); strcpy(P, s); }
CCharString::~CCharString() { free(P); P = 0; }

namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector { public: T* _M_start; T* _M_finish; T* _M_end_of_storage; };
template<class A, class B> struct pair { A first; B second; };
}
class CDefPointeeBase;
class CDefClassBase;
class CContextSensitiveItemDef;
class CThing;
class CWorld;
class CTCInventory;
template<class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer(CDefPointeeBase* o);
    CDefPointer(const CDefPointer& o);
    ~CDefPointer() { }
};

// ---- event log
struct Ev { char t; const void* p; long a, b, c, d; float f, g; };
static Ev g_log[256], g_exp[256]; static int g_n, g_en; static bool g_model;
static void Log(char t, const void* p = 0, long a = 0, long b = 0, long c = 0, long d = 0, float f = 0, float g = 0)
{
    Ev* arr = g_model ? g_exp : g_log; int& n = g_model ? g_en : g_n;
    if (n >= 256) return;
    Ev& e = arr[n++]; memset(&e, 0, sizeof e); e.t = t; e.p = p; e.a = a; e.b = b; e.c = c; e.d = d; e.f = f; e.g = g;
}

// ---- defs
struct SubInfo { long DefIndex; long OriginalParentDefIndex; };
static int g_deleted;
struct FakeDef {
    virtual void s0() { printf("FAIL def slot 0\n"); exit(3); }
    virtual void Delete() { ++g_deleted; }                                          // slot 1
    virtual void s2() { exit(3); } virtual void s3() { exit(3); } virtual void s4() { exit(3); } virtual void s5() { exit(3); }
    virtual void s6() { exit(3); } virtual void s7() { exit(3); } virtual void s8() { exit(3); } virtual void s9() { exit(3); }
    virtual void s10() { exit(3); } virtual void s11() { exit(3); } virtual void s12() { exit(3); } virtual void s13() { exit(3); }
    virtual const SubInfo* GetPSubDefInfo(const CCharString& name) const             // slot 14
    {
        Log('S', this, strcmp(name.P, "CContextSensitiveItemDef") == 0);
        return HasSub ? &Sub : 0;
    }
    long IVRefCount;             // +0x04
    unsigned char p08[0x14];
    void* PDefManager;           // +0x1C
    unsigned char p20[8];
    long Type;                   // +0x28
    long Priority;               // +0x2C
    long SlotIndex;              // +0x30
    long ExtraData1;             // +0x34
    long ExtraData2;             // +0x38
    // test-only data behind the native record
    SubInfo Sub; bool HasSub; bool Valid;
};
enum { NDEF = 10, BASE = 5, SUBOFF = 100 };
static FakeDef g_thingDef[NDEF], g_ctxDef[NDEF];
static unsigned char g_mgr[16];
static FakeDef* DefOf(long index)
{
    if (index >= 1 && index < NDEF) return &g_thingDef[index];
    if (index >= SUBOFF + 1 && index < SUBOFF + NDEF) return &g_ctxDef[index - SUBOFF];
    return 0;
}
template<class T> CDefPointer<T>::CDefPointer(CDefPointeeBase* o) : Object(o) { if (o) ++((FakeDef*)o)->IVRefCount; }
template<class T> CDefPointer<T>::CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++((FakeDef*)Object)->IVRefCount; }

class CDefinitionManager {
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);
public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;
};
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index) { Log('g', this, index); return CDefPointer<CDefClassBase>((CDefPointeeBase*)DefOf(index)); }
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index) const { Log('G', this, index); return CDefPointer<const CDefClassBase>((CDefPointeeBase*)DefOf(index)); }
class CGameDefinitionManager : public CDefinitionManager { };

class CContextSensitiveItemDef { public: bool IsValidOverride(const CThing& thing, long thing_def_index, const CTCInventory* inventory) const; };
bool CContextSensitiveItemDef::IsValidOverride(const CThing& thing, long thing_def_index, const CTCInventory* inventory) const
{
    Log('V', this, thing_def_index, (long)&thing, (long)inventory); return ((const FakeDef*)this)->Valid;
}

class CRGBColour { public: unsigned char B, G, R, A; };
class CFadeInFadeOutBase { public: void Open(float fade_in_time, float fade_out_time, CRGBColour to_colour); };
void CFadeInFadeOutBase::Open(float fade_in_time, float fade_out_time, CRGBColour to_colour)
{
    Log('P', this, to_colour.B, to_colour.G, to_colour.R, to_colour.A, fade_in_time, fade_out_time);
}

static long g_frame;
class CWorld { public: static long __fastcall GetFrame(); };
long __fastcall CWorld::GetFrame() { Log('F'); return g_frame; }
static unsigned char g_world[4];

class CTCBase {
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;
    const CWorld& PeekWorld() const;
};
const CGameDefinitionManager& CTCBase::GetDefinitionManager() const { Log('M', this); return *(const CGameDefinitionManager*)g_mgr; }
const CWorld& CTCBase::PeekWorld() const { Log('W', this); return *(const CWorld*)g_world; }

typedef std::pair<long, long> TItem;
static TItem g_items[16]; static int g_itemCount;
static long g_countOf[32];
namespace NInventory { class CTCInventoryBase { public:
    void GetAllItems(std::vector<TItem>& items) const;
    long GetNumberOfItemsOfTypeInInventory(long type) const;
}; }
void NInventory::CTCInventoryBase::GetAllItems(std::vector<TItem>& items) const
{
    Log('A', this, (long)items._M_start, (long)items._M_finish);
    if (g_itemCount == 0) return;                                        // leaves the empty vector alone (no storage to free)
    items._M_start = (TItem*)malloc(sizeof(TItem) * g_itemCount);
    memcpy(items._M_start, g_items, sizeof(TItem) * g_itemCount);
    items._M_finish = items._M_end_of_storage = items._M_start + g_itemCount;
}
long NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long type) const { Log('N', this, type); return g_countOf[type & 31]; }

static long g_buttonOf[32], g_slotOfButton[8];
class CTCInventory {
public:
    class CContextSensitiveItem { public: long ThingDefIndex, ButtonIndex, Priority; };
    void UpdateContextSensitiveItems();
    static long __fastcall GetContextSensitivePreferredItemButtonIndex(long type);
    long GetQuickAccessIndexFromButtonIndex(long button_index) const;
    static std::vector<CContextSensitiveItem>& Preferred() { return ContextSensitivePreferredItems; }
private:
    static std::vector<CContextSensitiveItem> ContextSensitivePreferredItems;
};
std::vector<CTCInventory::CContextSensitiveItem> CTCInventory::ContextSensitivePreferredItems;
long __fastcall CTCInventory::GetContextSensitivePreferredItemButtonIndex(long type) { Log('B', 0, type); return g_buttonOf[type & 31]; }
long CTCInventory::GetQuickAccessIndexFromButtonIndex(long button_index) const { Log('Q', this, button_index); return g_slotOfButton[button_index & 7]; }

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > TCtx;
class CContextSensitivePriorityCompare { };
namespace std {
template<class I, class C> void __fastcall sort(I first, I last, C comp);
template<> void __fastcall sort<TCtx*, CContextSensitivePriorityCompare>(TCtx* first, TCtx* last, CContextSensitivePriorityCompare)
{
    Log('O', 0, (long)(last - first));
    if (last - first < 2) return;
    for (TCtx* a = first, *b = last - 1; a < b; ++a, --b) { unsigned char t[sizeof(TCtx)]; memcpy(t, a, sizeof t); memcpy(a, b, sizeof t); memcpy(b, t, sizeof t); }
}
}

// ---- the inventory fixture
struct Fade { unsigned char b[0x1C]; };
static unsigned char g_inv[0x228];
static long g_slots[5]; static Fade g_fades[5]; static long g_opinion[7];
static CTCInventory::CContextSensitiveItem g_pref[4]; static unsigned char g_thing[4];
template<class T> static T& At(void* base, int off) { return *(T*)((unsigned char*)base + off); }

static unsigned long g_seed = 777;
static unsigned long Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 10; }

static int g_cases, g_fail;
static void Fail(const char* what, long a = 0, long b = 0) { if (g_fail++ < 12) printf("FAIL case %d: %s (%ld, %ld)\n", g_cases, what, a, b); }

// Reference model: expected log, slots, opinion counters, flag and preferred-item list.
static long m_slots[5], m_opinion[7]; static bool m_flag; static CTCInventory::CContextSensitiveItem m_pref[4]; static int m_prefCount;
static void Model(int prefCount, int slotCount)
{
    g_model = true; g_en = 0;
    int i;
    memcpy(m_slots, g_slots, sizeof m_slots); memcpy(m_opinion, g_opinion, sizeof m_opinion);
    m_flag = At<bool>(g_inv, 0x1D4); memcpy(m_pref, g_pref, sizeof m_pref); m_prefCount = prefCount;
    if (!m_flag)
    {
        Log('W', g_inv); Log('F');
        if (g_frame % 8 != 0) { g_model = false; return; }
    }
    for (i = 0; i < m_prefCount; )
    {
        Log('N', g_inv, m_pref[i].ThingDefIndex);
        if (g_countOf[m_pref[i].ThingDefIndex & 31] == 0) { for (int k = i; k + 1 < m_prefCount; ++k) m_pref[k] = m_pref[k + 1]; --m_prefCount; }
        else ++i;
    }
    Log('A', g_inv, 0, 0);
    for (i = 0; i < 6; ++i) m_opinion[i] = 0;
    struct C { long type; FakeDef* def; } cand[32]; int n = 0;
    for (i = 0; i < g_itemCount; ++i)
    {
        long type = g_items[i].first;
        Log('M', g_inv);
        if (type > 0) Log('G', g_mgr, type);
        FakeDef* base = DefOf(type);
        Log('S', base, 1);
        if (base->HasSub)
        {
            Log('g', g_mgr, base->Sub.DefIndex);
            FakeDef* ctx = DefOf(base->Sub.DefIndex);
            cand[n].type = type; cand[n].def = ctx; ++n;
            if (ctx->Type == 6 && ctx->ExtraData1 >= 0 && ctx->ExtraData1 < 6) ++m_opinion[ctx->ExtraData1];
        }
    }
    for (i = 0; i < m_prefCount; ++i) { cand[n].type = m_pref[i].ThingDefIndex; cand[n].def = 0; ++n; }
    Log('O', 0, n);
    for (int a = 0, b = n - 1; a < b; ++a, --b) { C t = cand[a]; cand[a] = cand[b]; cand[b] = t; }
    bool found[4] = { false, false, false, false };
    for (i = 0; i < n; ++i)
    {
        long type = cand[i].type, slot;
        Log('B', 0, type);
        long button = g_buttonOf[type & 31];
        if (button != -1) { Log('Q', g_inv, button); slot = g_slotOfButton[button & 7]; }
        else slot = cand[i].def->SlotIndex;
        if (slot < 0) continue;
        if (cand[i].def) { Log('V', cand[i].def, type, (long)g_thing, (long)g_inv); if (!cand[i].def->Valid) continue; }
        if (!g_fades[slot].b[0x0D] && m_slots[slot] != type && !found[slot])
        {
            m_slots[slot] = type;
            Log('P', &g_fades[slot], 0, 0, 0, 255, 1.0f, 0.0f);
        }
        found[slot] = true;
    }
    for (i = 0; i < slotCount; ++i) if (!found[i]) m_slots[i] = 0;
    m_flag = false;
    g_model = false;
}

static void Run(int prefCount, int slotCount)
{
    ++g_cases;
    int i;
    std::vector<CTCInventory::CContextSensitiveItem>& pv = CTCInventory::Preferred();
    static CTCInventory::CContextSensitiveItem store[5];
    memcpy(store, g_pref, sizeof g_pref); store[4].ThingDefIndex = 0x7E7E7E7E;
    pv._M_start = store; pv._M_finish = store + prefCount; pv._M_end_of_storage = store + 4;
    At<long*>(g_inv, 0x154) = g_slots; At<long*>(g_inv, 0x158) = g_slots + slotCount; At<long*>(g_inv, 0x15C) = g_slots + 4;
    At<Fade*>(g_inv, 0x160) = g_fades; At<Fade*>(g_inv, 0x164) = g_fades + 4; At<Fade*>(g_inv, 0x168) = g_fades + 4;
    At<long*>(g_inv, 0x1D8) = g_opinion; At<long*>(g_inv, 0x1DC) = g_opinion + 6; At<long*>(g_inv, 0x1E0) = g_opinion + 6;
    At<void*>(g_inv, 4) = g_thing;
    Model(prefCount, slotCount);
    Fade fades0[5]; memcpy(fades0, g_fades, sizeof fades0);
    g_n = 0; g_deleted = 0;
    ((CTCInventory*)g_inv)->UpdateContextSensitiveItems();
    if (g_deleted) Fail("def deleted", g_deleted);
    for (i = 1; i < NDEF; ++i)
    {
        if (g_thingDef[i].IVRefCount != BASE) Fail("thing def count", i, g_thingDef[i].IVRefCount);
        if (g_ctxDef[i].IVRefCount != BASE) Fail("context def count", i, g_ctxDef[i].IVRefCount);
    }
    if (memcmp(m_slots, g_slots, sizeof m_slots)) Fail("slots");
    if (memcmp(m_opinion, g_opinion, sizeof m_opinion)) Fail("opinion counters");
    if (memcmp(fades0, g_fades, sizeof fades0)) Fail("fade records written");
    if (At<bool>(g_inv, 0x1D4) != m_flag) Fail("update flag");
    if (pv._M_start != store || pv._M_finish - pv._M_start != m_prefCount) Fail("preferred count", (long)(pv._M_finish - pv._M_start), m_prefCount);
    else if (memcmp(store, m_pref, sizeof(m_pref[0]) * m_prefCount)) Fail("preferred items");
    if (store[4].ThingDefIndex != 0x7E7E7E7E) Fail("preferred overrun");
    if (g_n != g_en) { Fail("event count", g_n, g_en); return; }
    for (i = 0; i < g_n; ++i)
    {
        Ev a = g_log[i], b = g_exp[i];
        if (a.t == 'A') { a.a = a.b = 0; }                                   // the vector's address is the row's own local
        if (memcmp(&a, &b, sizeof a)) { Fail("event differs", i, a.t * 256 + b.t); return; }
    }
}

int main()
{
    if ((char*)&g_thingDef[0].IVRefCount - (char*)&g_thingDef[0] != 4 || (char*)&g_thingDef[0].PDefManager - (char*)&g_thingDef[0] != 0x1C ||
        (char*)&g_thingDef[0].Type - (char*)&g_thingDef[0] != 0x28 || (char*)&g_thingDef[0].SlotIndex - (char*)&g_thingDef[0] != 0x30 ||
        (char*)&g_thingDef[0].ExtraData1 - (char*)&g_thingDef[0] != 0x34 || sizeof(TCtx) != 8 || sizeof(CTCInventory::CContextSensitiveItem) != 12)
    { printf("FAIL fixture layout\n"); return 2; }
    memset(g_inv, 0xC3, sizeof g_inv);
    for (int n = 0; n < 6000; ++n)
    {
        int i;
        for (i = 1; i < NDEF; ++i)
        {
            FakeDef& t = g_thingDef[i]; FakeDef& c = g_ctxDef[i];
            t.IVRefCount = BASE; c.IVRefCount = BASE; t.PDefManager = g_mgr; c.PDefManager = g_mgr;
            t.HasSub = (Rnd() % 4) != 0; t.Sub.DefIndex = SUBOFF + i; t.Sub.OriginalParentDefIndex = 0;
            t.Type = 6; t.SlotIndex = 2; t.ExtraData1 = 1;                   // never read through the thing def
            c.HasSub = false; c.Valid = (Rnd() % 4) != 0;
            static const long kType[] = { 6, 6, 6, 5, 7, 0 };
            c.Type = kType[Rnd() % 6]; c.SlotIndex = (long)(Rnd() % 5) - 1; c.ExtraData1 = (long)(Rnd() % 9) - 2; c.Priority = Rnd() % 9; c.ExtraData2 = 0;
        }
        g_itemCount = Rnd() % 7;
        for (i = 0; i < g_itemCount; ++i) { g_items[i].first = 1 + Rnd() % (NDEF - 1); g_items[i].second = 1 + Rnd() % 3; }
        for (i = 0; i < 32; ++i) { g_countOf[i] = Rnd() % 3; g_buttonOf[i] = -1; }
        for (i = 1; i < NDEF; ++i) if (Rnd() % 4 == 0) g_buttonOf[i] = Rnd() % 8;
        int prefCount = Rnd() % 5;
        for (i = 0; i < 4; ++i)
        {
            g_pref[i].ThingDefIndex = 12 + i * 3 + Rnd() % 3; g_pref[i].ButtonIndex = Rnd() % 8; g_pref[i].Priority = Rnd() % 5;
            g_buttonOf[g_pref[i].ThingDefIndex] = g_pref[i].ButtonIndex;          // a preferred item always has a button
        }
        int slotCount = (Rnd() % 6 == 0) ? 3 : 4;
        for (i = 0; i < 8; ++i) g_slotOfButton[i] = (long)(Rnd() % (slotCount + 1)) - 1;
        for (i = 1; i < NDEF; ++i) if (g_ctxDef[i].SlotIndex >= slotCount) g_ctxDef[i].SlotIndex = slotCount - 1;
        for (i = 0; i < 5; ++i)
        {
            g_slots[i] = (Rnd() % 3 == 0) ? (long)(1 + Rnd() % (NDEF - 1)) : (long)(12 + Rnd() % 12);
            memset(&g_fades[i], 0x5A, sizeof(Fade)); g_fades[i].b[0x0D] = (Rnd() % 4 == 0) ? 1 : 0;
        }
        for (i = 0; i < 7; ++i) g_opinion[i] = 1000 + i;
        g_frame = (Rnd() % 3 == 0) ? (long)(Rnd() % 9) * 8 : (long)(Rnd() % 1000);
        At<bool>(g_inv, 0x1D4) = (Rnd() % 2) != 0;
        Run(prefCount, slotCount);
    }
    if (g_fail) { printf("00591b7f_TEST FAIL (%d of %d)\n", g_fail, g_cases); return 1; }
    printf("00591b7f_TEST PASS (%d cases)\n", g_cases);
    return 0;
}