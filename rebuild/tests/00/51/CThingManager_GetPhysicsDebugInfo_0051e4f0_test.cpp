// Behaviour test for 0051e4f0: body copied from src/0051e4f0.cpp (CThingManager::GetPhysicsDebugInfo), the virtual callee stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CPhysicsDebugInfo { public: int visits; };
struct CThingPhysics {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {}
    virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {}
    virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {}
    virtual void v015() {} virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {}
    virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {} virtual void v024() {}
    virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {}
    virtual void v030() {} virtual void v031() {} virtual void v032() {} virtual void v033() {} virtual void v034() {}
    virtual void v035() {} virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {}
    virtual void v040() {} virtual void v041() {} virtual void v042() {} virtual void v043() {} virtual void v044() {}
    virtual void v045() {} virtual void v046() {} virtual void v047() {} virtual void v048() {} virtual void v049() {}
    virtual void v050() {} virtual void v051() {} virtual void v052() {} virtual void v053() {} virtual void v054() {}
    virtual void v055() {} virtual void v056() {} virtual void v057() {} virtual void v058() {} virtual void v059() {}
    virtual void v060() {} virtual void v061() {} virtual void v062() {} virtual void v063() {} virtual void v064() {}
    virtual void v065() {} virtual void v066() {} virtual void v067() {} virtual void v068() {} virtual void v069() {}
    virtual void v070() {} virtual void v071() {} virtual void v072() {} virtual void v073() {} virtual void v074() {}
    virtual void v075() {} virtual void v076() {} virtual void v077() {} virtual void v078() {} virtual void v079() {}
    virtual void v080() {} virtual void v081() {} virtual void v082() {} virtual void v083() {} virtual void v084() {}
    virtual void v085() {} virtual void v086() {} virtual void v087() {} virtual void v088() {}
    virtual void GetPhysicsDebugInfo(CPhysicsDebugInfo* info) { ++info->visits; ++calls; }
    int calls; CThingPhysics() : calls(0) {}
};
class CThing { public:
    unsigned char  _pad00[0x60];
    CThingPhysics* PPhysics;
};
struct SThingListNode {
    SThingListNode* PNext;
    SThingListNode* PPrev;
    CThing*         PThing;
};
struct CThingList {
    SThingListNode* PHead;
    SThingListNode* begin() const { return PHead->PNext; }
    SThingListNode* end() const { return PHead; }
};
class CThingManager { public:
    unsigned char _pad00[0x14];
    unsigned char _alloc14[4];
    CThingList    Things;
    void GetPhysicsDebugInfo(CPhysicsDebugInfo* info);
};
void CThingManager::GetPhysicsDebugInfo(CPhysicsDebugInfo* info)
{
    for (SThingListNode* it = Things.begin(); it != Things.end(); it = it->PNext) {
        CThing* thing = it->PThing;
        if (thing->PPhysics)
            thing->PPhysics->GetPhysicsDebugInfo(info);
    }
}
int main()
{
    CThingPhysics p1, p3; CThing t1, t2, t3; t1.PPhysics = &p1; t2.PPhysics = 0; t3.PPhysics = &p3;
    SThingListNode head, n1, n2, n3; head.PNext = &n1; n1.PNext = &n2; n2.PNext = &n3; n3.PNext = &head;
    n1.PThing = &t1; n2.PThing = &t2; n3.PThing = &t3;
    CThingManager m; m.Things.PHead = &head; CPhysicsDebugInfo info = { 0 };
    // 1. three things, one without physics: two visits
    m.GetPhysicsDebugInfo(&info); CHECK(info.visits == 2); CHECK(p1.calls == 1); CHECK(p3.calls == 1);
    // 2. empty list: nothing
    head.PNext = &head; m.GetPhysicsDebugInfo(&info); CHECK(info.visits == 2);
    if (fails == 0) printf("0051e4f0_TEST PASS\n"); else printf("0051e4f0_TEST FAIL\n");
    return fails;
}