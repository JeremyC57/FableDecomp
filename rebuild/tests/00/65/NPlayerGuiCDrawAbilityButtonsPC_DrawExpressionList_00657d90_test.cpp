// Behaviour test for NPlayerGui::CDrawAbilityButtonsPC::DrawExpressionList (0x00657D90): model and body verbatim from final.cpp
// (the same-TU GetExpressionSlotOffsets stand-in included), the player / component map / expressions / draw callees stubbed.
// Checks the dead-hero and no-component exits, and that the buttons drawn for a scrolled list match a plain reference walk
// (rows at and after the scroll row first, then the rows before it, each row drawn only when one of its slots holds an expression).
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __declspec(x)
#define __fastcall
#define _snprintf snprintf
#endif
#include <stdlib.h>
class C2DVector { public: float X, Y; };
void __fastcall GFAPositionToScreen(C2DVector* out, const C2DVector* in);   // 0x00643D0C
struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry *Start, *Finish;
    ComponentEntry* LowerBound(const long& key);                 // 0x0040F020
    const ComponentEntry* LowerBound(const long& key) const;     // 0x004365B0
};
struct CThingNative {
    void* VTable; unsigned char Base[0x1c]; unsigned int UsingComponents[9]; ComponentMap Components;
    unsigned char State[0x45]; unsigned char LifeFlags;
};
class CPlayer { public: CThingNative* GetCharacterThing(); };          // 0x00487DD0
class CPlayerManager;
struct LongVector {
    long *Start, *Finish, *End;
    LongVector() : Start(0), Finish(0), End(0) {}
    ~LongVector() { if (Start) free(Start); }
    unsigned int size() const { return Finish - Start; }
    long& operator[](unsigned int i) { return Start[i]; }
};
class CEnginePrimitivePrepareInfo;
enum EEngine2DLayer { LAYER_44 = 0x44, LAYER_45 = 0x45 };

struct CTCHeroExpressions {
    unsigned char _pad00[0x1b8];
    long          ScrollIndex;                  // +0x1b8
    void GetExpressions(LongVector& out);       // 0x005EED8C
};

namespace NPlayerGui {
struct CDrawAbilityButtonsPC {
    void*           PlayerGui;              // +0x04
    CPlayerManager* PlayerManager;          // +0x08
    void*           World;                  // +0x0c
    unsigned char   _pad10[0x28 - 0x10];
    long            Columns;                // +0x28
    CPlayer* GetPlayer();                   // 0x00643D7C
    void GetExpressionSlotOffsets(LongVector& offsets);   // 0x00657BA0
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void DrawExpressionButton(CEnginePrimitivePrepareInfo* info, LongVector* expressions, long slot, long index,
                                      long row, float scale);   // slot 9
    void DrawExpressionList(CEnginePrimitivePrepareInfo* info, float scale);
};

__declspec(noinline) void CDrawAbilityButtonsPC::GetExpressionSlotOffsets(LongVector& offsets)
{
    for (long i = 0; i < Columns; ++i) {
        if (offsets.Finish == offsets.End) {
            long n = (long)(offsets.Finish - offsets.Start);
            long* grown = (long*)malloc((n + 4) * sizeof(long));
            for (long j = 0; j < n; ++j)
                grown[j] = offsets.Start[j];
            if (offsets.Start)
                free(offsets.Start);
            offsets.Start = grown;
            offsets.Finish = grown + n;
            offsets.End = grown + n + 4;
        }
        *offsets.Finish++ = i * Columns + 1;
    }
}
void CDrawAbilityButtonsPC::DrawExpressionList(CEnginePrimitivePrepareInfo* info, float scale)
{
    CThingNative* hero = GetPlayer()->GetCharacterThing();
    if (hero == 0 || (hero->LifeFlags & 1))
        return;
    LongVector offsets;
    GetExpressionSlotOffsets(offsets);
    if (hero->UsingComponents[3] & 0x8000) {
        const ComponentMap& map = hero->Components;
        const ComponentEntry* it = map.LowerBound(0x6f);
        const ComponentEntry* last = map.Finish;
        CTCHeroExpressions* tc = (CTCHeroExpressions*)(it == last || 0x6f < it->Type ? last : it)->Component;
        if (tc) {
            scale *= 0.75f;
            LongVector expressions;
            tc->GetExpressions(expressions);
            long firstRow = tc->ScrollIndex / Columns + 1;
            long rows = (long)expressions.size() / Columns * Columns / Columns;
            long row = firstRow;
            long drawn = firstRow - 1;
            for (; row < rows; ++row) {
                bool any = false;
                long col;
                for (col = 0; col < Columns; ++col) {
                    unsigned int index = offsets.Start[col] + row * Columns - 1;
                    if (index < expressions.size() && expressions[index] > 0) {
                        any = true;
                        break;
                    }
                }
                if (any) {
                    ++drawn;
                    for (col = 0; col < Columns; ++col)
                        DrawExpressionButton(info, &expressions, col, offsets.Start[col] + row * Columns - 1, drawn - firstRow, scale);
                }
            }
            for (long row2 = 0; row2 < firstRow - 1; ++row2) {
                bool any = false;
                long col;
                for (col = 0; col < 3; ++col) {
                    unsigned int index = offsets.Start[col] + row2 * Columns - 1;
                    if (index < expressions.size() && expressions[index] > 0) {
                        any = true;
                        break;
                    }
                }
                if (any) {
                    ++drawn;
                    for (col = 0; col < Columns; ++col)
                        DrawExpressionButton(info, &expressions, col, offsets.Start[col] + row2 * Columns - 1, drawn - firstRow, scale);
                }
            }
        }
    }
}
}


// ---- stubs ----
static CThingNative g_hero; static CPlayer g_player; static CTCHeroExpressions g_tc; static ComponentEntry g_entries[2];
static long g_expr[32]; static int g_exprCount = 0; static int g_drawn = 0;
static long g_drawSlot[64], g_drawIndex[64], g_drawRow[64]; static float g_drawScale[64];
void __fastcall GFAPositionToScreen(C2DVector* out, const C2DVector* in) { *out = *in; }
CThingNative* CPlayer::GetCharacterThing() { return &g_hero; }
CPlayer* NPlayerGui::CDrawAbilityButtonsPC::GetPlayer() { return &g_player; }
const ComponentEntry* ComponentMap::LowerBound(const long& key) const
{
    const ComponentEntry* it = Start;
    while (it != Finish && it->Type < key) ++it;
    return it;
}
ComponentEntry* ComponentMap::LowerBound(const long& key) { return (ComponentEntry*)((const ComponentMap*)this)->LowerBound(key); }
void CTCHeroExpressions::GetExpressions(LongVector& out)
{
    out.Start = (long*)malloc(sizeof(long) * (g_exprCount ? g_exprCount : 1));
    for (int i = 0; i < g_exprCount; ++i) out.Start[i] = g_expr[i];
    out.Finish = out.Start + g_exprCount; out.End = out.Finish;
}
void NPlayerGui::CDrawAbilityButtonsPC::DrawExpressionButton(CEnginePrimitivePrepareInfo*, LongVector*, long slot, long index, long row, float scale)
{
    if (g_drawn < 64) { g_drawSlot[g_drawn] = slot; g_drawIndex[g_drawn] = index; g_drawRow[g_drawn] = row; g_drawScale[g_drawn] = scale; }
    g_drawn++;
}
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
// reference walk: slot offsets are i*Columns+1 (the stand-in), index = slot + row*Columns - 1
static void Reference(long columns, long scroll, long* outSlot, long* outIndex, long* outRow, int* outCount)
{
    int n = 0; long firstRow = scroll / columns + 1; long rows = g_exprCount / columns; long drawn = firstRow - 1;
    for (long row = firstRow; row < rows; ++row) {
        bool any = false;
        for (long col = 0; col < columns && !any; ++col) { long idx = (col * columns + 1) + row * columns - 1; if (idx < g_exprCount && g_expr[idx] > 0) any = true; }
        if (any) { ++drawn; for (long col = 0; col < columns; ++col) { outSlot[n] = col; outIndex[n] = (col * columns + 1) + row * columns - 1; outRow[n] = drawn - firstRow; n++; } }
    }
    for (long row = 0; row < firstRow - 1; ++row) {
        bool any = false;
        for (long col = 0; col < 3 && !any; ++col) { long idx = (col * columns + 1) + row * columns - 1; if (idx < g_exprCount && g_expr[idx] > 0) any = true; }
        if (any) { ++drawn; for (long col = 0; col < columns; ++col) { outSlot[n] = col; outIndex[n] = (col * columns + 1) + row * columns - 1; outRow[n] = drawn - firstRow; n++; } }
    }
    *outCount = n;
}
int main()
{
    memset(&g_hero, 0, sizeof(g_hero)); memset(&g_tc, 0, sizeof(g_tc));
    g_entries[0].Type = 0x6f; g_entries[0].Component = &g_tc;
    g_hero.Components.Start = g_entries; g_hero.Components.Finish = g_entries + 1;
    NPlayerGui::CDrawAbilityButtonsPC gui; gui.PlayerGui = 0; gui.PlayerManager = 0; gui.World = 0; gui.Columns = 3;
    // 1. dead hero: nothing drawn
    g_hero.LifeFlags = 1; g_drawn = 0; gui.DrawExpressionList(0, 1.0f); CHECK(g_drawn == 0);
    // 2. alive, no expressions component: nothing drawn
    g_hero.LifeFlags = 0; g_hero.UsingComponents[3] = 0; g_drawn = 0; gui.DrawExpressionList(0, 1.0f); CHECK(g_drawn == 0);
    // 3. component present, 15 expressions, scrolled to index 4: compare with the reference walk
    g_hero.UsingComponents[3] = 0x8000; g_exprCount = 15;
    static const long pattern[15] = { 1, 0, 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0 };
    for (int i = 0; i < 15; ++i) g_expr[i] = pattern[i];
    g_tc.ScrollIndex = 4; g_drawn = 0; gui.DrawExpressionList(0, 2.0f);
    long rs[64], ri[64], rr[64]; int rn = 0; Reference(3, 4, rs, ri, rr, &rn);
    CHECK(g_drawn == rn && rn > 0);
    for (int k = 0; k < rn && k < 64; ++k)
        CHECK(g_drawSlot[k] == rs[k] && g_drawIndex[k] == ri[k] && g_drawRow[k] == rr[k] && g_drawScale[k] == 1.5f);
    // 4. no scroll, dense list: every row drawn in order
    for (int j = 0; j < 15; ++j) g_expr[j] = j + 1;
    g_tc.ScrollIndex = 0; g_drawn = 0; gui.DrawExpressionList(0, 1.0f); Reference(3, 0, rs, ri, rr, &rn);
    CHECK(g_drawn == rn);
    for (int m = 0; m < rn && m < 64; ++m) CHECK(g_drawSlot[m] == rs[m] && g_drawIndex[m] == ri[m] && g_drawRow[m] == rr[m]);
    char line[64];
    _snprintf(line, sizeof(line), "00657d90_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}