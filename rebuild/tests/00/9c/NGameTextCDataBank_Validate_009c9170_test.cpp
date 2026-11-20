// Behaviour test for NGameText::CDataBank::Validate (0x009C9170): links the row object; its same-TU GetPTextEntry (with IsLoaded /
// GetTextInfo / BeginLoadingText) and the expanded GetText / GetTextInfo run for real; GetPGroupEntry and the other out-of-line callees
// are /alternatename stand-ins that log every call. Fixtures: the bank (Entries CArray +0x1E0, ResourceList +0x168), group entries
// (vptr, IVRefCount +4, Entries CArray<long> +0x1C) and text entries (vptr, IVRefCount +4, Text +0x28, PFileData +0x38), each with a
// vtable whose slot 1 (Delete) logs. Checked: the whole call sequence (this / arguments), the group temporary and text pointer releases
// (Delete only when a count reaches zero), each CWideString copy (from the entry's Text, destroyed once), and that the bank, records,
// groups and texts end as they started (a first-loaded record keeps the text the BuildText stand-in gave it).
#include <stdio.h>
#include <string.h>
extern "C" void __fastcall RowValidate(void* self, int unused);
#pragma comment(linker, "/alternatename:@RowValidate@8=?Validate@CDataBank@NGameText@@QAEXXZ")
enum { TYPE, GROUP, ASYNC, FINISHED, LOAD, BUILD, FREE, REMOVE, INSERT, COPY, DTOR, DEL };
static const char* kNames[] = { "GetEntryDataType", "GetPGroupEntry", "AsyncUpdatePending", "IsFinished", "BeginLoading", "BuildText",
    "MakeFreeSpace", "ListRemove", "ListInsert", "CWideString(copy)", "~CWideString", "Delete" };
struct Ev { int kind; const void* self; unsigned long a; const void* b; };
static Ev g_log[128]; static int g_n;
static void Log(int k, const void* s, unsigned long a, const void* b) { if (g_n < 128) { g_log[g_n].kind = k; g_log[g_n].self = s; g_log[g_n].a = a; g_log[g_n].b = b; } ++g_n; }

enum { N = 9 };
static unsigned long g_types[N]; static void* g_entries[N]; static unsigned char* g_build_text[N];
static unsigned char g_bank[0x220];
static unsigned char g_text[N][0x44]; static void* g_record[N][2]; static unsigned char g_group[N][0x38]; static long g_ids[N][4];
static unsigned long g_last_index;
extern "C" unsigned long __fastcall StubType(const void* s, int, unsigned long i) { Log(TYPE, s, i, 0); return g_types[i]; }
extern "C" void** __fastcall StubGroup(const void* s, int, void** result, unsigned long i, bool wait)
{
    Log(GROUP, s, i, (const void*)(unsigned long)wait);
    unsigned char* g = g_group[i]; *result = g; ++*(unsigned long*)(g + 4);              // every index has a fixture group (empty unless a group)
    return result;
}
extern "C" void __fastcall StubAsync(void* s, int) { Log(ASYNC, s, 0, 0); }
extern "C" bool __fastcall StubFinished(const void* s, int) { Log(FINISHED, s, 0, 0); return true; }
extern "C" void __fastcall StubLoad(const void* s, int, unsigned long i, void* bank) { Log(LOAD, s, i, bank); g_last_index = i; }
extern "C" void __fastcall StubBuild(const void* s, int) { Log(BUILD, s, 0, 0); if (!*(void* const*)s) *(void**)s = g_build_text[g_last_index]; }
extern "C" bool __fastcall StubFree(void* s, int, unsigned long n) { Log(FREE, s, n, 0); return true; }
extern "C" void __fastcall StubRemove(void* s, int) { Log(REMOVE, s, 0, 0); }
extern "C" void __fastcall StubInsert(void* s, int, void* prev, void* next) { Log(INSERT, s, (unsigned long)prev, next); }
extern "C" void* __fastcall StubCopy(void* s, int, const void* src) { Log(COPY, s, 0, src); *(const void**)s = *(const void* const*)src; return s; }
extern "C" void __fastcall StubDtor(void* s, int) { Log(DTOR, s, 0, 0); }
extern "C" void __fastcall StubDelete(const void* s, int) { Log(DEL, s, 0, 0); }
extern "C" void __fastcall StubDeletingDtor(void* s, int, unsigned) { Log(DEL, s, 99, 0); }
#pragma comment(linker, "/alternatename:?GetEntryDataType@CBankFile@@QBEKK@Z=@StubType@12")
#pragma comment(linker, "/alternatename:?GetPGroupEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCGroupEntry@NGameText@@@@K_N@Z=@StubGroup@20")
#pragma comment(linker, "/alternatename:?AsyncUpdatePending@CBankFileAsync@@QAEXXZ=@StubAsync@8")
#pragma comment(linker, "/alternatename:?IsFinished@CBankFileAsyncData@@QBE_NXZ=@StubFinished@8")
#pragma comment(linker, "/alternatename:?BeginLoading@CDataBankTextEntry@NGameText@@ABEXKAAVCDataBank@2@@Z=@StubLoad@16")
#pragma comment(linker, "/alternatename:?BuildText@CDataBankTextEntry@NGameText@@ABEXXZ=@StubBuild@8")
#pragma comment(linker, "/alternatename:?MakeFreeSpace@CResourceList@@QAE_NK@Z=@StubFree@12")
#pragma comment(linker, "/alternatename:?ListRemove@CResource@@AAEXXZ=@StubRemove@8")
#pragma comment(linker, "/alternatename:?ListInsert@CResource@@AAEXPAV1@0@Z=@StubInsert@16")
#pragma comment(linker, "/alternatename:??0CWideString@@QAE@ABV0@@Z=@StubCopy@12")
#pragma comment(linker, "/alternatename:??1CWideString@@QAE@XZ=@StubDtor@8")
static void* g_vtable[2] = { (void*)StubDeletingDtor, (void*)StubDelete };

static int g_text_data[N];

static void Want(Ev* w, int& n, int k, const void* s, unsigned long a, const void* b) { w[n].kind = k; w[n].self = s; w[n].a = a; w[n].b = b; ++n; }

// spec of one scenario: per index, its type; texts: loaded or first-load (record PText null, BuildText supplies it); refcounts
struct Spec { const char* name; unsigned long types[N]; int first_load[N]; unsigned long text_refs[N]; unsigned long group_refs[N];
              int group_ids[N][4]; int group_counts[N]; };

static bool Run(const Spec& sp)
{
    memset(g_bank, 0x5A, sizeof g_bank); g_n = 0;
    for (int i = 0; i < N; ++i) {
        g_types[i] = sp.types[i]; g_entries[i] = 0; g_build_text[i] = 0;
        memset(g_text[i], 0x3C, 0x44); *(void**)g_text[i] = g_vtable; *(unsigned long*)(g_text[i] + 4) = sp.text_refs[i];
        *(void**)(g_text[i] + 8) = 0; *(void**)(g_text[i] + 0x28) = &g_text_data[i]; *(void**)(g_text[i] + 0x38) = 0;
        memset(g_group[i], 0x2D, 0x38); *(void**)g_group[i] = g_vtable; *(unsigned long*)(g_group[i] + 4) = sp.group_refs[i];
        for (int k = 0; k < sp.group_counts[i]; ++k) g_ids[i][k] = sp.group_ids[i][k];
        *(long**)(g_group[i] + 0x1C) = g_ids[i]; *(long**)(g_group[i] + 0x20) = g_ids[i] + sp.group_counts[i]; *(long**)(g_group[i] + 0x24) = g_ids[i] + 4;
        if (sp.types[i] == 0) { g_record[i][0] = sp.first_load[i] ? 0 : g_text[i]; g_record[i][1] = (void*)5; g_entries[i] = g_record[i]; g_build_text[i] = g_text[i]; }
        if (sp.types[i] == 1) g_entries[i] = (void*)0x5E5E5E5E;
    }
    *(void**)(g_bank + 0x1E0) = g_entries; *(void**)(g_bank + 0x1E4) = g_entries + N; *(void**)(g_bank + 0x1E8) = g_entries + N;
    unsigned char b0[0x220]; memcpy(b0, g_bank, sizeof b0);
    unsigned char t0[N][0x44], gr0[N][0x38]; memcpy(t0, g_text, sizeof t0); memcpy(gr0, g_group, sizeof gr0);
    RowValidate(g_bank, 0);

    Ev w[128]; int n = 0; bool ok = true;
    int empty[N]; for (int i = 0; i < N; ++i) empty[i] = sp.types[i] == 0 && sp.first_load[i];
    for (unsigned long i = 1; i < N; ++i) {
        Want(w, n, TYPE, g_bank, i, 0);
        if (sp.types[i] != 1) continue;
        Want(w, n, GROUP, g_bank, i, (const void*)1);
        if (sp.group_refs[i] == 0) Want(w, n, DEL, g_group[i], 0, 0);
        for (int k = 0; k < sp.group_counts[i]; ++k) {
            unsigned long id = (unsigned long)sp.group_ids[i][k];
            Want(w, n, TYPE, g_bank, id, 0);                                 // GetTextInfo (result unused)
            Want(w, n, TYPE, g_bank, id, 0);                                 // GetPTextEntry -> GetTextInfo
            Want(w, n, ASYNC, g_bank, 0, 0);
            if (empty[id]) {
                empty[id] = 0;
                Want(w, n, TYPE, g_bank, id, 0); Want(w, n, LOAD, g_record[id], id, g_bank); Want(w, n, BUILD, g_record[id], 0, 0);
                Want(w, n, FREE, g_bank + 0x168, 0, 0);
            }
            Want(w, n, COPY, 0, 0, g_text[id] + 0x28);
            Want(w, n, DTOR, 0, 0, 0);
            if (sp.text_refs[id] == 0) Want(w, n, DEL, g_text[id], 0, 0);
        }
    }
    ok = g_n == n;
    for (int i = 0; ok && i < n; ++i) {
        ok = g_log[i].kind == w[i].kind && g_log[i].a == w[i].a && g_log[i].b == w[i].b;
        if (w[i].kind == COPY) ok = ok && g_log[i + 1].self == g_log[i].self;          // the string destroyed is the one constructed
        else if (w[i].kind != DTOR) ok = ok && g_log[i].self == w[i].self;
    }
    ok = ok && !memcmp(b0, g_bank, sizeof b0) && !memcmp(t0, g_text, sizeof t0) && !memcmp(gr0, g_group, sizeof gr0);
    for (int i = 0; i < N; ++i) if (sp.types[i] == 0) ok = ok && g_record[i][0] == g_text[i] && g_record[i][1] == (void*)5;
    printf("%-40s %3d calls %s\n", sp.name, g_n, ok ? "ok" : "FAIL");
    if (!ok) for (int i = 0; i < g_n && i < 128; ++i) printf("   %s self %p a %lx b %p\n", kNames[g_log[i].kind], g_log[i].self, g_log[i].a, g_log[i].b);
    return ok;
}

int main()
{
    static const Spec specs[] = {
        { "two groups, texts loaded",
          { 1, 1, 0, 1, 0, 2, 0, 1, 0 }, { 0 }, { 2, 2, 2, 2, 2, 2, 2, 2, 2 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1 },
          { { 0 }, { 2, 4 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 8, 6, 2 }, { 0 } }, { 0, 2, 0, 0, 0, 0, 0, 3, 0 } },
        { "index 0 skipped, empty group",
          { 1, 2, 1, 0, 0, 0, 0, 0, 0 }, { 0 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 3, 3, 3, 3, 3, 3, 3, 3, 3 },
          { { 3 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } }, { 1, 0, 0, 0, 0, 0, 0, 0, 0 } },
        { "group and text deleted at count zero",
          { 0, 1, 0, 0, 1, 0, 0, 0, 0 }, { 0 }, { 1, 1, 0, 5, 1, 1, 1, 1, 1 }, { 1, 0, 1, 1, 2, 1, 1, 1, 1 },
          { { 0 }, { 2, 3 }, { 0 }, { 0 }, { 3 }, { 0 }, { 0 }, { 0 }, { 0 } }, { 0, 2, 0, 0, 1, 0, 0, 0, 0 } },
        { "first load of a text (record empty)",
          { 2, 0, 1, 0, 0, 0, 0, 0, 0 }, { 0, 1, 0, 0, 0, 0, 1, 0, 0 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1 },
          { { 0 }, { 0 }, { 1, 6, 1 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } }, { 0, 0, 3, 0, 0, 0, 0, 0, 0 } },
        { "no groups",
          { 0, 0, 2, 0, 2, 0, 0, 0, 0 }, { 0 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1 },
          { { 0 } }, { 0 } },
    };
    bool ok = true;
    for (unsigned i = 0; i < sizeof specs / sizeof specs[0]; ++i) ok &= Run(specs[i]);
    printf(ok ? "009c9170_TEST PASS\n" : "009c9170_TEST FAIL\n");
    return ok ? 0 : 1;
}