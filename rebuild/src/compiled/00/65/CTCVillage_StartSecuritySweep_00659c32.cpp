#pragma optimize("s", on)
// CTCVillage::StartSecuritySweep(bool) -- records whether the sweep is urgent; with sweep tasks already queued it invalidates and clears them, otherwise it stamps the start frame and resets the sweep radius to 20; then recalculates the sweep tasks
// Leaf: StartSecuritySweep
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00659C32, length 74 bytes. The catalog name "RecalculateSecuritySweep" is
// not a PDB name; the body is `?StartSecuritySweep@CTCVillage@@QAEX_N@Z` (one bool parameter, ret 4; the landed caller
// 0x00849250 passes 0).
// Callees: CTCVillage::InvalidateAndClearAllSecuritySweepTasks() @ 0x006590A0, CTCBase::GetWorld() const @ 0x00686A80
// (manifest "CViewBase::GetDisplayEngine" is propagated) and the static world-frame getter @ 0x0049D870 called
// through it, CTCVillage::CalculateSecuritySweepTasks() @ 0x00659800.
// Size TU (the shared tail call, the walk entered by `jmp`): `optimize("s",on)`.
// Shape: the retail std::list keeps no size word, so `size()` is the inline distance walk (`jmp` into the bottom
// test, count in edx); the urgency byte is stored between the count and its test, so the count is a named local taken
// first; 20.0f is an immediate store.
// Layout: CTCVillage.h is the PDB layout; with the 4-byte retail lists / 12-byte set: FrameStartedSecuritySweep +0x10,
// SecuritySweepTasks +0x5C, SecuritySweepUrgent +0xB4, InitialSecuritySweepRadius +0xB8 (PDB +0x140 / +0x144). Local model.
struct SSweepTaskNode {
    SSweepTaskNode* next;
    SSweepTaskNode* prev;
    void*           Object;
};

struct CSweepTaskList {                     // retail std::list<CCountedPointer<CVillageTaskSecuritySweep> >
    SSweepTaskNode* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SSweepTaskNode* p = _M_node->next; p != _M_node; p = p->next)
            ++n;
        return n;
    }
};

class CWorld { public:
    static long GetWorldFrame();            // 0x0049D870
};

class CTCVillage { public:
    void*          __vftable;               // +0x00
    unsigned char  _pad_0x04[0xC];
    long           FrameStartedSecuritySweep;   // +0x10
    unsigned char  _pad_0x14[0x48];
    CSweepTaskList SecuritySweepTasks;      // +0x5C
    unsigned char  _pad_0x60[0x54];
    bool           SecuritySweepUrgent;     // +0xB4
    float          InitialSecuritySweepRadius;  // +0xB8
    const CWorld* GetWorld() const;         // 0x00686A80 (CTCBase)
    void InvalidateAndClearAllSecuritySweepTasks();   // 0x006590A0
    void CalculateSecuritySweepTasks();     // 0x00659800
    void StartSecuritySweep(bool urgent);
};

void CTCVillage::StartSecuritySweep(bool urgent)
{
    unsigned long queued = SecuritySweepTasks.size();
    SecuritySweepUrgent = urgent;
    if (queued != 0) {
        InvalidateAndClearAllSecuritySweepTasks();
    } else {
        FrameStartedSecuritySweep = GetWorld()->GetWorldFrame();
        InitialSecuritySweepRadius = 20.0f;
    }
    CalculateSecuritySweepTasks();
}