#include <stdio.h>

struct IDirect3DIndexBuffer9;
struct IDirect3DIndexBuffer9Vtbl {
    void (__stdcall *QueryInterface)(IDirect3DIndexBuffer9*);
    void (__stdcall *AddRef)(IDirect3DIndexBuffer9*);
    void (__stdcall *Release)(IDirect3DIndexBuffer9*);
};
struct IDirect3DIndexBuffer9 {
    IDirect3DIndexBuffer9Vtbl* lpVtbl;
};

class CIndexBuffer {
public:
    void Uninitialise();
    void*                  __vftable;
    IDirect3DIndexBuffer9* PD3DIndexBuffer;
};

static int g_released = 0;
static IDirect3DIndexBuffer9* g_last = 0;
static void __stdcall MyRelease(IDirect3DIndexBuffer9* r) { g_released++; g_last = r; }

int main() {
    IDirect3DIndexBuffer9Vtbl vt;
    vt.QueryInterface = 0; vt.AddRef = 0; vt.Release = MyRelease;
    IDirect3DIndexBuffer9 res;
    res.lpVtbl = &vt;

    CIndexBuffer v;
    v.__vftable = 0;
    v.PD3DIndexBuffer = &res;

    v.Uninitialise();
    bool ok = (g_released == 1) && (g_last == &res) && (v.PD3DIndexBuffer == 0);

    CIndexBuffer v2;
    v2.__vftable = 0;
    v2.PD3DIndexBuffer = 0;
    v2.Uninitialise();
    ok = ok && (g_released == 1) && (v2.PD3DIndexBuffer == 0);

    if (ok) printf("RELEASEVOLUME_OK\n");
    else printf("RELEASEVOLUME_FAIL rel=%d\n", g_released);
    return ok ? 0 : 1;
}