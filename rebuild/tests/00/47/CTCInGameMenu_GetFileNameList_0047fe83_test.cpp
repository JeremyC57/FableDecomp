// Behaviour test for 0x0047FE83 CTCInGameMenu::GetFileNameList (links the REAL row object: the row body plus the TU's STLport vector
// COMDATs it carries -- erase(first, last), push_back, resize -- and the expanded EqualsCaseInsensitive / CDateAndTime / string code).
// Fixtures (native layout): the menu object (0xF8 bytes; ActiveScreen +0x18, FileNames +0x28, FileTimes +0x34, FileRegionNames +0x78,
// FileRegionMiniMapNames +0x84, FilePlayTimes +0x90, STLport 12-byte vectors of malloc'd storage); CWideString / CCharString stand-ins
// with counted heap payloads (CCharString's payload starts with its char pointer, as the inline GetAsCharArray reads it);
// CDateAndTime() marks its 7 fields -1; CAFile::GetFileModifiedTime stamps a hash of the path; the profile manager's sorted list, debug
// flag, autosave name, slot names and the path rule ("x..." names have no file) are scripted; AddRegionAndTimeInfo appends "R:"+path /
// "M:"+path / 77. vector::_M_fill_insert (resize's grow path, out of line) is a stub that appends n copies at the end.
// Reference model: Model() below, written from the row's statements; every run compares all five vectors element by element and the
// heap balance of strings and vector storage.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected call\n"); fflush(stdout); exit(3); }

#pragma comment(linker, "/alternatename:@CallRow@4=?GetFileNameList@CTCInGameMenu@@AAEXXZ")
#pragma comment(linker, "/alternatename:??0CWideString@@QAE@ABV0@@Z=@WsCopy@12")
#pragma comment(linker, "/alternatename:??0CWideString@@QAE@XZ=@WsDefault@8")
#pragma comment(linker, "/alternatename:??4CWideString@@QAEABV0@ABV0@@Z=@WsAssign@12")
#pragma comment(linker, "/alternatename:??1CWideString@@QAE@XZ=@WsDtor@8")
#pragma comment(linker, "/alternatename:??9CWideString@@QBE_NPBG@Z=@WsNe@12")
#pragma comment(linker, "/alternatename:?ToCharString@CWideString@@QBE?AVCCharString@@XZ=@WsToChar@12")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@ABV0@@Z=@CsCopy@12")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@CsFromText@16")
#pragma comment(linker, "/alternatename:??4CCharString@@QAEABV0@ABV0@@Z=@CsAssign@12")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@CsDtor@8")
#pragma comment(linker, "/alternatename:?NullChar@CCharString@@1DB=_NullCharStub")
#pragma comment(linker, "/alternatename:??0CDateAndTime@@QAE@XZ=@DtDefault@8")
#pragma comment(linker, "/alternatename:?GetFileModifiedTime@CAFile@@SI?AVCDateAndTime@@ABVCWideString@@@Z=@StubModified@8")
#pragma comment(linker, "/alternatename:?_M_fill_insert@?$vector@VCDateAndTime@@V?$allocator@VCDateAndTime@@@std@@@std@@QAEXPAVCDateAndTime@@IABV3@@Z=@FillDt@20")
#pragma comment(linker, "/alternatename:?_M_fill_insert@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAEXPAVCWideString@@IABV3@@Z=@FillWs@20")
#pragma comment(linker, "/alternatename:?Get@CUserProfileManager@@SIAAV1@XZ=@StubGet@0")
#pragma comment(linker, "/alternatename:?GetSortedSaveNamesAndTimes@CUserProfileManager@@QAEXAAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@AAV?$vector@VCDateAndTime@@V?$allocator@VCDateAndTime@@@std@@@3@@Z=@StubSorted@16")
#pragma comment(linker, "/alternatename:?IsDebugProfile@CUserProfileManager@@QBE_NXZ=@StubIsDebug@8")
#pragma comment(linker, "/alternatename:?GetEmptySlotName@CUserProfileManager@@QBE?AVCWideString@@XZ=@StubEmptySlot@12")
#pragma comment(linker, "/alternatename:?GetAutoSaveFileName@CUserProfileManager@@QBE?AVCWideString@@XZ=@StubAutoSave@12")
#pragma comment(linker, "/alternatename:?GetMaxSaveSlots@CUserProfileManager@@QBEJXZ=@StubMaxSlots@8")
#pragma comment(linker, "/alternatename:?GetSaveSlotFileName@CUserProfileManager@@QBEABVCWideString@@J@Z=@StubSlotName@12")
#pragma comment(linker, "/alternatename:?GetPathNameFromFileName@CUserProfileManager@@QBE?AVCWideString@@ABV2@_N@Z=@StubPath@20")
#pragma comment(linker, "/alternatename:?AddRegionAndTimeInfo@CTCInGameMenu@@AAEXABVCWideString@@@Z=@StubAddRegion@12")

static int g_fail = 0;
static void Check(bool ok, int line, int run) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, run); } }
static int g_run = 0;
#define CHECK(c) Check((c) ? true : false, __LINE__, g_run)

// --- string stand-ins (native one-pointer layouts) -----------------------------------------------------------------------
static int g_liveWs = 0, g_liveCs = 0, g_toChar = 0;
struct WS { wchar_t* p; };                                   // CWideString: PStringData (here: our own wide text, 0 = empty)
struct CSData { char* data; };                               // CCharString payload: Data's char pointer first
struct CS { CSData* p; };
extern "C" const char NullCharStub = 0;
static wchar_t* WDup(const wchar_t* s) { if (!s || !*s) return 0; size_t n = wcslen(s) + 1; wchar_t* r = (wchar_t*)malloc(n * sizeof(wchar_t)); memcpy(r, s, n * sizeof(wchar_t)); ++g_liveWs; return r; }
static void WFree(wchar_t* p) { if (p) { free(p); --g_liveWs; } }
static const wchar_t* WText(const WS* w) { return w->p ? w->p : L""; }
static CSData* CDup(const char* s) { if (!s || !*s) return 0; CSData* d = (CSData*)malloc(sizeof(CSData)); size_t n = strlen(s) + 1; d->data = (char*)malloc(n); memcpy(d->data, s, n); ++g_liveCs; return d; }
static void CFree(CSData* d) { if (d) { free(d->data); free(d); --g_liveCs; } }
static const char* CText(const CS* c) { return c->p ? c->p->data : ""; }

extern "C" WS* __fastcall WsCopy(WS* self, int, const WS* other) { self->p = WDup(other->p); return self; }
extern "C" WS* __fastcall WsDefault(WS* self, int) { self->p = 0; return self; }
extern "C" WS* __fastcall WsAssign(WS* self, int, const WS* other) { if (self != other) { wchar_t* n = WDup(other->p); WFree(self->p); self->p = n; } return self; }
extern "C" void __fastcall WsDtor(WS* self, int) { WFree(self->p); self->p = 0; }
extern "C" bool __fastcall WsNe(const WS* self, int, const wchar_t* text) { return wcscmp(WText(self), text) != 0; }
extern "C" CS* __fastcall WsToChar(const WS* self, int, CS* result)
{
    ++g_toChar;
    char buf[128]; const wchar_t* t = WText(self); int i = 0;
    for (; t[i] && i < 127; ++i) buf[i] = (char)t[i];
    buf[i] = 0; result->p = CDup(buf); return result;
}
extern "C" CS* __fastcall CsCopy(CS* self, int, const CS* other) { self->p = CDup(CText(other)); return self; }
extern "C" CS* __fastcall CsFromText(CS* self, int, const char* text, long length) { CHECK(length == -1); self->p = CDup(text); return self; }
extern "C" CS* __fastcall CsAssign(CS* self, int, const CS* other) { if (self != other) { CSData* n = CDup(CText(other)); CFree(self->p); self->p = n; } return self; }
extern "C" void __fastcall CsDtor(CS* self, int) { CFree(self->p); self->p = 0; }

struct DT { long f[7]; };                                    // CDateAndTime (0x1C)
extern "C" DT* __fastcall DtDefault(DT* self, int) { for (int i = 0; i < 7; ++i) self->f[i] = -1; return self; }
static long Hash(const wchar_t* s) { long h = 17; for (; *s; ++s) h = h * 31 + *s; return h & 0x7fffffff; }
extern "C" DT* __fastcall StubModified(DT* result, const WS* path) { for (int i = 0; i < 7; ++i) result->f[i] = Hash(WText(path)) + i; return result; }

// --- STLport vector layout --------------------------------------------------------------------------------------------------
template <class T> struct Vec { T* s; T* f; T* e; };
static int g_liveBlocks = 0;
template <class T> static void Grow(Vec<T>* v, size_t need)
{
    size_t n = v->f - v->s, cap = v->e - v->s;
    if (n + need <= cap) return;
    size_t nc = (n + need) * 2;
    T* b = (T*)malloc(nc * sizeof(T)); ++g_liveBlocks;
    if (n) memcpy(b, v->s, n * sizeof(T));                   // bitwise move of the one-pointer / POD elements
    if (v->s) { free(v->s); --g_liveBlocks; }
    v->s = b; v->f = b + n; v->e = b + nc;
}
// the row's own COMDATs allocate with malloc and free with free: count their blocks through the live element pointers at the end
extern "C" void __fastcall FillDt(Vec<DT>* v, int, DT* pos, unsigned n, const DT* x)
{
    CHECK(pos == v->f);
    DT copy = *x; Grow(v, n);
    for (unsigned i = 0; i < n; ++i) *v->f++ = copy;
}
extern "C" void __fastcall FillWs(Vec<WS>* v, int, WS* pos, unsigned n, const WS* x)
{
    CHECK(pos == v->f);
    wchar_t* text = x->p; Grow(v, n);
    for (unsigned i = 0; i < n; ++i) { v->f->p = WDup(text); ++v->f; }
}

// --- menu object and profile manager -----------------------------------------------------------------------------------------
struct Menu { unsigned char raw[0xf8]; };
#define AT(m, T, off) ((T*)((m)->raw + (off)))
static Menu g_menu;
struct Profile {
    int debug; const wchar_t* autosave; long max_slots; const wchar_t* slots[8];
    const wchar_t* sorted[8]; int nsorted;
} g_p;
static int g_profileObj;
extern "C" void* __fastcall StubGet() { return &g_profileObj; }
extern "C" void __fastcall StubSorted(void* self, int, Vec<WS>* names, Vec<DT>* times)
{
    CHECK(self == &g_profileObj && names == AT(&g_menu, Vec<WS>, 0x28) && times == AT(&g_menu, Vec<DT>, 0x34));
    CHECK(names->f == names->s && times->f == times->s);     // cleared first
    Grow(names, g_p.nsorted ? g_p.nsorted : 1); Grow(times, g_p.nsorted ? g_p.nsorted : 1);
    for (int i = 0; i < g_p.nsorted; ++i) {
        names->f->p = WDup(g_p.sorted[i]); ++names->f;
        for (int k = 0; k < 7; ++k) times->f->f[k] = 1000 + i; ++times->f;
    }
}
extern "C" bool __fastcall StubIsDebug(void*, int) { return g_p.debug != 0; }
extern "C" WS* __fastcall StubEmptySlot(void*, int, WS* r) { r->p = WDup(L"EMPTY"); return r; }
extern "C" WS* __fastcall StubAutoSave(void*, int, WS* r) { r->p = WDup(g_p.autosave); return r; }
extern "C" long __fastcall StubMaxSlots(void*, int) { return g_p.max_slots; }
static WS g_slotNames[8];
extern "C" const WS* __fastcall StubSlotName(void*, int, long i) { CHECK(i >= 0 && i < g_p.max_slots); return &g_slotNames[i]; }
extern "C" WS* __fastcall StubPath(void*, int, WS* r, const WS* name, bool full)
{
    CHECK(!full);
    const wchar_t* t = WText(name);
    if (!*t || t[0] == L'x') { r->p = 0; return r; }        // "x..." saves have no file
    wchar_t buf[128]; swprintf(buf, L"C:\\Saves\\%s", t); r->p = WDup(buf); return r;
}
extern "C" void __fastcall StubAddRegion(void* self, int, const WS* path)
{
    CHECK(self == &g_menu);
    char a[160], b[160]; const wchar_t* t = WText(path); int i = 0;
    for (; t[i] && i < 150; ++i) a[i + 2] = b[i + 2] = (char)t[i];
    a[i + 2] = b[i + 2] = 0; a[0] = 'R'; b[0] = 'M'; a[1] = b[1] = ':';
    Vec<CS>* rn = AT(&g_menu, Vec<CS>, 0x78); Vec<CS>* mn = AT(&g_menu, Vec<CS>, 0x84); Vec<long>* pt = AT(&g_menu, Vec<long>, 0x90);
    Grow(rn, 1); rn->f->p = CDup(a); ++rn->f;
    Grow(mn, 1); mn->f->p = CDup(b); ++mn->f;
    Grow(pt, 1); *pt->f++ = 77;
}
extern "C" void __fastcall CallRow(Menu* self);

// --- reference model ---------------------------------------------------------------------------------------------------------
struct Expect { int toChar; int n; wchar_t names[64][64]; long time0[64]; int ntimes; char region[64][160]; char mini[64][160]; long play[64]; int nextra; };
static Expect g_ex;
static void ExName(int i, const wchar_t* s) { wcscpy(g_ex.names[i], s); }
static void ExBlank() { int k = g_ex.nextra++; g_ex.region[k][0] = 0; g_ex.mini[k][0] = 0; g_ex.play[k] = 0; }
static void ExAdd(const wchar_t* path) { int k = g_ex.nextra++; sprintf(g_ex.region[k], "R:%S", path); sprintf(g_ex.mini[k], "M:%S", path); g_ex.play[k] = 77; }
static bool HasFile(const wchar_t* n, wchar_t* path) { if (!*n || n[0] == L'x') return false; swprintf(path, L"C:\\Saves\\%s", n); return true; }
static void Fill(int i, const wchar_t* name, bool timeFirst)
{
    wchar_t path[128];
    if (*name) {
        ExName(i, name);
        if (HasFile(name, path)) { g_ex.time0[i] = Hash(path); ExAdd(path); (void)timeFirst; }
        else { g_ex.time0[i] = -1; ExBlank(); }
    } else { ExName(i, L"EMPTY"); g_ex.time0[i] = -1; ExBlank(); }
}
static bool NoCase(const wchar_t* a, const wchar_t* b) { for (;; ++a, ++b) { wchar_t x = towlower(*a), y = towlower(*b); if (x != y) return false; if (!x) return true; } }
static void Model(long screen)
{
    memset(&g_ex, 0, sizeof g_ex);
    if (g_p.debug) {
        for (int i = 0; i < g_p.nsorted; ++i) Fill(i, g_p.sorted[i], false);
        ExName(g_p.nsorted, L"EMPTY"); g_ex.time0[g_p.nsorted] = -1; ExBlank();
        g_ex.n = g_ex.ntimes = g_p.nsorted + 1;
        return;
    }
    bool autosave = false;
    if (screen == 3 || screen == 0)
        for (int i = 0; i < g_p.nsorted; ++i) { g_ex.toChar += 2; if (NoCase(g_p.sorted[i], g_p.autosave)) { autosave = true; break; } }
    int off = autosave ? 1 : 0, count = (int)g_p.max_slots + off;
    for (int i = 0; i < count && i < g_p.nsorted; ++i) { ExName(i, g_p.sorted[i]); g_ex.time0[i] = 1000 + i; }   // kept by resize
    for (int i = g_p.nsorted; i < count; ++i) { ExName(i, L""); g_ex.time0[i] = -1; }
    if (autosave) Fill(0, g_p.autosave, true);
    for (int i = off; i < count; ++i) Fill(i, g_p.slots[i - off], true);
    g_ex.n = g_ex.ntimes = count;
}

// --- driver --------------------------------------------------------------------------------------------------------------------
static void Release()
{
    Vec<WS>* fn = AT(&g_menu, Vec<WS>, 0x28); Vec<DT>* ft = AT(&g_menu, Vec<DT>, 0x34);
    Vec<CS>* rn = AT(&g_menu, Vec<CS>, 0x78); Vec<CS>* mn = AT(&g_menu, Vec<CS>, 0x84); Vec<long>* pt = AT(&g_menu, Vec<long>, 0x90);
    for (WS* w = fn->s; w != fn->f; ++w) WFree(w->p);
    for (CS* c = rn->s; c != rn->f; ++c) CFree(c->p);
    for (CS* c = mn->s; c != mn->f; ++c) CFree(c->p);
    void* blocks[5] = { fn->s, ft->s, rn->s, mn->s, pt->s };
    for (int i = 0; i < 5; ++i) if (blocks[i]) free(blocks[i]);
    memset(&g_menu, 0, sizeof g_menu);
}
static void Run(int run, int debug, long screen, const wchar_t* autosave, long maxSlots, const wchar_t* const* sorted, int nsorted, const wchar_t* const* slots, bool prefill)
{
    g_run = run;
    memset(&g_p, 0, sizeof g_p);
    g_p.debug = debug; g_p.autosave = autosave; g_p.max_slots = maxSlots; g_p.nsorted = nsorted;
    for (int i = 0; i < nsorted; ++i) g_p.sorted[i] = sorted[i];
    for (int i = 0; i < maxSlots; ++i) { g_p.slots[i] = slots[i]; WFree(g_slotNames[i].p); g_slotNames[i].p = WDup(slots[i]); }
    *AT(&g_menu, long, 0x18) = screen;
    if (prefill) {                                           // stale entries the row must clear (and free)
        Vec<WS>* fn = AT(&g_menu, Vec<WS>, 0x28); Vec<CS>* rn = AT(&g_menu, Vec<CS>, 0x78); Vec<long>* pt = AT(&g_menu, Vec<long>, 0x90);
        Vec<DT>* ft = AT(&g_menu, Vec<DT>, 0x34); Vec<CS>* mn = AT(&g_menu, Vec<CS>, 0x84);
        Grow(fn, 2); fn->f->p = WDup(L"old1"); ++fn->f; fn->f->p = WDup(L"old2"); ++fn->f;
        Grow(ft, 2); ft->f += 2;
        Grow(rn, 1); rn->f->p = CDup("oldR"); ++rn->f;
        Grow(mn, 1); mn->f->p = CDup("oldM"); ++mn->f;
        Grow(pt, 1); *pt->f++ = 5;
    }
    g_toChar = 0;
    CallRow(&g_menu);
    Model(screen);
    Vec<WS>* fn = AT(&g_menu, Vec<WS>, 0x28); Vec<DT>* ft = AT(&g_menu, Vec<DT>, 0x34);
    Vec<CS>* rn = AT(&g_menu, Vec<CS>, 0x78); Vec<CS>* mn = AT(&g_menu, Vec<CS>, 0x84); Vec<long>* pt = AT(&g_menu, Vec<long>, 0x90);
    CHECK(g_toChar == g_ex.toChar);                          // the autosave search stops at the first match
    CHECK(fn->f - fn->s == g_ex.n && ft->f - ft->s == g_ex.ntimes);
    for (int i = 0; i < g_ex.n && i < fn->f - fn->s; ++i) {
        CHECK(wcscmp(WText(&fn->s[i]), g_ex.names[i]) == 0);
        CHECK(ft->s[i].f[0] == g_ex.time0[i]);
        for (int k = 1; k < 7; ++k) CHECK(ft->s[i].f[k] == (g_ex.time0[i] == -1 ? -1 : g_ex.time0[i] >= 1000 && g_ex.time0[i] < 1064 ? g_ex.time0[i] : g_ex.time0[i] + k));
    }
    CHECK(rn->f - rn->s == g_ex.nextra && mn->f - mn->s == g_ex.nextra && pt->f - pt->s == g_ex.nextra);
    for (int k = 0; k < g_ex.nextra && k < rn->f - rn->s; ++k) {
        CHECK(strcmp(CText(&rn->s[k]), g_ex.region[k]) == 0);
        CHECK(strcmp(CText(&mn->s[k]), g_ex.mini[k]) == 0);
        CHECK(pt->s[k] == g_ex.play[k]);
    }
    Release();
    int slotStrings = 0; for (int i = 0; i < 8; ++i) if (g_slotNames[i].p) ++slotStrings;
    CHECK(g_liveWs == slotStrings && g_liveCs == 0);         // every temporary path / name / char string released
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    static const wchar_t* const s1[] = { L"Save003", L"", L"xGone", L"Autosave" };
    static const wchar_t* const slots1[] = { L"Save001", L"", L"xBroken", L"Save004", L"Save005" };
    static const wchar_t* const s2[] = { L"Save001", L"AUTOSAVE" };
    static const wchar_t* const s3[] = { L"Save001" };
    static const wchar_t* const slots3[] = { L"Save009", L"xLost" };
    Run(1, 1, 3, L"Autosave", 5, s1, 4, slots1, true);       // debug profile: every sorted name, then an appended empty slot
    Run(2, 1, 2, L"Autosave", 5, s1, 0, slots1, false);      // debug profile, no saves
    Run(3, 0, 3, L"Autosave", 5, s1, 4, slots1, true);       // load screen, autosave listed: slot 0 autosave + 5 slots
    Run(4, 0, 0, L"autosave", 5, s2, 2, slots1, false);      // in-game menu screen, case-insensitive autosave match
    Run(5, 0, 2, L"Autosave", 5, s1, 4, slots1, false);      // save screen: no autosave search
    Run(6, 0, 3, L"Autosave", 2, s3, 1, slots3, true);       // autosave not listed, fewer sorted than slots
    Run(7, 0, 0, L"xAuto", 2, s1, 4, slots3, false);         // autosave listed (no case match) -> not found
    static const wchar_t* const s8[] = { L"xAuto" };
    Run(8, 0, 0, L"xAuto", 2, s8, 1, slots3, false);         // autosave found but has no file: blank region entry at 0
    static const wchar_t* const s9[] = { L"Save002", L"AutoSave", L"Save007", L"autosave" };
    Run(9, 0, 3, L"Autosave", 5, s9, 4, slots1, true);       // autosave matched mid-list: the search stops there
    if (g_fail) { printf("0047fe83_TEST FAIL\n"); return 1; }
    printf("0047fe83_TEST PASS\n");
    return 0;
}