// Behaviour test for 0x0055BAE0 NUISystem::CHoverable::OnHovered: the requested state is saved as StateBeforeHovered, the component changes to its definition's HoveredState (the def pointer released afterwards), then the on-hovered actions run
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static int g_step = 0, g_destroyed = 0;
struct CUIDef { int RefCount; int HoveredState; void DecRef() { if (--RefCount == 0) ++g_destroyed; } };
struct CDefPointer { CUIDef* Object;
    explicit CDefPointer(CUIDef* o) : Object(o) { if (Object) ++Object->RefCount; }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    CUIDef* operator->() const { return Object; } ~CDefPointer() { if (Object) Object->DecRef(); } };
static CUIDef g_def;
struct CHoverable {
    unsigned RequestedState, StateBeforeHovered, changedTo; void* PActionsOnHovered; void* ranActions; int changeStep, actionStep, releasedBeforeActions;
    CDefPointer LoadAndReturnDef() const { return CDefPointer(&g_def); }
    void ChangeState(unsigned s) { changedTo = s; changeStep = ++g_step; }
    void ExecuteActions(void* l) { ranActions = l; actionStep = ++g_step; releasedBeforeActions = (g_def.RefCount == 1); }
    void OnHovered() { StateBeforeHovered = RequestedState; ChangeState(LoadAndReturnDef()->HoveredState); ExecuteActions(PActionsOnHovered); }
};
int main()
{
    g_def.RefCount = 1; g_def.HoveredState = 5;
    int actions; CHoverable h; h.RequestedState = 3; h.StateBeforeHovered = 0; h.PActionsOnHovered = &actions;
    h.OnHovered();
    CHECK(h.StateBeforeHovered == 3 && h.changedTo == 5 && h.ranActions == &actions);
    CHECK(h.changeStep == 1 && h.actionStep == 2 && h.releasedBeforeActions && g_def.RefCount == 1 && g_destroyed == 0);
    if (fails == 0) printf("0055bae0_TEST PASS\n"); else printf("0055bae0_TEST FAIL\n");
    return fails;
}