// Behaviour test for 0x00B918B0 CEngineSceneGridCell::MovePrimitivesTo: all primitives move, counts and validity bits update.
#include <cstdio>

struct CCell;
struct CPrim {
    CCell* m_pCell; CPrim* m_pNextInCell; CPrim** m_pHead;
    void RemoveFromList() { *m_pHead = m_pNextInCell; m_pNextInCell = 0; }   // always the list head in this test
    void AddToList(CPrim** head) { m_pNextInCell = *head; *head = this; m_pHead = head; }
};
static int g_invalidated = 0;
struct CCell {
    CCell* Parent; CPrim* FirstPrimitive; long LocalCount; unsigned char Flags;
    void InvalidateAll() { ++g_invalidated; Flags &= ~7; }
    void Invalidate() { if (Flags & 7) { Flags &= ~7; if (Parent) Parent->InvalidateAll(); } }
    void AddPrimitive(CPrim* p) { p->m_pCell = this; p->AddToList(&FirstPrimitive); ++LocalCount; Invalidate(); }
    void MovePrimitivesTo(CCell* dest)
    {
        CPrim* p = FirstPrimitive;
        while (p) { CPrim* next = p->m_pNextInCell; p->RemoveFromList(); dest->AddPrimitive(p); p = next; }
        Invalidate();
    }
};

int main()
{
    CCell parent; parent.Parent = 0; parent.FirstPrimitive = 0; parent.LocalCount = 0; parent.Flags = 0x0f;
    CCell a; a.Parent = &parent; a.FirstPrimitive = 0; a.LocalCount = 0; a.Flags = 0x18;
    CCell b; b.Parent = &parent; b.FirstPrimitive = 0; b.LocalCount = 5; b.Flags = 0x07;
    CPrim p[3];
    for (int i = 2; i >= 0; --i) a.AddPrimitive(&p[i]);
    a.Flags = 0x1a;
    a.MovePrimitivesTo(&b);
    if (b.LocalCount != 8 || a.FirstPrimitive != 0 || b.FirstPrimitive != &p[2] || p[0].m_pCell != &b || p[1].m_pCell != &b
        || (b.Flags & 7) || a.Flags != 0x18 || g_invalidated != 2) { std::printf("00b918b0_TEST FAIL\n"); return 1; }
    std::printf("00b918b0_TEST PASS\n");
    return 0;
}