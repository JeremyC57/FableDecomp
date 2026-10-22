// NPlayerGui::CDrawAbilityButtonsPC::DrawExpressionButton(CEnginePrimitivePrepareInfo*, vector<long>*, long, long, long, float) -- draw one expression button: only for a live hero thing that has the expression component (0x6f); for a valid expression index inside the column-rounded list, place the button at the definition's expression origin plus the slot's button-type offset (X from the slot's type, Y from the first type) and the row step, look the expression def up by its global index, and draw the button with the def's graphic at full scale (or the default graphic at a third of the scale when there is no def), on layer 0x45 for type 1 and 0x44 otherwise
// Leaf: DrawExpressionButton
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658020, length 536 bytes (`ret 0x18`).
// Callees: GetPlayer 0x00643D7C (PlayerManager->GetMainPlayer), CPlayer::GetCharacterThing 0x00487DD0, button-type list fill
//   0x00657C80, LowerBound 0x004365B0 (const overload), PeekGuiDef 0x00438570, button vector size 0x0043CAC0 (0x5c entries),
//   GFA-to-screen position helper 0x00643D0C (ecx = out, edx = in), CDefinitionManager::GetPDefFromGlobalIndex 0x009AD9E0,
//   CDrawAbilityButtons::DrawButton 0x00645099, CRT free 0x00BFEA14 (vector storage).
// Layout: retail CDrawAbilityButtons Columns sits at +0x28 (PDB +0x2c; the draw base is 4 bytes smaller in retail); hero thing
//   component bits +0x20 (0x6f is word 3 bit 15), component map +0x44, life flags +0x91 as the landed RemoveAllHeroWeapons 0x00898600;
//   gui def +0x214 expression origin, +0x7a4 row offset, +0x7ac row step, +0x7b0 default graphic, +0x7fc ability / +0x808 expression
//   button vectors; expression def graphic +0x60. Modelled locally.
// Shape: one nested-if body (the early exits after the vector share retail's free tail); the def pointer's operator= and dtor are
//   __forceinline, the dtor releases through `if (--count == 0) Destroy()` and nulls; the definition manager is read into a named local
//   ahead of the argument; the layer is a named local defaulting to 0x44.
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
struct CRefCounted { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
struct CDefVirtual { virtual void v00(); virtual void Destroy(); };
struct CDefBase : CDefVirtual, CRefCounted {
    void Release() { if (--m_count == 0) Destroy(); }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CExpressionDef : CDefBase { unsigned char _pad08[0x58]; long Graphic; };   // +0x60
struct CDefPointer {
    CExpressionDef* m_p;
    CDefPointer() : m_p(0) {}
    __forceinline ~CDefPointer() { if (m_p) { m_p->Release(); m_p = 0; } }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) { m_p->ReleaseTemporary(); m_p = 0; }
            m_p = other.m_p;
            if (m_p) ++m_p->m_count;
        }
        return *this;
    }
};
struct CDefinitionManager { CDefPointer GetPDefFromGlobalIndex(long index) const; };   // 0x009AD9E0
struct CWorld { unsigned char _pad[0x10]; CDefinitionManager* Definitions; };
struct SButtonEntry { unsigned char _pad[0x54]; C2DVector Position; };   // 0x5c bytes
struct ButtonVector { SButtonEntry *Start, *Finish, *End; long size() const; };   // 0x0043CAC0
struct CPlayerGuiDef {
    unsigned char _pad00[0x214];
    C2DVector     ExpressionOrigin;             // +0x214
    unsigned char _pad21c[0x7a4 - 0x21c];
    float         ExpressionRowOffset;          // +0x7a4
    float         _f7a8;
    float         ExpressionRowStep;            // +0x7ac
    long          DefaultExpressionGraphic;     // +0x7b0
    unsigned char _pad7b4[0x7fc - 0x7b4];
    ButtonVector  AbilityButtons;               // +0x7fc
    ButtonVector  ExpressionButtons;            // +0x808
};
extern CPlayerGuiDef* __fastcall PeekGuiDef();  // 0x00438570
struct CEnginePrimitivePrepareInfo;
enum EEngine2DLayer { LAYER_44 = 0x44, LAYER_45 = 0x45 };

namespace NPlayerGui {
struct CDrawAbilityButtonsPC {
    void*           _vtable;
    void*           PlayerGui;              // +0x04
    CPlayerManager* PlayerManager;          // +0x08
    CWorld*         World;                  // +0x0c
    unsigned char   _pad10[0x28 - 0x10];
    long            Columns;                // +0x28
    CPlayer* GetPlayer();                   // 0x00643D7C
    void GetExpressionButtonTypes(LongVector& types);   // 0x00657C80
    void DrawButton(CEnginePrimitivePrepareInfo* info, C2DVector* pos, float scale, long button, long graphic, bool flag,
                    EEngine2DLayer layer);  // 0x00645099
    void DrawExpressionButton(CEnginePrimitivePrepareInfo* info, LongVector* expressions, long slot, long index, long row,
                              float scale);
};

void CDrawAbilityButtonsPC::DrawExpressionButton(CEnginePrimitivePrepareInfo* info, LongVector* expressions, long slot,
                                                 long index, long row, float scale)
{
    CThingNative* hero = GetPlayer()->GetCharacterThing();
    if (hero == 0 || (hero->LifeFlags & 1))
        return;
    LongVector types;
    GetExpressionButtonTypes(types);
    if (hero->UsingComponents[3] & 0x8000) {
        const ComponentMap& map = hero->Components;
        const ComponentEntry* it = map.LowerBound(0x6f);
        const ComponentEntry* last = map.Finish;
        if ((it == last || 0x6f < it->Type ? last : it)->Component) {
            if (index >= 0) {
                long count = expressions->size();
                if (index < (count + 2) / Columns * Columns && (unsigned int)index < expressions->size()) {
                    CPlayerGuiDef* def = PeekGuiDef();
                    long button = def->AbilityButtons.size() + index + 7;
                    C2DVector pos;
                    GFAPositionToScreen(&pos, &def->ExpressionOrigin);
                    pos.X += def->ExpressionButtons.Start[types[slot]].Position.X;
                    pos.Y += def->ExpressionButtons.Start[types[0]].Position.Y;
                    CDefPointer icon;
                    pos.Y += row * def->ExpressionRowStep;
                    pos.Y += def->ExpressionRowOffset;
                    if ((unsigned int)index < expressions->size()) {
                        CDefinitionManager* definitions = World->Definitions;
                        icon = definitions->GetPDefFromGlobalIndex((*expressions)[index]);
                    }
                    long graphic = def->DefaultExpressionGraphic;
                    float buttonScale = scale;
                    if (icon.m_p)
                        graphic = icon.m_p->Graphic;
                    else
                        buttonScale = scale * 0.33333334f;
                    EEngine2DLayer layer = LAYER_44;
                    if (types[slot] == 1)
                        layer = LAYER_45;
                    DrawButton(info, &pos, buttonScale, button, graphic, false, layer);
                }
            }
        }
    }
}
}
