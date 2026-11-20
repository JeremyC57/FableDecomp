// Behaviour test for 0x00A781A0 CNavQuadTree::UpdateLines (links the REAL row object, which also carries the
// std::vector<C2DBoxF>(size_type) COMDAT it calls). The classes come from the shared headers only so that the one out-of-line
// callee defined here, C2DLineF::IntersectsWith(const C2DBoxF&), gets its retail name; every object is raw native-layout storage:
//   CNavQuadTree            MapPos +7 (two unaligned floats), MapWidth +0x10
//   vector<list<C2DLineF>>  {start, finish, end_of_storage}; a list is one pointer to its sentinel node; a node is
//                           {next +0, prev +4, C2DLineF +8}, 0x18 bytes from malloc
//   vector<C2DLineF>        {start, finish, end_of_storage}, 16-byte lines; C2DBoxI {TLX, TLY, BRX, BRY}
// UpdateLines is a private member, so the test reaches it through a linker alias (thiscall = fastcall with a spare edx).
// After every call the test walks every list of the array (forward links, back links, node payloads), compares them and the
// IntersectsWith call list (line, box bits, order) with the independent model in RunModel(), and checks that the tree, the line
// array, the area and the two vector headers are unchanged.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#include "fable_compiler.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DLineF.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);

extern "C" void __fastcall CallUpdateLines(void* self, int spare, void* line_lists, const void* lines, const void* update_area);
#pragma comment(linker, "/alternatename:@CallUpdateLines@20=?UpdateLines@CNavQuadTree@@ABEXAAV?$vector@V?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@V?$allocator@V?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@2@@std@@ABV?$vector@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@3@ABVC2DBoxI@@@Z")

static long __stdcall Crash(void*) { ExitProcess(3); return 0; }

// ---- deterministic random numbers
static unsigned long g_seed = 0x1234abcd;
static unsigned long Rnd() { g_seed = g_seed * 1664525ul + 1013904223ul; return g_seed >> 8; }
static long RndRange(long lo, long hi) { return lo + (long)(Rnd() % (unsigned long)(hi - lo + 1)); }

// ---- raw objects
struct RawLine { float v[4]; };                      // Point1.X, Point1.Y, Point2.X, Point2.Y
struct RawNode { RawNode* next; RawNode* prev; RawLine line; };
struct RawVec { char* start; char* finish; char* end_of_storage; };
struct RawArea { long tlx, tly, brx, bry; };

enum { LISTS = 400, LIST_BASE = 80, MAX_LINES = 8, MAX_PER_LIST = 80, MAX_CALLS = 4096 };
static RawNode* g_sentinel[LISTS];                   // what each list slot points at
static RawNode* g_slots[LISTS];                      // the vector<list> storage itself (one pointer per list)
static RawLine g_lines[MAX_LINES];
static int g_lineCount;

// ---- model state
struct ModelList { int n; RawLine e[MAX_PER_LIST]; };
static ModelList g_model[LISTS];
struct Call { long line; unsigned long box[4]; };
static Call g_calls[MAX_CALLS]; static int g_callCount;
static Call g_want[MAX_CALLS]; static int g_wantCount;
static int g_wild;                                   // 1: the node boxes are not initialised by the row (see the wild scenarios)
static unsigned long g_caseSalt;
static int g_fail;

static bool Decide(long line, const unsigned long* box)
{
    unsigned long h = g_caseSalt ^ ((unsigned long)line * 2654435761ul) ^ (box[0] * 40503ul) ^ (box[1] * 69069ul) ^ (box[2] >> 3) ^ (box[3] << 5);
    h ^= h >> 15; h *= 2246822519ul; h ^= h >> 13;
    return (h & 7) < 3;
}

// The one out-of-line callee. In the wild scenarios the box is uninitialised storage: answer true and log nothing.
bool C2DLineF::IntersectsWith(const C2DBoxF& box) const
{
    long line = (long)((const RawLine*)this - g_lines);
    if (line < 0 || line >= g_lineCount || (const char*)this != (const char*)&g_lines[line]) { printf("FAIL IntersectsWith this\n"); g_fail = 1; return false; }
    if (g_wild) return true;
    unsigned long bits[4]; memcpy(bits, &box, 16);
    if (g_callCount < MAX_CALLS) { g_calls[g_callCount].line = line; memcpy(g_calls[g_callCount].box, bits, 16); }
    g_callCount++;
    return Decide(line, bits);
}

// ---- model arithmetic (the row's x87 runs at 53-bit precision: double arithmetic, results rounded on the float stores)
static long RoundNearEven(double d)
{
    double f = floor(d); double r = d - f; long k = (long)f;
    if (r > 0.5) return k + 1;
    if (r < 0.5) return k;
    return (k & 1) ? k + 1 : k;
}
static long CeilModel(float value)
{
    long result = RoundNearEven((double)value + 0.5);
    float back = (float)((double)result - 1.0);
    if (memcmp(&value, &back, 4) == 0) result--;
    return result;
}
static long FloorModel(float value)
{
    long result = RoundNearEven((double)value - 0.5);
    float next = (float)((double)result + 1.0);
    if (memcmp(&value, &next, 4) == 0) result++;
    return result;
}
static void ModelPush(long slot, const RawLine& line)
{
    long i = slot + LIST_BASE;
    if (i < 0 || i >= LISTS || g_model[i].n >= MAX_PER_LIST) { printf("FAIL test geometry (slot %ld)\n", slot); g_fail = 1; return; }
    g_model[i].e[g_model[i].n++] = line;
}

static long g_statClear, g_statPush, g_statContains1, g_statContains2, g_statIntersect, g_statReject, g_statReturnHigh, g_statReturnNeg,
            g_statSkipBig, g_statAtSize, g_statClampHx, g_statClampHy, g_statClampLx, g_statClampLy;

static void RunModel(float map_x, float map_y, float map_width, const RawArea& a)
{
    long full_boxes_wide = (long)((double)map_width * 0.125);
    long width = (long)((unsigned long)a.brx - (unsigned long)a.tlx);
    long height = (long)((unsigned long)a.bry - (unsigned long)a.tly);
    unsigned long size = (unsigned long)width * (unsigned long)height;
    static float boxes[64][4];
    g_wantCount = 0;
    if (!g_wild)
    {
        if (size > 64) { printf("FAIL test geometry (size)\n"); g_fail = 1; return; }
        for (unsigned long x = (unsigned long)a.tlx; x < (unsigned long)a.brx; x++)
            for (unsigned long y = (unsigned long)a.tly; y < (unsigned long)a.bry; y++)
            {
                double left = (double)x * 8.0 + (double)map_x;
                double top = (double)y * 8.0 + (double)map_y;
                float* b = boxes[(y - a.tly) * width + (x - a.tlx)];
                b[0] = (float)left; b[1] = (float)top; b[2] = (float)(left + 8.0); b[3] = (float)(top + 8.0);
                long slot = (long)y * full_boxes_wide + (long)x + LIST_BASE;
                if (slot < 0 || slot >= LISTS) { printf("FAIL test geometry (clear slot)\n"); g_fail = 1; return; }
                if (g_model[slot].n) g_statClear++;
                g_model[slot].n = 0;
            }
    }
    for (int i = 0; i < g_lineCount; i++)
    {
        const RawLine& l = g_lines[i];
        float hx = l.v[0] < l.v[2] ? l.v[2] : l.v[0], hy = l.v[1] < l.v[3] ? l.v[3] : l.v[1];
        float lx = l.v[2] < l.v[0] ? l.v[2] : l.v[0], ly = l.v[3] < l.v[1] ? l.v[3] : l.v[1];
        long high_x = CeilModel((float)(((double)hx - (double)map_x) * 0.125));
        if (high_x > a.brx) { high_x = a.brx; g_statClampHx++; }
        long high_y = CeilModel((float)(((double)hy - (double)map_y) * 0.125));
        if (high_y > a.bry) { high_y = a.bry; g_statClampHy++; }
        long low_x = FloorModel((float)(((double)lx - (double)map_x) * 0.125));
        if (low_x < a.tlx) { low_x = a.tlx; g_statClampLx++; }
        long low_y = FloorModel((float)(((double)ly - (double)map_y) * 0.125));
        if (low_y < a.tly) { low_y = a.tly; g_statClampLy++; }
        for (long x = low_x; x < high_x; x++)
            for (long y = low_y; y < high_y; y++)
            {
                __int64 wide = ((__int64)y - (__int64)a.tly) * (__int64)width + ((__int64)x - (__int64)a.tlx);
                if (wide < 0) { g_statReturnNeg++; return; }
                if (wide > 0x7fffffff) { g_statReturnHigh++; return; }
                unsigned long index = (unsigned long)wide;
                if (index > size) { g_statSkipBig++; continue; }
                bool keep;
                if (g_wild)
                {
                    keep = true;
                    if (index == size) g_statAtSize++;
                }
                else
                {
                    if (index >= size) { printf("FAIL test geometry (index)\n"); g_fail = 1; return; }
                    const float* b = boxes[index];
                    if (l.v[0] >= b[0] && l.v[0] < b[2] && l.v[1] >= b[1] && l.v[1] < b[3]) { keep = true; g_statContains1++; }
                    else if (l.v[2] >= b[0] && l.v[2] < b[2] && l.v[3] >= b[1] && l.v[3] < b[3]) { keep = true; g_statContains2++; }
                    else
                    {
                        unsigned long bits[4]; memcpy(bits, b, 16);
                        if (g_wantCount < MAX_CALLS) { g_want[g_wantCount].line = i; memcpy(g_want[g_wantCount].box, bits, 16); }
                        g_wantCount++;
                        keep = Decide(i, bits);
                        if (keep) g_statIntersect++; else g_statReject++;
                    }
                }
                if (keep) { ModelPush(y * full_boxes_wide + x, l); g_statPush++; }
            }
    }
}

static float RandomCoord(float map, long lo_cell, long hi_cell)
{
    long cell = RndRange(lo_cell, hi_cell);
    double off;
    switch (Rnd() % 6)
    {
        case 0: off = 0.0; break;                                  // exactly on a cell edge
        case 1: off = 4.0; break;
        case 2: off = 7.999; break;
        case 3: off = 0.001; break;
        default: off = (double)(Rnd() % 8192) / 1024.0; break;
    }
    return (float)((double)map + 8.0 * (double)cell + off);
}

static int RunCase(int number, int scenario)
{
    unsigned char tree[0x40], tree_copy[0x40];
    int i;
    for (i = 0; i < 0x40; i++) tree[i] = (unsigned char)Rnd();
    float map_x = (float)RndRange(-640, 640) / 16.0f, map_y = (float)RndRange(-640, 640) / 16.0f;
    if (Rnd() % 4 == 0) { map_x += (float)(Rnd() % 1000) / 7919.0f; map_y -= (float)(Rnd() % 1000) / 7907.0f; }
    long boxes_wide = RndRange(0, 9);
    float map_width = (float)(boxes_wide * 8) + (float)(Rnd() % 8192) / 1024.0f;       // 8 * n + [0, 8): the count truncates
    RawArea area, area_copy;
    long lo_x, hi_x, lo_y, hi_y;                                       // cell range the line end points are drawn from
    g_wild = 0;
    if (scenario == 0)
    {
        // ordinary area: every node box is initialised by the first loop
        area.tlx = RndRange(0, 5); area.tly = RndRange(0, 5);
        area.brx = area.tlx + RndRange(0, 5); area.bry = area.tly + RndRange(0, 5);
        lo_x = area.tlx - 2; hi_x = area.brx + 2; lo_y = area.tly - 2; hi_y = area.bry + 2;
    }
    else if (scenario == 1)
    {
        // index == size: width 2^k and height 2^(32-k) + a give a 32-bit box count of a * width, while the first loop is skipped
        // (TLX < 0 is a huge unsigned start). Rows below a index real storage, row a column 0 is exactly size(), later cells are
        // past it and skipped.
        long k = RndRange(2, 4), w = 1L << k, a = RndRange(1, 2);
        g_wild = 1; map_x = (float)RndRange(-4, 4); map_y = (float)RndRange(-4, 4);
        area.tlx = -RndRange(1, w); area.brx = area.tlx + w;
        area.tly = RndRange(0, 3); area.bry = area.tly + (long)(1UL << (32 - k)) + a;
        lo_x = area.tlx - 1; hi_x = area.brx; lo_y = area.tly - 1; hi_y = area.tly + a + 1;
    }
    else if (scenario == 2)
    {
        // 64-bit sum above 0x7fffffff: width 2^16, height 2^16 + 1 (a 32-bit box count of 65536). Rows from 32768 on overflow the
        // index and end the whole update; row 0 is real storage.
        g_wild = 1; map_x = (float)RndRange(-4, 4); map_y = (float)RndRange(-4, 4);
        area.tlx = -32768; area.brx = 32768; area.tly = RndRange(0, 2); area.bry = area.tly + 65537;
        lo_x = -3; hi_x = 3; lo_y = area.tly - 1; hi_y = area.tly;
    }
    else if (scenario == 4)
    {
        // a row range that starts below zero: the first loop's unsigned row counter starts above BRY, so no box is initialised and no
        // list is cleared, while the signed second loop still walks the cells (all of them inside the storage).
        g_wild = 1; map_x = (float)RndRange(-4, 4); map_y = (float)RndRange(-4, 4);
        area.tlx = RndRange(0, 3); area.brx = area.tlx + RndRange(1, 4); area.tly = -RndRange(1, 3); area.bry = RndRange(0, 2);
        lo_x = area.tlx - 1; hi_x = area.brx + 1; lo_y = area.tly - 1; hi_y = area.bry + 1;
    }
    else
    {
        // negative 64-bit sum: TLX = -2^31 and BRX = 8 make the 32-bit width -2^31 + 8; with two rows the box count is 16. Row 1
        // from column -8 on indexes the storage, row 1 left of it is negative (ends the update), row 0 right of 0 is too high.
        g_wild = 1; map_x = (float)RndRange(-4, 4); map_y = (float)RndRange(-4, 4);
        area.tlx = (long)0x80000000; area.brx = 8; area.tly = RndRange(0, 2); area.bry = area.tly + 2;
        lo_x = -12; hi_x = 9; lo_y = area.tly - 1; hi_y = area.tly + 2;
    }
    if (g_wild && scenario != 1) boxes_wide = RndRange(0, 9);
    if (g_wild) map_width = (float)(boxes_wide * 8) + (float)(Rnd() % 8192) / 1024.0f;
    memcpy(tree + 7, &map_x, 4); memcpy(tree + 0xb, &map_y, 4); memcpy(tree + 0x10, &map_width, 4);
    memcpy(tree_copy, tree, 0x40); area_copy = area;

    g_lineCount = (int)RndRange(0, MAX_LINES - 2);
    for (i = 0; i < MAX_LINES; i++)
    {
        g_lines[i].v[0] = RandomCoord(map_x, lo_x, hi_x); g_lines[i].v[1] = RandomCoord(map_y, lo_y, hi_y);
        if (Rnd() % 3 == 0)
        {
            // a short line inside one cell or reaching into the next
            g_lines[i].v[2] = g_lines[i].v[0] + (float)RndRange(-6, 6); g_lines[i].v[3] = g_lines[i].v[1] + (float)RndRange(-6, 6);
        }
        else { g_lines[i].v[2] = RandomCoord(map_x, lo_x, hi_x); g_lines[i].v[3] = RandomCoord(map_y, lo_y, hi_y); }
    }
    if (scenario == 2)
    {
        // some lines far down, in the rows whose index overflows
        for (i = 0; i < g_lineCount; i++)
            if (Rnd() % 3 == 0)
            {
                long row = area.tly + RndRange(32766, 32770) + (Rnd() % 4 == 0 ? 32768 : 0);
                g_lines[i].v[1] = (float)((double)map_y + 8.0 * row + 2.0); g_lines[i].v[3] = g_lines[i].v[1] + (float)RndRange(0, 12);
            }
    }
    RawLine lines_copy[MAX_LINES]; memcpy(lines_copy, g_lines, sizeof g_lines);

    // lists: every slot starts with 0..2 lines of its own
    for (i = 0; i < LISTS; i++)
    {
        RawNode* s = (RawNode*)malloc(sizeof(RawNode)); memset(s, 0xcd, sizeof(RawNode));
        s->next = s; s->prev = s; g_sentinel[i] = s; g_slots[i] = s; g_model[i].n = 0;
        int n = (Rnd() % 3 == 0) ? (int)RndRange(1, 2) : 0;
        for (int j = 0; j < n; j++)
        {
            RawNode* node = (RawNode*)malloc(sizeof(RawNode));
            for (int c = 0; c < 4; c++) node->line.v[c] = (float)RndRange(-1000, 1000);
            node->next = s; node->prev = s->prev; s->prev->next = node; s->prev = node;
            g_model[i].e[g_model[i].n++] = node->line;
        }
    }
    RawVec lists_vec, lists_vec_copy, lines_vec, lines_vec_copy;
    lists_vec.start = (char*)&g_slots[LIST_BASE]; lists_vec.finish = (char*)&g_slots[LISTS]; lists_vec.end_of_storage = lists_vec.finish;
    lines_vec.start = (char*)g_lines; lines_vec.finish = (char*)&g_lines[g_lineCount]; lines_vec.end_of_storage = (char*)&g_lines[MAX_LINES];
    lists_vec_copy = lists_vec; lines_vec_copy = lines_vec;

    g_caseSalt = Rnd(); g_callCount = 0;
    RunModel(map_x, map_y, map_width, area);
    if (g_fail) return 1;
    CallUpdateLines(tree, 0, &lists_vec, &lines_vec, &area);
    if (g_fail) return 1;

    int bad = 0;
    if (memcmp(tree, tree_copy, 0x40)) { printf("FAIL case %d: the tree was written\n", number); bad = 1; }
    if (memcmp(&area, &area_copy, sizeof area)) { printf("FAIL case %d: the area was written\n", number); bad = 1; }
    if (memcmp(g_lines, lines_copy, sizeof g_lines)) { printf("FAIL case %d: the lines were written\n", number); bad = 1; }
    if (memcmp(&lists_vec, &lists_vec_copy, sizeof lists_vec) || memcmp(&lines_vec, &lines_vec_copy, sizeof lines_vec)) { printf("FAIL case %d: a vector header was written\n", number); bad = 1; }
    if (!g_wild)
    {
        if (g_callCount != g_wantCount) { printf("FAIL case %d: %d IntersectsWith calls, model %d\n", number, g_callCount, g_wantCount); bad = 1; }
        else if (g_callCount <= MAX_CALLS && memcmp(g_calls, g_want, g_callCount * sizeof(Call))) { printf("FAIL case %d: IntersectsWith call list differs\n", number); bad = 1; }
    }
    for (i = 0; i < LISTS && !bad; i++)
    {
        RawNode* s = g_sentinel[i];
        if (g_slots[i] != s) { printf("FAIL case %d: list %d sentinel pointer changed\n", number, i - LIST_BASE); bad = 1; break; }
        RawNode* prev = s; RawNode* node = s->next; int n = 0;
        while (node != s)
        {
            if (n >= g_model[i].n) { printf("FAIL case %d: list %d has more than the model's %d lines\n", number, i - LIST_BASE, g_model[i].n); bad = 1; break; }
            if (node->prev != prev) { printf("FAIL case %d: list %d back link\n", number, i - LIST_BASE); bad = 1; break; }
            if (memcmp(&node->line, &g_model[i].e[n], 16)) { printf("FAIL case %d: list %d line %d differs\n", number, i - LIST_BASE, n); bad = 1; break; }
            prev = node; node = node->next; n++;
        }
        if (!bad && n != g_model[i].n) { printf("FAIL case %d: list %d has %d lines, model %d\n", number, i - LIST_BASE, n, g_model[i].n); bad = 1; }
        if (!bad && s->prev != prev) { printf("FAIL case %d: list %d sentinel back link\n", number, i - LIST_BASE); bad = 1; }
    }
    if (bad)
        printf("  scenario %d area %ld %ld %ld %ld map %g %g width %g lines %d\n", scenario, area.tlx, area.tly, area.brx, area.bry, map_x, map_y, map_width, g_lineCount);
    for (i = 0; i < LISTS && !bad; i++)
    {
        RawNode* s = g_sentinel[i]; RawNode* node = s->next;
        while (node != s) { RawNode* next = node->next; free(node); node = next; }
        free(s);
    }
    return bad;
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    int cases = 0;
    for (int n = 0; n < 24000; n++)
    {
        int scenario = 0;
        if (n % 16 == 5) scenario = 1;
        else if (n % 160 == 7) scenario = 2;
        else if (n % 16 == 9) scenario = 3;
        else if (n % 16 == 11) scenario = 4;
        if (RunCase(n, scenario)) { printf("FAIL\n"); return 1; }
        cases++;
    }
    long* stats[] = { &g_statClear, &g_statPush, &g_statContains1, &g_statContains2, &g_statIntersect, &g_statReject, &g_statReturnHigh,
                      &g_statReturnNeg, &g_statSkipBig, &g_statAtSize, &g_statClampHx, &g_statClampHy, &g_statClampLx, &g_statClampLy };
    for (int s = 0; s < (int)(sizeof stats / sizeof stats[0]); s++)
        if (*stats[s] == 0) { printf("FAIL path %d never taken\n", s); return 1; }
    printf("cases %d: cleared %ld, pushed %ld (first point %ld, second point %ld, intersects %ld), rejected %ld, ended high %ld, ended negative %ld, past size %ld, at size %ld, clamps %ld %ld %ld %ld\n",
           cases, g_statClear, g_statPush, g_statContains1, g_statContains2, g_statIntersect, g_statReject, g_statReturnHigh, g_statReturnNeg,
           g_statSkipBig, g_statAtSize, g_statClampHx, g_statClampHy, g_statClampLx, g_statClampLy);
    printf("00a781a0_TEST PASS\n");
    return 0;
}