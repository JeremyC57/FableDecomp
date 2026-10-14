// CAIStateGroup_GoToPub::PayAtBar() -- walk to the pay point and pay; terminate the group on any failure.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094AAC0, length 60 bytes.
// Callees: CAIStateGroup_GoToPub::GetPayPoint @ 0x009492A0, WalkToPoint(C3DVector*, CRightHandedSet*) @
// 0x0094A6D0, PayMoney @ 0x0094A840, CAIStateGroupBase::Terminate @ 0x008FCF40 (`Terminated = true`,
// manifest label "CXMVPlaybackInfo" is propagated).
//
// Shape: three guarded steps, each with its OWN `{ Terminate(); return false; }` block; VC's tail merging
// folds the three blocks into one epilogue, and the `mov ecx,esi` hoisted ahead of the second branch (so
// the WalkToPoint miss jumps past it) is the fingerprint of that form -- an `&&` chain or nested ifs with
// one Terminate make all three misses jump to the same label. The pay point is a C3DVector at +0x78 and its
// orientation set follows at +0x84. No engine header exists for CAIStateGroup_GoToPub; the layout is local.
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector X, Y, Z; };

struct CAIStateGroupBase {
    virtual void v00();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    unsigned char _pub[0x64];                   // +0x14 pub/bar pointers, timers, bar point index (+0x74)
    C3DVector m_payPoint;                       // +0x78
    CRightHandedSet m_payOrientation;           // +0x84
    bool GetPayPoint();                         // 0x009492A0
    bool WalkToPoint(C3DVector* pPoint, CRightHandedSet* pOrientation);   // 0x0094A6D0
    bool PayMoney();                            // 0x0094A840
    bool PayAtBar();
};

bool CAIStateGroup_GoToPub::PayAtBar()
{
    if (!GetPayPoint()) {
        Terminate();
        return false;
    }
    if (!WalkToPoint(&m_payPoint, &m_payOrientation)) {
        Terminate();
        return false;
    }
    if (!PayMoney()) {
        Terminate();
        return false;
    }
    return true;
}
