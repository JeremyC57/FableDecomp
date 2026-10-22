// Behaviour test for CAIStateGroup_MoveGoodsBase::FetchGoods (0x00915890): model copy, scripted stubs.
#include <cstdio>
#include <cstdlib>

struct CThing;
struct CTCCrateStack {
    CThing* crates[4];
    int count;
    CThing* GetNextCrate() { return count ? crates[count - 1] : 0; }
    int GetNumCrates() { return count; }
    CThing* RemoveNextCrate() { return count ? crates[--count] : 0; }
};
struct CTCUserActionPointBase {
    int freed;
    void FreeActionPoint(int index) { freed = index; }
};
struct CThing {
    bool dead;
    CTCCrateStack* stack;
    CTCUserActionPointBase* point;
};
struct CIntelligentPointerThing {
    CThing* m_pItem;
    CIntelligentPointerThing() : m_pItem(0) { }
    CIntelligentPointerThing(CThing* p) : m_pItem(p) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* p) { m_pItem = p; }
};
static bool IS_THING_ALIVE(const CIntelligentPointerThing& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->dead)
            return true;
    }
    return false;
}
struct CLongVector {
    int* Start;
    CLongVector(int a, int b, int c) { Start = (int*)std::malloc(3 * sizeof(int)); Start[0] = a; Start[1] = b; Start[2] = c; }
    CLongVector(const CLongVector& o) { Start = (int*)std::malloc(3 * sizeof(int)); for (int i = 0; i < 3; ++i) Start[i] = o.Start[i]; }
    ~CLongVector() { if (Start) std::free(Start); }
};
static int g_moveResult, g_actionResult, g_lastGood, g_moves;
static CThing* g_lastTarget;
static CThing* g_killOnMove;
struct CAIStateGroup_MoveGoodsBase {
    int CurrentActionID;
    int AIAction_MoveToPossiblyBlockedObjectAndFace(CIntelligentPointerThing target, int* point)
    {
        ++g_moves; g_lastTarget = target.GetPItem(); *point = 7;
        if (g_killOnMove) g_killOnMove->dead = true;
        return g_moveResult;
    }
    bool FetchGoods(CIntelligentPointerThing container, CLongVector goods);
};

bool CAIStateGroup_MoveGoodsBase::FetchGoods(CIntelligentPointerThing container, CLongVector goods)
{
    CIntelligentPointerThing target;
    target.SetPItem(container.GetPItem());
    CTCCrateStack* stack = 0;
    if (container.GetPItem()->stack) {
        stack = container.GetPItem()->stack;
        target = CIntelligentPointerThing(stack->GetNextCrate());
    }
    int actionPoint;
    if (AIAction_MoveToPossiblyBlockedObjectAndFace(target, &actionPoint) == 2 && IS_THING_ALIVE(target)) {
        int* data = goods.Start;
        int index;
        CTCCrateStack* crates = container.GetPItem()->stack;
        if (crates)
            index = crates->GetNumCrates() - 1;
        else
            index = 0;
        CThing* item = target.GetPItem();
        g_lastGood = data[index];
        g_lastTarget = item;
        CurrentActionID = 0x55;
        if (g_actionResult == 2) {
            if (IS_THING_ALIVE(target))
                target.GetPItem()->point->FreeActionPoint(actionPoint);
            if (container.GetPItem() && stack)
                stack->RemoveNextCrate();
            return true;
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    CTCUserActionPointBase pt = { -1 };
    CThing crateA = { false, 0, &pt };
    CThing crateB = { false, 0, &pt };
    CTCCrateStack st = { { &crateA, &crateB, 0, 0 }, 2 };
    CThing pile = { false, &st, &pt };
    CThing barrel = { false, 0, &pt };
    CLongVector goods(10, 20, 30);
    CAIStateGroup_MoveGoodsBase s;
    s.CurrentActionID = 0;
    g_moveResult = 2; g_actionResult = 2;
    // a crate stack: fetch the top crate's good, free its point, pop it
    if (!s.FetchGoods(CIntelligentPointerThing(&pile), goods) || g_lastTarget != &crateB || g_lastGood != 20
            || st.count != 1 || pt.freed != 7 || s.CurrentActionID != 0x55) ok = 0;
    // a plain container: target it directly, first good
    pt.freed = -1;
    if (!s.FetchGoods(CIntelligentPointerThing(&barrel), goods) || g_lastTarget != &barrel || g_lastGood != 10 || pt.freed != 7) ok = 0;
    // the walk fails
    g_moveResult = 3;
    if (s.FetchGoods(CIntelligentPointerThing(&pile), goods) || st.count != 1) ok = 0;
    g_moveResult = 2;
    // the action fails: nothing popped
    g_actionResult = 3;
    if (s.FetchGoods(CIntelligentPointerThing(&pile), goods) || st.count != 1) ok = 0;
    g_actionResult = 2;
    // the target dies during the walk
    g_killOnMove = &crateA;
    if (s.FetchGoods(CIntelligentPointerThing(&pile), goods) || st.count != 1) ok = 0;
    g_killOnMove = 0;
    if (!ok) { std::printf("00915890_TEST FAIL\n"); return 1; }
    std::printf("00915890_TEST PASS\n");
    return 0;
}
