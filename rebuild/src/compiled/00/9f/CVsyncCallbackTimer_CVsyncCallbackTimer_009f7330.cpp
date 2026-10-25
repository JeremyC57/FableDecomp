// CVsyncCallbackTimer::~CVsyncCallbackTimer() -- unoptimised destructor: clear every outstanding query, then destroy the two
// query lists in reverse member order (PendingQueories at +0x08 first, then FreeQueoryObjects at +0x04): each list frees its
// nodes and then its head node.
// Leaf: ??1CVsyncCallbackTimer@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F7330, length 107 bytes (+ int3 padding). Catalogued as
// CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures (a propagated label) and first landed as CPairedNodeLists.
// Evidence: Ego_r.pdb orders CVsyncCallbackTimer::ClearAllQueries (0x00B4ABF0), ~CVsyncCallbackTimer (0x00B4AD40),
// OnPreDeviceReset (0x00B4ADA0), OnPostSwapScreens (0x00B4ADC0); retail runs the same order 0x009F7250 / 0x009F7330 /
// 0x009F73A0 / 0x009F73C0, and this body's first call is 0x009F7250 on `this` (the call OnPreDeviceReset is made of).
// Ego_r.pdb layout: Device +0, FreeQueoryObjects / PendingQueories (list<CVsyncCallbackTimer::CPendingQuery>), then
// PendingFrame / FinishedFrame; retail's STLport list is one word (the head node), so the lists sit at +0x04 / +0x08.
// Callees: ClearAllQueries @ 0x009F7250, the list's node walk-and-free @ 0x009F7830 (on each list), free @ 0x00BFEA14.
// Shape: an unoptimised TU (`#pragma optimize("", off)`: `this` spilled to [ebp-14h] and reloaded); the list destructor is
// `__forceinline` (VC7.1 still inlines it at /Od), so each inlined call homes its own `this` and its head local in the
// frame ([ebp-8]/[ebp-4] for +0x08, [ebp-10h]/[ebp-0Ch] for +0x04); members die in reverse order.
#pragma optimize("", off)

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct IDirect3DDevice9;

// std::list<CVsyncCallbackTimer::CPendingQuery> as retail lays it out: the head node only.
struct CPendingQueryList {
    void* _M_node;                                 // head node
    void clear();                                  // 0x009F7830
    __forceinline ~CPendingQueryList()
    {
        clear();
        void* head = _M_node;
        if (head != 0)
            free(head);
    }
};

class CVsyncCallbackTimer {
public:
    ~CVsyncCallbackTimer();
private:
    void ClearAllQueries();                        // 0x009F7250
    IDirect3DDevice9*  Device;                     // +0x00
    CPendingQueryList  FreeQueoryObjects;          // +0x04
    CPendingQueryList  PendingQueories;            // +0x08
    long               PendingFrame;               // +0x0C
    long               FinishedFrame;              // +0x10
};

CVsyncCallbackTimer::~CVsyncCallbackTimer()
{
    ClearAllQueries();
}
