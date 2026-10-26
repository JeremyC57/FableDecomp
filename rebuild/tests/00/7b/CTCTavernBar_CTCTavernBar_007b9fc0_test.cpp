// Behaviour test for CTCTavernBar::CTCTavernBar @ 0x007B9FC0: empty jug vector, null barman.
#include <cstdio>
class CThing { public: int id; };
class CTCBase { public: CThing* ParentThing; CTCBase(CThing* o) : ParentThing(o) {} };
struct CIntelligentPointer { void* p; CIntelligentPointer() : p(0) {} };
struct CVector { void* first; void* last; void* end; CVector() : first(0), last(0), end(0) {} };
class CTCTavernBar : public CTCBase { public: CVector Jugs; CIntelligentPointer Barman; CTCTavernBar(CThing* o) : CTCBase(o) {} };
int main()
{
    CThing th; CTCTavernBar b(&th);
    if (b.ParentThing != &th || b.Jugs.first || b.Jugs.last || b.Jugs.end || b.Barman.p) { std::printf("007b9fc0_TEST FAIL\n"); return 1; }
    std::printf("007b9fc0_TEST PASS\n");
    return 0;
}