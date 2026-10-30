// NCameraModes::CStringAndCageBase::UpdateXYVars() -- ease the current string length and cage radius a quarter of the way toward their targets (the cage eases toward the string length while the expansion timer runs)
// Grade: DIFFER(124v124) -- wave 2 sec3: the effect-free getter is the PDB protected CBase::GetCamTools() 0x0084D080 and the easing is the inline PDB CTools::GetRollingAverage<float>(cur, target, 4.0f) (constants 3.0/0.25 exact); a same-TU GetCamTools body lets VC7.1 drop both calls (so retail had it declared only). Residue unchanged: fld [esi+0ACh] scheduled after the result reload (retail before). Tried: same-TU getter body, declared-only, result held in a local until after the target load
// Leaf: ?UpdateXYVars@CStringAndCageBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00871FD0, 124 bytes. Callees: a CBase getter @ 0x0084D080 (returns &m_40->[+0x78];
//          the catalog label "CVertexBufferWin32::DoSizeof" is a byte-twin propagation), called for its effect-free result.
// Layout: Ego_r.pdb members 0x24 lower in retail (see the CStringAndCage ctor 0x0069DE30).
// Shape: each eased value is a named float local stored before the getter call and copied to the member after it;
//        the cage target stays on the FPU stack across the timer decrement.
namespace NCameraModes {
class CTools {
public:
    template <class T> T GetRollingAverage(const T& current, const T& target, float weight) const   // Ego_r ??$GetRollingAverage@M@CTools@NCameraModes@@QBEMABM0M@Z (inline)
    {
        return (current * (weight - 1.0f) + target) / weight;
    }
};
}
class CGameCameraManager {
public:
    unsigned char _pad00[0x78];
    NCameraModes::CTools Tools;                    // +0x78
};
namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    unsigned char _pad04[0x40 - 0x04];
    CGameCameraManager* PCameraManager;            // +0x40
    unsigned char _pad44[0xa4 - 0x44];
protected:
    const CTools& GetCamTools() const;             // 0x0084D080 (Ego_r ?GetCamTools@CBase@NCameraModes@@IBEABVCTools@2@XZ)
};
class __declspec(novtable) CStringAndCageBase : public CBase {
public:
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
};

void CStringAndCageBase::UpdateXYVars()
{
    float length = GetCamTools().GetRollingAverage(CurrentStringLength, DesiredStringLength, 4.0f);
    float target = DesiredCageRadius;
    CurrentStringLength = length;
    if (TimerForExpansion > 0) {
        --TimerForExpansion;
        target = DesiredStringLength;
    }
    CurrentCageRadius = GetCamTools().GetRollingAverage(CurrentCageRadius, target, 4.0f);
}
}
