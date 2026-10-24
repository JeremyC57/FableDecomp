#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/0069edc0.cpp) ----
// NDisplayView::CView3DBase::SetVWindow(const C2DBoxF& window) -- vtable slot 2: stores the view's screen window box
// Leaf: ?SetVWindow@CView3DBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069EDC0, 32 bytes.
// Name: FableWin.pdb ?SetVWindow@CView3DBase@NDisplayView@@UAEXABVC2DBoxF@@@Z; slot 2 of the vtable 0x0125C7E0
// (rtti-vtables.tsv; slot 3 is the landed Render 0x0069E980).
// Callees: none.
// Layout: Ego_r.pdb CView3DBase (156 bytes) = { vptr, CIEngine* PEngine +4, C2DBoxF Window +8,
// CCameraMovementInfo CameraMovementInfo +0x18 }; C2DBoxF is four floats (Left, Top, Right, Bottom), as the
// landed Render model.
// Shape: the implicit copy of the 16-byte POD box: `add ecx,8` forms &Window once, then four dword moves
// through edx (the last through eax). `ret 4`.
struct C2DBoxF { float Left, Top, Right, Bottom; };
struct CIEngine;

namespace NDisplayView {

struct CView3DBase {
    virtual void v00();
    CIEngine* PEngine;   // +0x04
    C2DBoxF   Window;    // +0x08
    void SetVWindow(const C2DBoxF& window);
};

void CView3DBase::SetVWindow(const C2DBoxF& window)
{
    Window = window;
}

} // namespace NDisplayView

// ---- stubs for the external callees ----
void NDisplayView::CView3DBase::v00() {}

static void run()
{

    NDisplayView::CView3DBase view;
    view.PEngine = 0;
    C2DBoxF zero = { 0.0f, 0.0f, 0.0f, 0.0f };
    view.Window = zero;
    C2DBoxF box = { 0.25f, 0.5f, 0.75f, 1.0f };
    view.SetVWindow(box);
    CHECK(view.Window.Left == 0.25f && view.Window.Top == 0.5f && view.Window.Right == 0.75f && view.Window.Bottom == 1.0f);
    CHECK(view.PEngine == 0);
}

int main()
{
    run();
    if (!g_ok) { std::printf("0069edc0_TEST FAIL\n"); return 1; }
    std::printf("0069edc0_TEST PASS\n");
    return 0;
}