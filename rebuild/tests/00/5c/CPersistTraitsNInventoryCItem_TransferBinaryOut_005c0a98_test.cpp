// Behaviour test for 0x005C0A98 CPersistTraits<NInventory::CItem>::TransferBinaryOut (links the REAL row object). Fixtures: raw retail
// layouts (CItem: ItemType +0, NumberOfItems +4, PPerItemData.Data +8 / Info +0xC, AverageBoughtPrice +0x10; CPerItemData: augmentation
// vector +0/+4/+8, NumberOfKills +0xC, TimeWielded +0x10, BestTrophyWitnessesAheadToDate +0x14, QuestPersistent +0x18,
// QuestPersistentRefCount +0x1C; CCharString: data pointer, StringLength at data +4). Every callee the object leaves undefined is a
// recording stub bound by /alternatename. Each run checks the exact write sequence (bools, longs, strings), the def-name lookups and
// that every CCharString built is destroyed once.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ALT(a, b) comment(linker, "/alternatename:" a "=" b)
#pragma ALT("??0CCharString@@QAE@ABV0@@Z", "@StrCopy@12")
#pragma ALT("??0CCharString@@QAE@PBDJ@Z", "@StrCtor@16")
#pragma ALT("??1CCharString@@QAE@XZ", "@StrDtor@8")
#pragma ALT("??4CCharString@@QAEABV0@ABV0@@Z", "@StrAssign@12")
#pragma ALT("??BCDefString@@QBE?AVCCharString@@XZ", "@DefConv@12")
#pragma ALT("?GetDefNameFromGlobalIndex@CDefinitionManager@@QBE?AVCDefString@@J@Z", "@GetDefName@16")
#pragma ALT("?Get@CGameDefinitionManager@@SIAAV1@XZ", "@GameDefsGet@0")
#pragma ALT("?WriteString@CDataOutputStream@@QAEXABVCCharString@@@Z", "@WriteString@12")
#pragma ALT("?WriteSLONG@CDataOutputStream@@QAEXJ@Z", "@WriteSLONG@12")
#pragma ALT("?WriteEBOOL@CDataOutputStream@@QAEX_N@Z", "@WriteEBOOL@12")
#pragma ALT("@CallRow@16", "?TransferBinaryOut@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCDataOutputStream@@ABVCItem@NInventory@@@Z")

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct SD { char* p; unsigned long len; unsigned long dl; long refs; };
struct Str { SD* d; };
struct DefStr { long pos; };
struct Stream { long tag; };
struct PerItem { long* start; long* finish; long* eos; long kills, wielded, trophy; bool quest; long refs; float fish; };
typedef char PerItemCheck[(size_t)&((PerItem*)0)->refs == 0x1C ? 1 : -1];
struct Item { long type, count; PerItem* data; void* info; long price; };
typedef char ItemCheck[sizeof(Item) == 0x14 ? 1 : -1];

static char g_log[4096]; static int g_live = 0, g_built = 0; static Stream g_stream; static long g_mgr; static int g_badThis = 0;
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static SD* MakeSD(const char* t) { SD* d = (SD*)malloc(sizeof(SD)); d->len = (unsigned long)strlen(t); d->p = (char*)malloc(d->len + 1); strcpy(d->p, t); d->dl = d->len; d->refs = 1; return d; }

extern "C" {
Str* __fastcall StrCtor(Str* s, int, const char* t, long len) { CHECK(len == -1); s->d = MakeSD(t); ++g_live; ++g_built; return s; }
Str* __fastcall StrCopy(Str* s, int, const Str* o) { s->d = MakeSD(o->d->p); ++g_live; ++g_built; return s; }
void __fastcall StrDtor(Str* s, int) { CHECK(s->d != 0); free(s->d->p); free(s->d); s->d = 0; --g_live; }
const Str* __fastcall StrAssign(Str* s, int, const Str* o) { SD* n = MakeSD(o->d->p); free(s->d->p); free(s->d); s->d = n; return s; }
Str* __fastcall DefConv(const DefStr* ds, int, Str* ret) { char b[32]; sprintf(b, "DEF%ld", ds->pos); ret->d = MakeSD(b); ++g_live; ++g_built; return ret; }
DefStr* __fastcall GetDefName(const void* mgr, int, DefStr* ret, long index) { if (mgr != &g_mgr) g_badThis = 1; char b[32]; sprintf(b, "N%ld", index); Log(b); ret->pos = index; return ret; }
void* __fastcall GameDefsGet() { return &g_mgr; }
void __fastcall WriteString(Stream* os, int, const Str* s) { if (os != &g_stream) g_badThis = 1; char b[64]; sprintf(b, "S%s", s->d->p); Log(b); }
void __fastcall WriteSLONG(Stream* os, int, long v) { if (os != &g_stream) g_badThis = 1; char b[32]; sprintf(b, "L%ld", v); Log(b); }
void __fastcall WriteEBOOL(Stream* os, int, int v) { if (os != &g_stream) g_badThis = 1; char b[32]; sprintf(b, "B%d", v & 0xFF); Log(b); }
void __fastcall CallRow(void* traits, int, Stream* os, const Item* val);
}

static void Run(const Item& it, const char* expect)
{
    ++g_run; g_log[0] = 0; g_live = 0; g_built = 0; g_badThis = 0;
    long traits = 0x5A5A;
    CallRow(&traits, 0, &g_stream, &it);
    if (strcmp(g_log, expect)) { g_fail = 1; printf("run %d log\n  got  %s\n  want %s\n", g_run, g_log, expect); }
    CHECK(g_live == 0); CHECK(g_badThis == 0); CHECK(traits == 0x5A5A);
}

int main()
{
    Item a = { 7, 3, 0, 0, 50 };
    Run(a, "B0;N7;L4;SDEF7;L3;L50;");
    long aug[4] = { 5, 0, -1, 12 };
    PerItem pd = { aug, aug + 4, aug + 4, 9, 11, 13, true, 2, 0.0f };
    Item b = { 21, 1, &pd, (void*)1, 400 };
    Run(b, "B1;L4;N5;L4;SDEF5;L4;SNULL;L4;SNULL;N12;L5;SDEF12;L9;L11;L13;B1;L2;N21;L5;SDEF21;L1;L400;");
    CHECK(g_built == 8);
    PerItem pe = { aug, aug, aug, -3, 0, 1, false, -1, 0.0f };
    Item c = { 100, 0, &pe, (void*)1, -8 };
    Run(c, "B1;L0;L-3;L0;L1;B0;L-1;N100;L6;SDEF100;L0;L-8;");
    if (!g_fail) printf("005c0a98_TEST PASS\n");
    return g_fail;
}