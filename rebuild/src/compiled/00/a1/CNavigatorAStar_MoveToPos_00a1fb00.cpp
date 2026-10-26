// CNavigatorAStar::MoveToPos(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11) -- forward the eleven arguments to the route planner (0x00A1E5C0) with the 1e-4 tolerance (0x0129BA3C) inserted after the second and a 0 after the fourth
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A1FB00, length 80 bytes.
// Leaf: MoveToPos
// Callees: 0x00A1E5C0 (thiscall, 13 arguments; labelled CNavigatorAStar_MoveTowardGoal). Shape: no frame, each argument reloaded into eax/edx and pushed right to left, `ret 2Ch`.
class CNavigatorAStar { public:
    void MoveTowardGoal(long c1, long c2, long c3, long c4, long c5, long c6, long c7, long c8, long c9, long c10, long c11, long c12, long c13);   // 0x00A1E5C0
    void MoveToPos(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11);
};
extern float g_navigationEpsilon_0129ba3c;         // 0x0129BA3C: 1e-4f (copied as a dword)

void CNavigatorAStar::MoveToPos(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11)
{
    MoveTowardGoal(a1, a2, *(long*)&g_navigationEpsilon_0129ba3c, a3, a4, 0, a5, a6, a7, a8, a9, a10, a11);
}