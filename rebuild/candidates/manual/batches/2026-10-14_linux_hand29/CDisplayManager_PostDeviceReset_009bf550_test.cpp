#include <cstdio>
struct IDirect3DDevice9 { int x; };
static IDirect3DDevice9* g_init = 0; static int g_sys = 0;
struct CRenderManagerCore { void Init(IDirect3DDevice9* d) { g_init = d; } };
struct CSystemManager { void OnDisplayDeviceReset() { ++g_sys; } };
struct CDeviceResetCallback { virtual void v00() { } virtual bool OnDeviceReset() = 0; };
struct CYes : CDeviceResetCallback { int n; CYes() : n(0) { } bool OnDeviceReset() { ++n; return true; } };
struct CNo : CDeviceResetCallback { bool OnDeviceReset() { return false; } };
struct CRetailCallbackList { struct node { node* next; node* prev; CDeviceResetCallback* value; }; node* head; };
struct CDisplayManager {
    void* __vftable; CSystemManager* SystemManager; CRenderManagerCore* PRenderManager; unsigned char _pad00c[0x58 - 0x0c]; IDirect3DDevice9* D3DDevice;
    unsigned char _pad05c[0x1b0 - 0x5c]; bool UsingRenderManager; unsigned char _pad1b1[0x264 - 0x1b1]; CRetailCallbackList DeviceResetCallbacks;
    bool PostDeviceReset() {
        if (UsingRenderManager) PRenderManager->Init(D3DDevice);
        for (CRetailCallbackList::node* n = DeviceResetCallbacks.head->next; n != DeviceResetCallbacks.head; n = n->next) if (!n->value->OnDeviceReset()) return false;
        SystemManager->OnDisplayDeviceReset(); return true; } };
int main() {
    CSystemManager sys; CRenderManagerCore rm; IDirect3DDevice9 dev; CYes a, b; CNo no;
    CRetailCallbackList::node sentinel, n1, n2, n3; sentinel.next = &n1; n1.next = &n2; n2.next = &sentinel; n1.value = &a; n2.value = &b;
    CDisplayManager m; m.SystemManager = &sys; m.PRenderManager = &rm; m.D3DDevice = &dev; m.UsingRenderManager = true; m.DeviceResetCallbacks.head = &sentinel;
    if (!m.PostDeviceReset() || g_init != &dev || a.n != 1 || b.n != 1 || g_sys != 1) { std::printf("009bf550_TEST FAIL\n"); return 1; }
    n2.next = &n3; n3.next = &sentinel; n3.value = &no; m.UsingRenderManager = false; g_init = 0;
    if (m.PostDeviceReset() || g_init != 0 || g_sys != 1 || a.n != 2) { std::printf("009bf550_TEST FAIL\n"); return 1; } std::printf("009bf550_TEST PASS\n"); return 0; }
