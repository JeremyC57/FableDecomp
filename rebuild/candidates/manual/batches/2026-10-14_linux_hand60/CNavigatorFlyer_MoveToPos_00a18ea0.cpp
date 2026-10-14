// CNavigatorFlyer::MoveToPos(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11) -- forward the eleven arguments to this navigator's virtual slot 4 with the 1e-4 tolerance (0x0129BA3C) inserted after the second
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A18EA0, length 80 bytes.
// Leaf: MoveToPos
// Shape: no frame, every argument reloaded into edx and pushed right to left, the vtable fetched mid-way, `ret 2Ch`.
extern float g_navigationEpsilon_0129ba3c;         // 0x0129BA3C: 1e-4f (copied as a dword)
struct CNavigatorFlyer {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void MoveTowardGoal(long c1, long c2, long c3, long c4, long c5, long c6, long c7, long c8, long c9, long c10, long c11, long c12);   // slot 4 (+0x10)
    void MoveToPos(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11);
};

void CNavigatorFlyer::MoveToPos(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11)
{
    MoveTowardGoal(a1, a2, *(long*)&g_navigationEpsilon_0129ba3c, a3, a4, a5, a6, a7, a8, a9, a10, a11);
}
