// Behaviour test for 0x006DDAE0 CTCTrophy::CTCTrophy(CThing*): member defaults, self-linked lists, conditional frame update.
#include <cstdio>
#include <cstdlib>

struct CThingState { unsigned char m_flags; };
static int g_forced = 0;
class CThing { public:
    CThingState* m_pState;
    void SetForceFrameUpdate(bool force) { if (force) ++g_forced; }
};
class CTCBase { public:
    CThing* ParentThing;
    CTCBase(CThing* parent) : ParentThing(parent) {}
    virtual ~CTCBase() {}
};
struct CDefPointer { const void* m_pDef; CDefPointer() : m_pDef(0) {} };
struct CList {
    struct node { node* next; node* prev; unsigned char value[8]; };
    node* head;
    CList() : head(0)
    {
        node* sentinel = (node*)std::malloc(sizeof(node));
        sentinel->next = sentinel; sentinel->prev = sentinel; head = sentinel;
    }
};
class CTCTrophy : public CTCBase { public:
    CDefPointer PDef; long NonMinigameDeedHandle; bool MinigameInProgress;
    long BestWitnessesAheadToDate, MinigameInitialFramesLeft, MinigameCurrentFramesLeft, MinigameStartingWitnessCount,
         NumberOfWitnessesLeft, PendingGainedWitnessSounds, FrameForNextGainedWitnessSound;
    CList AvailableWitnesses, WitnessesAchieved;
    bool WaitingForConfirmation, WaitingForScore, WaitingForTerminate, WaitingForNextInstructionsPage, IsGlobalTC;
    long CurrentInstructionsPage; bool Mountable;
    CTCTrophy(CThing* parent)
        : CTCBase(parent), NonMinigameDeedHandle(-1), MinigameInProgress(false), BestWitnessesAheadToDate(0),
          MinigameInitialFramesLeft(0), MinigameCurrentFramesLeft(0), MinigameStartingWitnessCount(0), NumberOfWitnessesLeft(0),
          PendingGainedWitnessSounds(0), FrameForNextGainedWitnessSound(0), WaitingForConfirmation(false), WaitingForScore(false),
          WaitingForTerminate(false), WaitingForNextInstructionsPage(false), IsGlobalTC(false), CurrentInstructionsPage(0), Mountable(true)
    {
        if (parent->m_pState && !(parent->m_pState->m_flags & 2))
            parent->SetForceFrameUpdate(true);
    }
};

int main()
{
    CThingState st; st.m_flags = 0;
    CThing a; a.m_pState = 0;
    CTCTrophy t1(&a);
    if (g_forced != 0 || t1.NonMinigameDeedHandle != -1 || !t1.Mountable || t1.PDef.m_pDef != 0 || t1.CurrentInstructionsPage != 0
        || t1.AvailableWitnesses.head->next != t1.AvailableWitnesses.head || t1.WitnessesAchieved.head->prev != t1.WitnessesAchieved.head
        || t1.AvailableWitnesses.head == t1.WitnessesAchieved.head || t1.ParentThing != &a) { std::printf("006ddae0_TEST FAIL\n"); return 1; }
    a.m_pState = &st;
    CTCTrophy t2(&a);
    if (g_forced != 1) { std::printf("006ddae0_TEST FAIL\n"); return 1; }
    st.m_flags = 2;
    CTCTrophy t3(&a);
    if (g_forced != 1) { std::printf("006ddae0_TEST FAIL\n"); return 1; }
    std::printf("006ddae0_TEST PASS\n");
    return 0;
}