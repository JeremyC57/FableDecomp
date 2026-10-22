// NPlayerGui::CDrawAbilityButtonsPC::DrawExpressionList(CEnginePrimitivePrepareInfo*, float) -- draw the hero's expression grid: for a live hero with the expression component, scale by 0.75, fetch the expression list, and starting at the row after the scroll row draw every row that holds at least one expression, then wrap round to the rows before the scroll row, numbering the drawn rows consecutively
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00657D90, length 653 bytes (`ret 8`).
// Callees: GetPlayer 0x00643D7C, CPlayer::GetCharacterThing 0x00487DD0, slot-offset fill 0x00657BA0 (same-TU stand-in body below),
//   LowerBound 0x004365B0 (const), CTCHeroExpressions::GetExpressions 0x005EED8C, DrawExpressionButton (virtual slot 9, 0x00658020),
//   CRT free 0x00BFEA14.
// Layout: as DrawExpressionButton 0x00658020 (Columns +0x28; hero component bits +0x20, map +0x44, life flags +0x91); the
//   expressions component scroll index +0x1b8. Modelled locally.
// Grade note: matched by advisor A3. The slot-offset fill is a same-TU noinline stand-in that reads Start/Finish/End before writing,
//   so the vector stops escaping and _M_start stays in ebp (rematerialised from the vector) as in retail; `long row = firstRow;` is
//   declared before `long drawn = firstRow - 1;`; the wrap-round scan runs on its own counter.
// Leaf: DrawExpressionList
#include <stdlib.h>
struct C2DVector { float X, Y; };
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
struct CPlayer { CThingNative* GetCharacterThing(); };          // 0x00487DD0
struct CPlayerManager;
struct LongVector {
    long *Start, *Finish, *End;
    LongVector() : Start(0), Finish(0), End(0) {}
    ~LongVector() { if (Start) free(Start); }
    unsigned int size() const { return Finish - Start; }
    long& operator[](unsigned int i) { return Start[i]; }
};
struct CEnginePrimitivePrepareInfo;
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
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
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
