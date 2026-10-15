// CRenderLayerWin32::ApplySlotB(long a, long b) -- update slot set 1 from m_slotB; when it succeeds and m_slotB already matched m_slotA in x/y, mirror m_slotB into m_slotA and the set-1 state word into set 0's
// Leaf: ApplySlotB
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4D4B0, length 110 bytes.
// Label correction: the catalog "CShaderConstantRange::operator!=" is propagated and wrong. The body is a
// member of the Win32 render layer class whose vtable sits at 0x012B5B80 (no RTTI; "Win32" tag string just
// before it): 0x00C4D440 is reached from its slot 2 through the tail-jump stub 0x00C3E330, 0x00C4D4B0 is
// called directly (0x00C43BDD). The class name and member names are descriptive.
// Callee: the shared slot-update helper @ 0x00C4D240 (returns 0 on success; its third argument points at
// a global counter, the fourth at a shared global, the fifth selects the slot set).
// Globals: 0x0139C98C / 0x0139C9A4 (the two slot sets' current-state words), 0x0139CA10 (shared state).
// Shape: returns the helper's result (left in eax, which is why the copies rotate through ecx/edx
// only); the slot is a reference local held in edi; `bool same = a.x == b.x && a.y == b.y;` (the `mov bl,1` / `xor bl,bl` pair) computed before the
// call; `if (Update(...) == 0 && same)` then four field copies and one global word copy.
struct SStateSlot {                                  // 20 bytes
    long id;                                         // +0x00
    long x, y, z, w;                                 // +0x04 .. +0x10
};

extern long g_slotStateA;                            // 0x0139C98C
extern long g_slotStateB;                            // 0x0139C9A4
extern long g_sharedSlotState;                       // 0x0139CA10

struct CRenderLayerWin32 {
    virtual void v00();
    SStateSlot m_slotA;                              // +0x04
    SStateSlot m_slotB;                              // +0x18
    long UpdateSlot(long a, SStateSlot* slot, long* state, long* shared, long set, long b);   // 0x00C4D240
    long ApplySlotA(long a, long b);
    long ApplySlotB(long a, long b);
};

long CRenderLayerWin32::ApplySlotB(long a, long b)
{
    SStateSlot& src = m_slotB;
    bool same = m_slotA.x == src.x && m_slotA.y == src.y;
    long result = UpdateSlot(a, &src, &g_slotStateB, &g_sharedSlotState, 1, b);
    if (result == 0 && same) {
        m_slotA.x = src.x;
        m_slotA.y = src.y;
        m_slotA.z = src.z;
        m_slotA.w = src.w;
        g_slotStateA = g_slotStateB;
    }
    return result;
}
