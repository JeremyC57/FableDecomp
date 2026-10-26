// 00541020 NUISystem::CDynamicList::Update, 250 retail bytes.
// Synchronize selected child with parent state after CChangingStateComponent::Update.
// Callee 0x0052c7e0: CChangingStateComponent::Update(float).
// Layout: no generated namespace class header exists. PDB CDynamicList SelectionIndex
// +0x19c and ScrollingChildren +0x1a4 are retail +0x16c/+0x174 (base -0x30).
// Update is recovered from base call/slot 1. State-accessor names are inferred.
namespace NUISystem {
class CComponent { public:
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
class CChangingStateComponent : public CComponent { public:
    void Update(float delta); // 0x0052c7e0
};
class CDynamicList : public CChangingStateComponent { public:
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