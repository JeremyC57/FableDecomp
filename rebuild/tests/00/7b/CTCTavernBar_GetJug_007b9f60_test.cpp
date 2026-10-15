// Behaviour test for CTCTavernBar::GetJug @ 0x007B9F60: the index-th jug, a null pointer past the end (unsigned compare).
#include <cstdio>
struct CThingObject { int id; };
struct CIntelligentPointer {
    CThingObject* p;
    CIntelligentPointer(CThingObject* t) : p(t) {}
    CIntelligentPointer(const CIntelligentPointer& o) : p(o.p) {}
};
struct CTCTavernBar {
    CIntelligentPointer* first; CIntelligentPointer* last;
    unsigned long size() const { return (unsigned long)(last - first); }
    CIntelligentPointer GetJug(unsigned long index)
    {
        if (index >= size()) return CIntelligentPointer(0);
        return first[index];
    }
};
int main()
{
    CThingObject a, b; a.id = 1; b.id = 2;
    CIntelligentPointer jugs[2] = { CIntelligentPointer(&a), CIntelligentPointer(&b) };
    CTCTavernBar bar; bar.first = jugs; bar.last = jugs + 2;
    if (bar.GetJug(0).p != &a || bar.GetJug(1).p != &b || bar.GetJug(2).p != 0 || bar.GetJug((unsigned long)-1).p != 0) { std::printf("007b9f60_TEST FAIL\n"); return 1; }
    std::printf("007b9f60_TEST PASS\n");
    return 0;
}