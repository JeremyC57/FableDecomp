// CCombatRenderer::UpdateRenderState() (label propagated; a mixer-style update loop) -- when the dirty bit (+0xa4 bit 31)
// Grade: DIFFER(408v408) -- argument scheduling only: retail loads and pushes the change mask before reading the node's vtable for the slot-1 Evaluate call, ours reads the vtable first (4 instructions); tried reference/pointer/typed params, named node, signed mask
// is set clear it, refresh every node of the +0x48 list and fold the global change masks; then while the device has work:
// on the first pass (or after any node reported a timed change) time the pass with QueryPerformanceCounter, update the
// +0x34 nodes (re-evaluating them when dirty) and the +0x20 nodes, then process the +0x5c nodes, submit, and clear the masks
// Leaf: UpdateRenderState
union LARGE_INTEGER_ { struct { unsigned long LowPart; long HighPart; } u; __int64 QuadPart; };
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(LARGE_INTEGER_* count);   // [0x0143FE00]
extern unsigned long g_changeMaskA;                  // 0x0143BC24
extern unsigned long g_changeMaskB;                  // 0x0143BC28
struct CNodeA {                                       // +0x48 list
    unsigned char _pad[8];
    CNodeA* Next;                                     // +0x08
    void Refresh();                                   // 0x00C40590
};
struct CNodeB {                                       // +0x34 / +0x20 lists
    virtual void v00();
    virtual bool Evaluate(void* a, void* b, unsigned long changes);   // slot 1
    unsigned char _pad04[4];
    CNodeB* Next;                                     // +0x08
    unsigned char _pad0c[4];
    unsigned long Flags;                              // +0x10
    void Advance(__int64 elapsed);                    // 0x00C43A10
    void Recompute();                                 // 0x00C41FB0
};
struct CNodeC {                                       // +0x5c list
    unsigned char _pad[8];
    CNodeC* Next;                                     // +0x08
    void Rebuild();                                   // 0x00C40D10
    void Process();                                   // 0x00C40E20
};
struct CDevice {
    unsigned char _pad[0x190];
    __int64 LastTime;                                 // +0x190
    bool HasWork();                                   // 0x00C3E760
    __int64 GetElapsed() { LARGE_INTEGER_ now; QueryPerformanceCounter(&now); return now.QuadPart - LastTime; }
    void Mix();                                       // 0x00C3E800
    void Submit();                                    // 0x00C3E890
};
struct CCombatRenderer {
    void*   __vftable;
    CDevice* Device;                                  // +0x04
    unsigned char _pad08[0x20 - 0x08];
    CNodeB* ListB2;                                   // +0x20
    unsigned char _pad24[0x34 - 0x24];
    CNodeB* ListB;                                    // +0x34
    unsigned char _pad38[0x44 - 0x38];
    unsigned char ParamA[4];                          // +0x44
    CNodeA* ListA;                                    // +0x48
    unsigned char _pad4c[0x58 - 0x4c];
    unsigned char ParamB[4];                          // +0x58
    CNodeC* ListC;                                    // +0x5c
    unsigned char _pad60[0xa4 - 0x60];
    unsigned long DirtyFlags;                         // +0xa4
    void UpdateRenderState();
};

void CCombatRenderer::UpdateRenderState()
{
    bool dirty = (DirtyFlags >> 31) & 1;
    unsigned long changes;
    if (dirty) {
        DirtyFlags &= 0x7fffffff;
        for (CNodeA* a = ListA; a != 0; ) {
            CNodeA* next = a->Next;
            a->Refresh();
            a = next;
        }
        changes = g_changeMaskB | g_changeMaskA | DirtyFlags;
    } else
        changes = 0;
    bool timed = true;
    while (Device->HasWork()) {
        if (timed) {
            __int64 elapsed = Device->GetElapsed();
            timed = false;
            for (CNodeB* b = ListB; b != 0; ) {
                CNodeB* next = b->Next;
                if (dirty)
                    (b->Evaluate(ParamA, ParamB, changes) ? b : 0)->Recompute();
                if (b->Flags & 0x217) {
                    b->Advance(elapsed);
                    timed = true;
                }
                b = next;
            }
            Device->Mix();
            for (CNodeB* b2 = ListB2; b2 != 0; ) {
                CNodeB* next = b2->Next;
                if (b2->Flags & 0x217) {
                    b2->Advance(elapsed);
                    timed = true;
                }
                b2 = next;
            }
        }
        for (CNodeC* c = ListC; c != 0; ) {
            CNodeC* next = c->Next;
            if (dirty)
                c->Rebuild();
            c->Process();
            c = next;
        }
        Device->Submit();
        g_changeMaskA = 0;
        g_changeMaskB = 0;
        DirtyFlags = 0;
    }
}
