// Behaviour test for 005b4795: src/005b4795.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryClothing::SetUpSelectedClothes()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryClothing::SetUpSelectedClothes() -- rebuilds the worn-clothing map: every selected item that is not a whole suit and whose appearance modifier names a suit part is recorded as that part's item
// Retail 0x005B4795, 269 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: map<EClothingSuitPart, long>::clear 0x004DB795 (manifest "CTCCarryable::OnKill" is propagated),
// CTCInventoryBase::GetAllSelectedItems(vector<long>*) 0x005BEAF5, CTCBase::GetDefinitionManager 0x006869D0,
// CDefinitionManager::GetDef<T>(long, CDefPointer*) 0x0042AF3C, CDefClassBase::GetSubDef<CHeroSuitDef> 0x005935C4 and
// GetSubDef<CAppearanceModifierDef> 0x0057FF92, map<EClothingSuitPart, long>::operator[](const key&) 0x005BBBF3, free
// 0x00BFEA14, the defs' slot 1 on release.
// Shape: the selected item vector is a zeroed local freed inline at the end; each positive item fetches its def, then
// (unless it is a suit) its appearance modifier into the def's now-dead out slot; a non-zero suit part maps to the item
// (the element address is formed before the map call); the modifier is released without a null test, the suit and the
// def with one (the shared zero register `esi`).
// Layout (retail): CurrentClothingSelected map +0x140 (as the landed ctor 0x005B7049); CAppearanceModifierDef suit part
// +0x4C.
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);           // 0x00BFEA14

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other);
    ~CDefPointer() { if (Object) Object->Release(); }
};
struct CHeroSuitDef : CDefPointeeBase {};
struct CAppearanceModifierDef : CDefPointeeBase {
    unsigned char _pad_0x08[0x4c - 0x08];
    long SuitPart;                                   // +0x4c EClothingSuitPart
};
template <class T> struct CDefPointerSlot {         // the out-parameter view of a CDefPointer (released by hand)
    T* Object;
};
struct CThingDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CHeroSuitDef>* out);            // 0x005935C4
    bool GetSubDef(CDefPointerSlot<CAppearanceModifierDef>* out);   // 0x0057FF92
};
void* FindDef_s7r_0();
long TmplSink_s7_0(const void* self, long seen);   // stand-in: the work FindDef_s7r_0 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_0 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_0()
{
    long r = TmplSink_s7_0(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
__declspec(noinline) bool CThingDef::GetSubDef(CDefPointer<CHeroSuitDef>* out)
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
void* FindDef_s7r_1();
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindDef_s7r_1 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_1 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_1()
{
    long r = TmplSink_s7_1(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
__declspec(noinline) bool CThingDef::GetSubDef(CDefPointerSlot<CAppearanceModifierDef>* out)
{
    void* def = FindDef_s7r_1();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CDefinitionManager {
    bool GetDef(long index, CDefPointerSlot<CThingDef>* out);   // 0x0042AF3C
};
void* FindDef_s7r_2();
long TmplSink_s7_2(const void* self, long seen);   // stand-in: the work FindDef_s7r_2 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_2 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_2()
{
    long r = TmplSink_s7_2(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
__declspec(noinline) bool CDefinitionManager::GetDef(long p0, CDefPointerSlot<CThingDef>* out)
{
    void* def = FindDef_s7r_2();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    __forceinline ~CLongVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    long& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CSuitPartMap {                                // map<EClothingSuitPart, long>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    void clear();                                    // 0x004DB795
    long& operator[](const long& part);              // 0x005BBBF3
};

struct CTCInventoryClothing {
    unsigned char _pad_0x000[0x140];
    CSuitPartMap CurrentClothingSelected;            // +0x140
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    void GetAllSelectedItems(CLongVector* items);    // 0x005BEAF5 (CTCInventoryBase)
    void SetUpSelectedClothes();
};

void CTCInventoryClothing::SetUpSelectedClothes()
{
    CurrentClothingSelected.clear();
    CLongVector selected;
    GetAllSelectedItems(&selected);
    for (unsigned long i = 0; i < selected.size(); ++i) {
        if (selected[i] > 0) {
            CDefPointerSlot<CThingDef> def;
            def.Object = 0;
            GetDefinitionManager()->GetDef(selected[i], &def);
            CThingDef* thingDef = def.Object;
            {
                CDefPointer<CHeroSuitDef> suit;
                if (!thingDef->GetSubDef(&suit)) {
                    CDefPointerSlot<CAppearanceModifierDef> modifierSlot;
                    modifierSlot.Object = 0;
                    thingDef->GetSubDef(&modifierSlot);
                    CAppearanceModifierDef* modifier = modifierSlot.Object;
                    if (modifier->SuitPart != 0)
                        CurrentClothingSelected[modifier->SuitPart] = selected[i];
                    modifier->Release();
                }
            }
            if (thingDef)
                thingDef->Release();
        }
    }
}

// ---- stubs and driver ----
#include <cstdlib>
static void* g_suit[16]; static void* g_mod[16]; static void* g_thing[16]; static long g_lastIndex = -1; static long g_curItem = 0;
static int g_release = 0, g_destroy = 0;
static CDefinitionManager g_dm;
long TmplSink_s7_0(const void*, long) { return (long)g_suit[g_curItem & 15]; }     // the suit sub-def of the current thing def
long TmplSink_s7_1(const void*, long) { return (long)g_mod[g_curItem & 15]; }      // its appearance-modifier sub-def
static long g_order[3] = { 5, 7, 9 }; static int g_next = 0;
long TmplSink_s7_2(const void*, long) { g_curItem = g_order[g_next++ % 3]; return (long)g_thing[g_curItem & 15]; }    // the thing def for the next selected index
void ReleaseDef_s7(void*) { ++g_release; }
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() { ++g_destroy; }
static long g_mapPart[8]; static int g_mapSet = 0, g_clear = 0;
long& CSuitPartMap::operator[](const long& part) { ++g_mapSet; return g_mapPart[part & 7]; }
CDefinitionManager* CTCInventoryClothing::GetDefinitionManager() const { return &g_dm; }
void CSuitPartMap::clear() { ++g_clear; for (int i = 0; i < 8; ++i) g_mapPart[i] = 0; }
static long g_items[4] = { 5, -1, 7, 9 };
void CTCInventoryClothing::GetAllSelectedItems(CLongVector* items)
{ items->_M_start = (long*)malloc(sizeof(g_items)); memcpy(items->_M_start, g_items, sizeof(g_items)); items->_M_finish = items->_M_start + 4; items->_M_end_of_storage = items->_M_finish; }
int main()
{
    static CThingDef t5, t7, t9; t5.RefCount = 9; t7.RefCount = 9; t9.RefCount = 9;
    static CHeroSuitDef suit7; suit7.RefCount = 9;
    static CAppearanceModifierDef m5, m9; m5.RefCount = 9; m5.SuitPart = 3; m9.RefCount = 9; m9.SuitPart = 0;
    g_thing[5] = &t5; g_thing[7] = &t7; g_thing[9] = &t9; g_suit[7] = &suit7; g_mod[5] = &m5; g_mod[9] = &m9;
    static CTCInventoryClothing inv;
    inv.SetUpSelectedClothes();
    CHECK(g_clear == 1 && g_next == 3);                 // three positive items looked up (the -1 skipped)
    CHECK(g_mapSet == 1 && g_mapPart[3] == 5);          // item 5's modifier names suit part 3; item 9's part 0 is not recorded; 7 is a whole suit
    CHECK(t5.RefCount == 8 && t7.RefCount == 8 && t9.RefCount == 8);   // each thing def released once
    CHECK(m5.RefCount == 8 && m9.RefCount == 8 && suit7.RefCount == 8);
    CHECK(g_release == 0 && g_destroy == 0);
    if (fails == 0) printf("005b4795_TEST PASS\n"); else printf("005b4795_TEST FAIL\n");
    return fails;
}