#include <cstdio>
// Behaviour test for 00651a7e.

class CPlayerGui { public: int id; };
struct CRetailHandleVector { unsigned int reserved; CRetailHandleVector() : reserved(0) { } void Resize(long n) { reserved = (unsigned int)n; } };
struct CRetailEmitterVector { void* m_pFirst; void* m_pLast; void* m_pEnd; unsigned int reserved;
    CRetailEmitterVector() : m_pFirst(0), m_pLast(0), m_pEnd(0), reserved(0) { } void Resize(long n) { reserved = (unsigned int)n; } };
class CFadeInFadeOutBase { public: int built; CFadeInFadeOutBase() : built(1) { } };
struct CDrawBase {
    CPlayerGui* PlayerGui; void* DrawList; CRetailHandleVector PrimitiveHandles;
    CDrawBase(CPlayerGui* gui, void* list) : PlayerGui(gui), DrawList(list) { }
    virtual ~CDrawBase() { }
};

// NPlayerGui::CDrawAbilityButtons::CDrawAbilityButtons: three columns, empty handles and vectors, one effect slot per
// 0x5c-byte ability-button effect record in the GUI def.
struct CEffectDef { unsigned char body[0x5c]; };
class CPlayerGuiDef { public: CEffectDef* first; CEffectDef* last; };
static CEffectDef g_effects[5];
static CPlayerGuiDef g_def;
static CPlayerGuiDef* PeekGuiDef() { return &g_def; }
class CEnginePrimitiveHandle { public: void* Data; CEnginePrimitiveHandle() : Data(0) { } };
struct CDrawAbilityButtons : CDrawBase {
    long Columns; CEnginePrimitiveHandle BackgroundPrimitiveHandle, InteractBorderPrimitiveHandle; void* Special; CRetailEmitterVector Effects;
    CDrawAbilityButtons(CPlayerGui* gui, void* drawList)
        : CDrawBase(gui, drawList), Columns(3), BackgroundPrimitiveHandle(), InteractBorderPrimitiveHandle(), Special(0), Effects()
    { Effects.Resize((long)(PeekGuiDef()->last - PeekGuiDef()->first)); }
};
int main()
{
    g_def.first = g_effects; g_def.last = g_effects + 4;
    CPlayerGui gui;
    CDrawAbilityButtons b(&gui, 0);
    int ok = b.Columns == 3 && b.BackgroundPrimitiveHandle.Data == 0 && b.InteractBorderPrimitiveHandle.Data == 0 && b.Effects.reserved == 4;
    if (!ok) { std::printf("00651a7e_TEST FAIL\n"); return 1; }
    std::printf("00651a7e_TEST PASS\n");
    return 0;
}