#include <cstdio>

// Model of CTCHeroCentre::CTCHeroCentre(CThing*) @ 0x0077D470: null def, GUI mode and quest-card frame zeroed, both
// vectors empty, not waiting for a tutorial.
struct CThing { int id; };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* t) : ParentThing(t) { } virtual ~CTCBase() { } };
struct CDefPointer { void* Object; CDefPointer() : Object(0) { } };
template<class T> struct CRetailVector { T* m_pFirst; T* m_pLast; T* m_pEnd; CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    long size() const { return (long)(m_pLast - m_pFirst); } };
struct C3DVector { float X, Y, Z; };
struct CTCHeroCentre : CTCBase {
    CDefPointer PDef; long CurrentGUIMode; unsigned long FrameQuestCardListFrom;
    CRetailVector<void*> QuestCards; CRetailVector<C3DVector> QuestCardPositions; bool WaitingForTutorialToFinish;
    CTCHeroCentre(CThing* thing) : CTCBase(thing), CurrentGUIMode(0), FrameQuestCardListFrom(0), WaitingForTutorialToFinish(false) { }
};

int main()
{
    CThing t; t.id = 2;
    CTCHeroCentre* c = new CTCHeroCentre(&t);
    bool ok = c->ParentThing == &t && c->PDef.Object == 0 && c->CurrentGUIMode == 0 && c->FrameQuestCardListFrom == 0 &&
              c->QuestCards.size() == 0 && c->QuestCards.m_pEnd == 0 && c->QuestCardPositions.size() == 0 &&
              c->QuestCardPositions.m_pFirst == 0 && !c->WaitingForTutorialToFinish;
    delete c;
    if (!ok) { std::printf("0077d470_TEST FAIL\n"); return 1; }
    std::printf("0077d470_TEST PASS\n");
    return 0;
}