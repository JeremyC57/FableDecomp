// Behaviour test for 0x005C2118 CPersistTraits<NInventory::CItem>::TransferIn (links the REAL row object: the row body plus the TU's
// vector<long>::resize(n, const long&) COMDAT, whose _M_fill_insert is stubbed). A token-stream parser model drives the text format
//   "( hasData , [ ( count , aug... , kills , wielded , trophy [ , questPersistent , refCount ) | ) ] , ] defName , number , price )"
// Every out-of-line callee is a recording stub with the native calling convention: CStringParser::ReadAsSymbol (hidden result),
// CBasicString<char>::Compare, CCharString ctor / dtor / operator!=, the bool / CCharString / SLONG traits readers, PeekNextItemType,
// ReadAsInteger, CGameDefinitionManager::Get / GetDefGlobalIndexFromName, NInventory::CItem(type, number, data, price) (snapshots the
// per-item data it is handed), CItem::operator=, ~CCountedPointer<CPerItemData> (the TU's own COMDAT: the stub constructor hands the temporary a count block at
// RefCount 2, so the release must leave it at 1). Each run checks the item handed over (or none), the
// CCharString construct / destroy balance and how many tokens were consumed.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }

#pragma comment(linker, "/alternatename:@CallRow@20=?TransferIn@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCItem@NInventory@@@Z")
#pragma comment(linker, "/alternatename:?ReadAsSymbol@CStringParser@@QAE?AVCCharString@@XZ=@StubReadAsSymbol@12")
#pragma comment(linker, "/alternatename:?Compare@?$CBasicString@D@@KIJPBD0@Z=@StubCompare@8")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@XZ=@StubStrCtor@8")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StubStrDtor@8")
#pragma comment(linker, "/alternatename:??9CCharString@@QBE_NPBD@Z=@StubStrNe@12")
#pragma comment(linker, "/alternatename:?TransferIn@?$CPersistTraits@_N@@QAEXAAVCPersistContext@@AAVCStringParser@@AA_N@Z=@StubBoolIn@20")
#pragma comment(linker, "/alternatename:?TransferIn@?$CPersistTraits@VCCharString@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCCharString@@@Z=@StubStringIn@20")
#pragma comment(linker, "/alternatename:?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z=@StubLongIn@12")
#pragma comment(linker, "/alternatename:?PeekNextItemType@CStringParser@@QAE?AW4EParsedItemType@@XZ=@StubPeek@8")
#pragma comment(linker, "/alternatename:?ReadAsInteger@CStringParser@@QAEJXZ=@StubReadInt@8")
#pragma comment(linker, "/alternatename:?Get@CGameDefinitionManager@@SIAAV1@XZ=@StubGet@0")
#pragma comment(linker, "/alternatename:?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z=@StubDefIndex@12")
#pragma comment(linker, "/alternatename:?_M_fill_insert@?$vector@JV?$allocator@J@std@@@std@@QAEXPAJIABJ@Z=@StubFillInsert@20")
#pragma comment(linker, "/alternatename:??0CItem@NInventory@@QAE@JJPBVCPerItemData@1@J@Z=@StubItemCtor@24")
#pragma comment(linker, "/alternatename:??4CItem@NInventory@@QAEAAV01@ABV01@@Z=@StubItemAssign@12")

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct BStr { const char* PData; unsigned long len; };
struct CStr { BStr* p; };
struct Vec { long* s; long* f; long* e; };
struct PerItem { Vec aug; long kills, wielded, trophy; bool quest; unsigned char pad[3]; long ref; float fish; };
struct Item { long type, number; void* data; void* info; long price; };
struct Block { unsigned long refs; void (__fastcall* del)(void*); void* data; };
static Block g_block;

static const char* const* g_tok; static int g_pos, g_ntok; static int g_str = 0; static void* g_parser; static void* g_ctx;
static const char* Next() { CHECK(g_pos < g_ntok); return g_pos < g_ntok ? g_tok[g_pos++] : ")"; }
static BStr* MakeStr(const char* t) { BStr* b = (BStr*)malloc(sizeof(BStr)); b->PData = t; b->len = (unsigned long)strlen(t); return b; }
extern "C" CStr* __fastcall StubReadAsSymbol(void* self, void*, CStr* ret)
{
    CHECK(self == g_parser); const char* t = Next(); ++g_str;
    ret->p = t[0] == '~' ? 0 : MakeStr(t);           // "~" models an empty symbol: no string data
    return ret;
}
extern "C" long __fastcall StubCompare(const char* a, const char* b) { int r = strcmp(a, b); return r < 0 ? -1 : r > 0 ? 1 : 0; }
extern "C" CStr* __fastcall StubStrCtor(CStr* self, void*) { self->p = 0; ++g_str; return self; }
extern "C" void __fastcall StubStrDtor(CStr* self, void*) { --g_str; if (self->p) free(self->p); self->p = 0; }
extern "C" bool __fastcall StubStrNe(const CStr* self, void*, const char* text) { const char* s = self->p ? self->p->PData : ""; return strcmp(s, text) != 0; }
static void* g_boolTraits; static void* g_stringTraits;
extern "C" void __fastcall StubBoolIn(void* self, void*, void* ctx, void* parser, bool* v)
{   // the real reader leaves the value unchanged (and reports) on a token that is neither TRUE nor FALSE
    CHECK(ctx == g_ctx && parser == g_parser && self != 0); g_boolTraits = self; const char* t = Next();
    if (!strcmp(t, "TRUE")) *v = true; else if (!strcmp(t, "FALSE")) *v = false;
}
extern "C" void __fastcall StubStringIn(void* self, void*, void* ctx, void* parser, CStr* v)
{ CHECK(ctx == g_ctx && parser == g_parser && self != 0); g_stringTraits = self; if (v->p) free(v->p); v->p = MakeStr(Next()); }
extern "C" void __fastcall StubLongIn(void* ctx, void* parser, long* v) { CHECK(ctx == g_ctx && parser == g_parser); *v = atol(Next()); }
extern "C" long __fastcall StubPeek(void* self, void*) { CHECK(self == g_parser); const char* t = g_tok[g_pos]; return (t[0] >= '0' && t[0] <= '9') ? 3 : 1; }
extern "C" long __fastcall StubReadInt(void* self, void*) { CHECK(self == g_parser); return atol(Next()); }
static int g_mgr;
extern "C" void* __fastcall StubGet() { return &g_mgr; }
static int g_defCalls = 0;
extern "C" long __fastcall StubDefIndex(void* self, void*, const CStr* name) { CHECK(self == &g_mgr && name->p); ++g_defCalls; return name->p ? (long)strlen(name->p->PData) * 100 + name->p->PData[0] : -1; }
extern "C" void __fastcall StubFillInsert(Vec* self, void*, long* pos, unsigned int n, const long* x)
{
    CHECK(pos == self->f && *x == 0);
    unsigned int old = (unsigned int)(self->f - self->s);
    long* p = (long*)malloc((old + n) * sizeof(long) + 4);
    for (unsigned int i = 0; i < old; ++i) p[i] = self->s[i];
    for (unsigned int i = 0; i < n; ++i) p[old + i] = *x;
    if (self->s) free(self->s);
    self->s = p; self->f = self->e = p + old + n;
}
static int g_ctorCalls, g_assignCalls; static Item g_made; static bool g_hadData; static PerItem g_snap; static long g_snapAug[8]; static int g_snapN;
extern "C" Item* __fastcall StubItemCtor(Item* self, void*, long type, long number, const PerItem* data, long price)
{
    ++g_ctorCalls; self->type = type; self->number = number; g_block.refs = 2; g_block.del = 0; g_block.data = &g_block; self->data = &g_block; self->info = &g_block; self->price = price;
    g_hadData = data != 0;
    if (data) { g_snap = *data; g_snapN = (int)(data->aug.f - data->aug.s); for (int i = 0; i < g_snapN && i < 8; ++i) g_snapAug[i] = data->aug.s[i]; }
    g_made = *self; return self;
}
static Item g_val;

extern "C" Item* __fastcall StubItemAssign(Item* self, void*, const Item* other) { ++g_assignCalls; CHECK(self == &g_val); CHECK(other->type == g_made.type); *self = *other; return self; }
extern "C" void __fastcall CallRow(void* traits, void* edx, void* ctx, void* parser, Item* val);

static int g_traitsObj, g_ctxObj, g_parserObj;
static void Run(const char* const* toks, int n, bool expectItem)
{
    ++g_run; g_tok = toks; g_ntok = n; g_pos = 0; g_str = 0; g_ctorCalls = g_assignCalls = 0; g_block.refs = 0; g_defCalls = 0;
    g_parser = &g_parserObj; g_ctx = &g_ctxObj;
    g_val.type = -1; g_val.number = -2; g_val.data = 0; g_val.info = 0; g_val.price = -3;
    CallRow(&g_traitsObj, 0, g_ctx, g_parser, &g_val);
    CHECK(g_str == 0);                                   // every CCharString built (symbols, names, the traits default) destroyed
    if (expectItem) {
        CHECK(g_ctorCalls == 1 && g_assignCalls == 1 && g_block.refs == 1 && g_pos == n);   // the temporary released once
        CHECK(g_val.type == g_made.type && g_val.number == g_made.number && g_val.price == g_made.price);
    } else {
        CHECK(g_ctorCalls == 0 && g_assignCalls == 0 && g_block.refs == 0);
        CHECK(g_val.type == -1 && g_val.number == -2 && g_val.price == -3);
    }
}
#define RUN(a, item) Run(a, (int)(sizeof(a) / sizeof(a[0])), item)
static long Def(const char* s) { return (long)strlen(s) * 100 + s[0]; }

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    // 1: full item with four augmentation slots (an integer, a name, "NULL", ""), the quest pair, then def / number / price
    static const char* const a[] = { "(", "TRUE", ",", "(", "4", ",", "7", ",", "ABC", ",", "NULL", ",", "", ",", "5", ",", "6", ",", "1", ",",
                                     "FALSE", ",", "3", ")", ",", "SWORD", ",", "1", ",", "40", ")" };
    RUN(a, true);
    CHECK(g_made.type == Def("SWORD") && g_made.number == 1 && g_made.price == 40 && g_hadData);
    CHECK(g_snapN == 4 && g_snapAug[1] == Def("ABC") && g_snapAug[0] == 0 && g_snapAug[2] == 0 && g_snapAug[3] == 0);
    CHECK(g_snap.kills == 5 && g_snap.wielded == 6 && g_snap.trophy == 1 && !g_snap.quest && g_snap.ref == 3);
    CHECK(g_defCalls == 2 && g_boolTraits != 0 && g_stringTraits != 0 && g_boolTraits != g_stringTraits);
    // 2: no per-item data
    static const char* const b[] = { "(", "FALSE", ",", "AXE", ",", "2", ",", "9", ")" };
    RUN(b, true);
    CHECK(g_made.type == Def("AXE") && g_made.number == 2 && g_made.price == 9 && !g_hadData);
    // 3: per-item data without the quest pair (the trophy count closed by ")") and no augmentations: defaults kept
    static const char* const c[] = { "(", "TRUE", ",", "(", "0", ",", "8", ",", "9", ",", "2", ")", ",", "MACE", ",", "3", ",", "11", ")" };
    RUN(c, true);
    CHECK(g_hadData && g_snapN == 0 && g_snap.kills == 8 && g_snap.wielded == 9 && g_snap.trophy == 2 && g_snap.quest && g_snap.ref == -1);
    CHECK(g_made.type == Def("MACE") && g_made.number == 3 && g_made.price == 11);
    // 3b: an unreadable has-data flag leaves it false: the no-data layout follows
    static const char* const d[] = { "(", "MAYBE", ",", "AXE", ",", "4", ",", "12", ")" };
    RUN(d, true);
    CHECK(!g_hadData && g_made.type == Def("AXE") && g_made.number == 4 && g_made.price == 12);
    // 4..: every separator check returns without touching the item
    static const char* const f1[] = { "[" };
    static const char* const f1b[] = { "~" };                                                  // empty symbol (no string data)
    static const char* const f2[] = { "(", "TRUE", ";" };
    static const char* const f3[] = { "(", "TRUE", ",", "[" };
    static const char* const f4[] = { "(", "TRUE", ",", "(", "1", ";" };
    static const char* const f5[] = { "(", "TRUE", ",", "(", "1", ",", "ABC", ";" };
    static const char* const f6[] = { "(", "TRUE", ",", "(", "0", ",", "5", ";" };
    static const char* const f7[] = { "(", "TRUE", ",", "(", "0", ",", "5", ",", "6", ";" };
    static const char* const f8[] = { "(", "TRUE", ",", "(", "0", ",", "5", ",", "6", ",", "1", ",", "TRUE", ";" };
    static const char* const f9[] = { "(", "TRUE", ",", "(", "0", ",", "5", ",", "6", ",", "1", ",", "TRUE", ",", "4", "]" };
    static const char* const f10[] = { "(", "TRUE", ",", "(", "0", ",", "5", ",", "6", ",", "1", ")", ";" };
    static const char* const f11[] = { "(", "FALSE", ",", "AXE", ";" };
    static const char* const f12[] = { "(", "FALSE", ",", "AXE", ",", "2", ";" };
    static const char* const f13[] = { "(", "FALSE", ",", "AXE", ",", "2", ",", "9", "]" };
    RUN(f1, false); CHECK(g_pos == 1);
    RUN(f1b, false); CHECK(g_pos == 1);
    RUN(f2, false); CHECK(g_pos == 3);
    RUN(f3, false); CHECK(g_pos == 4);
    RUN(f4, false); CHECK(g_pos == 6);
    RUN(f5, false); CHECK(g_pos == 8 && g_defCalls == 1);
    RUN(f6, false); CHECK(g_pos == 8);
    RUN(f7, false); CHECK(g_pos == 10);
    RUN(f8, false); CHECK(g_pos == 14);
    RUN(f9, false); CHECK(g_pos == 16);
    RUN(f10, false); CHECK(g_pos == 13);
    RUN(f11, false); CHECK(g_pos == 5 && g_defCalls == 1);
    RUN(f12, false); CHECK(g_pos == 7);
    RUN(f13, false); CHECK(g_pos == 9);
    if (g_fail) { printf("005c2118_TEST FAIL\n"); return 1; }
    printf("005c2118_TEST PASS\n");
    return 0;
}