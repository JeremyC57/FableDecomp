// NCameraModes::CStringAndCageBase::UpdateXYVars() -- ease the current string length and cage radius a quarter of the way toward their targets (the cage eases toward the string length while the expansion timer runs)
// Grade: DIFFER(124v124) -- after the first getter call retail loads `fld [esi+0ACh]` before `mov eax,[esp+4]`; ours the other way round (scheduling only)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00871FD0, 124 bytes. Callees: a CBase getter @ 0x0084D080 (returns &m_40->[+0x78];
//          the catalog label "CVertexBufferWin32::DoSizeof" is a byte-twin propagation), called for its effect-free result.
// Layout: Ego_r.pdb members 0x24 lower in retail (see the CStringAndCage ctor 0x0069DE30).
// Shape: each eased value is a named float local stored before the getter call and copied to the member after it;
//        the cage target stays on the FPU stack across the timer decrement.
struct CTimeInfo;
namespace NCameraModes {
struct CBase {
    virtual void v00();
    unsigned char _body[0xa0];                     // +0x04..+0xa3 (retail)
    const CTimeInfo* GetFrameInfo() const;         // 0x0084D080
};
struct __declspec(novtable) CStringAndCageBase : CBase {
    float DesiredHeightOffset;                     // +0xa4
    float DesiredStringLength;                     // +0xa8
    float DesiredCageRadius;                       // +0xac
    float CurrentHeightOffset;                     // +0xb0
    float CurrentStringLength;                     // +0xb4
    float CurrentCageRadius;                       // +0xb8
    long  NextAutoGoBehindWF;                      // +0xbc
    long  TimerForExpansion;                       // +0xc0
    bool  DriftToMaxStringLength;                  // +0xc4
    void UpdateXYVars();
    float Ease(float current, float target) const
    {
        float v = (current * 3.0f + target) * 0.25f;
        GetFrameInfo();
        return v;
    }
};

void CStringAndCageBase::UpdateXYVars()
{
    CurrentStringLength = Ease(CurrentStringLength, DesiredStringLength);
    float target = DesiredCageRadius;
    if (TimerForExpansion > 0) {
        --TimerForExpansion;
        target = DesiredStringLength;
    }
    CurrentCageRadius = Ease(CurrentCageRadius, target);
}
}
