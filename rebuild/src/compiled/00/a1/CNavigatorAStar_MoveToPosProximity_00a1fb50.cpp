// CNavigatorAStar::MoveToPosProximity(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11, long a12) -- forward the twelve arguments to the route planner (0x00A1E5C0) with a 0 inserted after the fifth
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A1FB50, length 80 bytes.
// Leaf: MoveToPosProximity
// Callees: 0x00A1E5C0 (thiscall, 13 arguments; labelled CNavigatorAStar_MoveTowardGoal). Shape: no frame, each argument reloaded into eax/edx and pushed right to left, `ret 30h`.
struct CNavigatorAStar {
    void MoveTowardGoal(long c1, long c2, long c3, long c4, long c5, long c6, long c7, long c8, long c9, long c10, long c11, long c12, long c13);   // 0x00A1E5C0
    void MoveToPosProximity(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11, long a12);
};

void CNavigatorAStar::MoveToPosProximity(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11, long a12)
{
    MoveTowardGoal(a1, a2, a3, a4, a5, 0, a6, a7, a8, a9, a10, a11, a12);
}