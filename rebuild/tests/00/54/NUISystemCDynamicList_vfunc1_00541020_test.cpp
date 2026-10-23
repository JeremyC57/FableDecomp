// BEGIN exact source snapshot SHA256 c75853050cf57e674bfabc6f359690993223b88218f0593aecb69b2150bd1f9c
// 00541020 NUISystem::CDynamicList::Update, 250 retail bytes.
// Synchronize selected child with parent state after CChangingStateComponent::Update.
// Callee 0x0052c7e0: CChangingStateComponent::Update(float).
// Layout: no generated namespace class header exists. PDB CDynamicList SelectionIndex
// +0x19c and ScrollingChildren +0x1a4 are retail +0x16c/+0x174 (base -0x30).
// Update is recovered from base call/slot 1. State-accessor names are inferred.
namespace NUISystem {
struct CComponent {
    virtual void Slot000() = 0;
    virtual void Update(float delta) = 0;
    virtual void Slot002() = 0;
    virtual void Slot003() = 0;
    virtual void Slot004() = 0;
    virtual void Slot005() = 0;
    virtual void Slot006() = 0;
    virtual void Slot007() = 0;
    virtual void Slot008() = 0;
    virtual void Slot009() = 0;
    virtual void Slot010() = 0;
    virtual void Slot011() = 0;
    virtual void Slot012() = 0;
    virtual void Slot013() = 0;
    virtual void Slot014() = 0;
    virtual void Slot015() = 0;
    virtual void Slot016() = 0;
    virtual void Slot017() = 0;
    virtual void Slot018() = 0;
    virtual void Slot019() = 0;
    virtual void Slot020() = 0;
    virtual void Slot021() = 0;
    virtual void Slot022() = 0;
    virtual void Slot023() = 0;
    virtual void Slot024() = 0;
    virtual void Slot025() = 0;
    virtual void Slot026() = 0;
    virtual void Slot027() = 0;
    virtual void Slot028() = 0;
    virtual void Slot029() = 0;
    virtual void Slot030() = 0;
    virtual void Slot031() = 0;
    virtual void Slot032() = 0;
    virtual void Slot033() = 0;
    virtual void Slot034() = 0;
    virtual void Slot035() = 0;
    virtual void Slot036() = 0;
    virtual void Slot037() = 0;
    virtual void Slot038() = 0;
    virtual void Slot039() = 0;
    virtual void Slot040() = 0;
    virtual void Slot041() = 0;
    virtual void Slot042() = 0;
    virtual void Slot043() = 0;
    virtual void Slot044() = 0;
    virtual void Slot045() = 0;
    virtual void Slot046() = 0;
    virtual void Slot047() = 0;
    virtual void SetState(int state) = 0;
    virtual bool IsActive() = 0;
    virtual void Slot050() = 0;
    virtual void Slot051() = 0;
    virtual void Slot052() = 0;
    virtual void Slot053() = 0;
    virtual void Slot054() = 0;
    virtual void Slot055() = 0;
    virtual void Slot056() = 0;
    virtual void Slot057() = 0;
    virtual void Slot058() = 0;
    virtual void Slot059() = 0;
    virtual void Slot060() = 0;
    virtual void Slot061() = 0;
    virtual void Slot062() = 0;
    virtual void Slot063() = 0;
    virtual void Slot064() = 0;
    virtual void Slot065() = 0;
    virtual void Slot066() = 0;
    virtual void Slot067() = 0;
    virtual void Slot068() = 0;
    virtual void Slot069() = 0;
    virtual void Slot070() = 0;
    virtual void Slot071() = 0;
    virtual void Slot072() = 0;
    virtual void Slot073() = 0;
    virtual void Slot074() = 0;
    virtual void Slot075() = 0;
    virtual void Slot076() = 0;
    virtual void Slot077() = 0;
    virtual void Slot078() = 0;
    virtual void Slot079() = 0;
    virtual void Slot080() = 0;
    virtual void Slot081() = 0;
    virtual void Slot082() = 0;
    virtual void Slot083() = 0;
    virtual void Slot084() = 0;
    virtual void Slot085() = 0;
    virtual int GetActiveState() = 0;
    virtual int GetInactiveState() = 0;
};
struct CChangingStateComponent : CComponent {
    void Update(float delta); // 0x0052c7e0
};
struct CDynamicList : CChangingStateComponent {
    unsigned char InheritedState[0x168-4];
    int TopIndex;
    int SelectionIndex;
    int NumVisible;
    CComponent** ScrollingChildrenStart;
    CComponent** ScrollingChildrenFinish;
    CComponent** ScrollingChildrenLimit;
    void Update(float delta);
};
void CDynamicList::Update(float delta) {
    CChangingStateComponent::Update(delta);
    if (SelectionIndex >= 0 && (unsigned)SelectionIndex < (unsigned)(ScrollingChildrenFinish-ScrollingChildrenStart)) {
        if ((IsActive() && GetActiveState() == 1) || (!IsActive() && GetInactiveState() == 1)) {
            if (ScrollingChildrenStart[SelectionIndex]->IsActive()) {
                if (ScrollingChildrenStart[SelectionIndex]->GetActiveState() != 3)
                    ScrollingChildrenStart[SelectionIndex]->SetState(3);
            } else {
                if (ScrollingChildrenStart[SelectionIndex]->GetInactiveState() != 3)
                    ScrollingChildrenStart[SelectionIndex]->SetState(3);
            }
        }
    }
}
}

// END exact source snapshot
#include <stdio.h>
using namespace NUISystem;
static int updates;
void CChangingStateComponent::Update(float) { ++updates; }
struct Fixture : CDynamicList {
 bool active; int current, inactive, sets;
 Fixture() : active(true),current(1),inactive(1),sets(0) { SelectionIndex=-1; }
 void SetState(int s) { current=inactive=s; ++sets; }
 bool IsActive() { return active; }
 int GetActiveState() { return current; }
 int GetInactiveState() { return inactive; }
 void Slot000() {}
 void Slot002() {}
 void Slot003() {}
 void Slot004() {}
 void Slot005() {}
 void Slot006() {}
 void Slot007() {}
 void Slot008() {}
 void Slot009() {}
 void Slot010() {}
 void Slot011() {}
 void Slot012() {}
 void Slot013() {}
 void Slot014() {}
 void Slot015() {}
 void Slot016() {}
 void Slot017() {}
 void Slot018() {}
 void Slot019() {}
 void Slot020() {}
 void Slot021() {}
 void Slot022() {}
 void Slot023() {}
 void Slot024() {}
 void Slot025() {}
 void Slot026() {}
 void Slot027() {}
 void Slot028() {}
 void Slot029() {}
 void Slot030() {}
 void Slot031() {}
 void Slot032() {}
 void Slot033() {}
 void Slot034() {}
 void Slot035() {}
 void Slot036() {}
 void Slot037() {}
 void Slot038() {}
 void Slot039() {}
 void Slot040() {}
 void Slot041() {}
 void Slot042() {}
 void Slot043() {}
 void Slot044() {}
 void Slot045() {}
 void Slot046() {}
 void Slot047() {}
 void Slot050() {}
 void Slot051() {}
 void Slot052() {}
 void Slot053() {}
 void Slot054() {}
 void Slot055() {}
 void Slot056() {}
 void Slot057() {}
 void Slot058() {}
 void Slot059() {}
 void Slot060() {}
 void Slot061() {}
 void Slot062() {}
 void Slot063() {}
 void Slot064() {}
 void Slot065() {}
 void Slot066() {}
 void Slot067() {}
 void Slot068() {}
 void Slot069() {}
 void Slot070() {}
 void Slot071() {}
 void Slot072() {}
 void Slot073() {}
 void Slot074() {}
 void Slot075() {}
 void Slot076() {}
 void Slot077() {}
 void Slot078() {}
 void Slot079() {}
 void Slot080() {}
 void Slot081() {}
 void Slot082() {}
 void Slot083() {}
 void Slot084() {}
 void Slot085() {}
};
int main() {
 Fixture parent, child; CComponent* children[1]={&child};
 parent.ScrollingChildrenStart=children; parent.ScrollingChildrenFinish=children+1;
 parent.Update(0.125f); if(child.sets || updates!=1) return 1;
 parent.SelectionIndex=1; parent.Update(0.125f); if(child.sets) return 2;
 parent.SelectionIndex=0; parent.Update(0.125f); if(child.sets!=1 || child.current!=3) return 3;
 parent.Update(0.125f); if(child.sets!=1) return 4;
 parent.active=false; child.active=false; child.inactive=2;
 parent.Update(0.125f); if(child.sets!=2 || child.inactive!=3) return 5;
 parent.inactive=2; child.inactive=2; parent.Update(0.125f); if(child.sets!=2) return 6;
 puts("00541020_TEST PASS"); return 0;
}