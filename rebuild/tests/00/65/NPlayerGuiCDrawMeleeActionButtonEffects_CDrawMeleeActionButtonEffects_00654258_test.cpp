#include <cstdio>
class CPlayerGui { public: int x; }; static CPlayerGui* g_gui = 0; static long g_handles = 0, g_emitters = 0;
namespace NPlayerGui {
struct CRetailHandleVector { void* m_pFirst; void* m_pLast; void* m_pEnd; void Reserve(long n) { g_handles = n; } };
struct CRetailEmitterVector { void* m_pFirst; void* m_pLast; void* m_pEnd; CRetailEmitterVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } void Reserve(long n) { g_emitters = n; } };
class CDrawBase { public: virtual void v00() { } unsigned char _body[0x1c - 4]; CRetailHandleVector PrimitiveHandles; CDrawBase(CPlayerGui* g, void* l) { g_gui = g; } };
class CDrawMeleeActionButtonEffects : public CDrawBase { public: CRetailEmitterVector Emitters; long FlashTimer, ComboLoadingIconTimer, FlourishCounter, FlourishEffect; struct CDefPointer_Melee { void* m_p; CDefPointer_Melee() : m_p(0) { } } PMeleeAbilityDef;
    CDrawMeleeActionButtonEffects(CPlayerGui* g, void* l) : CDrawBase(g, l), Emitters(), FlashTimer(-1), ComboLoadingIconTimer(-1), FlourishCounter(-1), FlourishEffect(0), PMeleeAbilityDef() { Emitters.Reserve(3); PrimitiveHandles.Reserve(1); } }; }
int main() { CPlayerGui gui; int list; NPlayerGui::CDrawMeleeActionButtonEffects d(&gui, &list);
    if (g_gui != &gui || d.Emitters.m_pFirst || d.Emitters.m_pEnd || d.FlashTimer != -1 || d.ComboLoadingIconTimer != -1 || d.FlourishCounter != -1 || d.FlourishEffect || d.PMeleeAbilityDef.m_p || g_emitters != 3 || g_handles != 1) { std::printf("00654258_TEST FAIL\n"); return 1; } std::printf("00654258_TEST PASS\n"); return 0; }