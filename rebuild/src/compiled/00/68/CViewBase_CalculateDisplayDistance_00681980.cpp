// CViewBase::CalculateDisplayDistance() -- the view distance minus the near offset: the fixed distance when it is set, otherwise the opinion manager's tolerance for the viewed thing plus the target's own offset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681980, length 80 bytes.
// Callees: CViewBase::GetDisplayEngine @ 0x00686A80 (on `this`), COpinionReactionManager::GetToleranceToBeingHit(CThing*)
// @ 0x00701940 (the manager is the engine record's +0x74).
// Shape: `fld [esi+14h] ; fcomp [0.0f] ; fnstsw ; test ah,1 ; jne` is `m_distance >= 0.0f` (C0 only) taken
// as the simple `return m_distance - m_nearOffset` path; otherwise the thing pointer is a NAMED LOCAL loaded into edi before
// the GetDisplayEngine call (a member read as the argument is loaded after the call by the lane), the float result
// stays on the FPU stack, `if (m_pTarget) d += m_pTarget->m_offset` (+0x50), `fsub [esi+10h]` on both paths.
// No engine header for CViewBase (the vtable-less fields +4, +0x10, +0x14, +0x20 are modelled from the reads).
struct CThing;

struct COpinionReactionManager {
    float GetToleranceToBeingHit(CThing* thing);   // 0x00701940
};

struct CDisplayEngine {
    unsigned char            _pad00[0x74];
    COpinionReactionManager* m_pOpinionManager;    // +0x74
};

struct CViewTarget {
    unsigned char _pad00[0x50];
    float         m_displayOffset;                 // +0x50
};

struct CViewBase {
    virtual void v00();
    CThing*       m_pThing;                        // +0x04
    unsigned char _pad08[8];
    float         m_nearOffset;                    // +0x10
    float         m_distance;                      // +0x14 (negative = derive it from the opinion tolerance)
    unsigned char _pad18[8];
    CViewTarget*  m_pTarget;                       // +0x20
    CDisplayEngine* GetDisplayEngine();            // 0x00686A80
    float CalculateDisplayDistance();
};

float CViewBase::CalculateDisplayDistance()
{
    if (m_distance >= 0.0f)
        return m_distance - m_nearOffset;
    CThing* thing = m_pThing;                  // named local: loaded into edi before the GetDisplayEngine call
    COpinionReactionManager* manager = GetDisplayEngine()->m_pOpinionManager;   // the chained object in a local: the lane pushes the argument before the call otherwise
    float distance = manager->GetToleranceToBeingHit(thing);
    if (m_pTarget)
        distance += m_pTarget->m_displayOffset;
    return distance - m_nearOffset;
}