// Behaviour test for CThingCreatureBase::CThingCreatureBase(long) (0x00666090): the members the ctor initialises
// (zeroed pointers/counters, self-linked queued-action list header, handedness 2, default flag bits).
#include <cstdio>
#include <cstdlib>

struct SNode { SNode* next; SNode* prev; void* data; void* info; };
struct List {
    SNode* node;
    List() : node(0)
    {
        SNode* n = (SNode*)std::malloc(sizeof(SNode));
        n->next = n;
        n->prev = n;
        node = n;
    }
};
struct Creature {
    long type;
    unsigned long LastMessageEventICreatedID;
    void* PDef;
    long ShotAccuracyPercentage;
    void* PCurrentActionData;
    List PQueuedActions;
    float TurnSpeed;
    int PreviousActionHandedness;
    char* DebugText;
    bool CurrentlyFrameUpdating : 1;
    bool Blinking : 1;
    bool MeleeAttacker : 1;
    bool MeleeAttacked : 1;
    bool LeaveDeadBodyOverride : 1;
    bool LeaveExperienceOrbsOverride : 1;
    bool FollowHeroThroughTeleporters : 1;
    bool HeadPosCached : 1;
    bool UseMovementInActions : 1;
    bool UpdateMovementVectorThisFrame : 1;
    bool IsShowingDebugInfo : 1;
    Creature(long t)
        : type(t), LastMessageEventICreatedID(0), PDef(0), ShotAccuracyPercentage(0), PCurrentActionData(0),
          TurnSpeed(0), PreviousActionHandedness(2), DebugText(0)
    {
        CurrentlyFrameUpdating = false; Blinking = false; MeleeAttacker = false; MeleeAttacked = false;
        LeaveDeadBodyOverride = true; LeaveExperienceOrbsOverride = true; FollowHeroThroughTeleporters = false;
        HeadPosCached = false; UseMovementInActions = true; UpdateMovementVectorThisFrame = true;
        IsShowingDebugInfo = false;
    }
};

int main()
{
    Creature c(13);
    if (c.type != 13 || c.LastMessageEventICreatedID != 0 || c.PDef != 0 || c.PCurrentActionData != 0 ||
        c.TurnSpeed != 0.0f || c.PreviousActionHandedness != 2 || c.DebugText != 0) {
        std::printf("00666090_TEST FAIL\n"); return 1;
    }
    if (c.PQueuedActions.node == 0 || c.PQueuedActions.node->next != c.PQueuedActions.node ||
        c.PQueuedActions.node->prev != c.PQueuedActions.node) { std::printf("00666090_TEST FAIL\n"); return 1; }
    if (!c.LeaveDeadBodyOverride || !c.LeaveExperienceOrbsOverride || !c.UseMovementInActions ||
        !c.UpdateMovementVectorThisFrame || c.Blinking || c.IsShowingDebugInfo || c.HeadPosCached) {
        std::printf("00666090_TEST FAIL\n"); return 1;
    }
    std::free(c.PQueuedActions.node);
    std::printf("00666090_TEST PASS\n");
    return 0;
}
