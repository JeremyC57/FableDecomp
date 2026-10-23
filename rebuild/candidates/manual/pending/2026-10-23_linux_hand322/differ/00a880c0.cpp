// CSBCCorridor::Process(const CSteeringAgent* agent) -- when steering is globally disabled just pass the desired velocity through; otherwise mark the behaviour active and, when the corridor has points, steer the agent back toward the corridor's nearest segment
// Grade: DIFFER(115v1768) -- skeleton draft, parked on sight: 1.7 KB x87 steering maths in an 8-byte aligned frame
//   (doubles); only the disabled-steering early-out is modelled.
// Leaf: ?Process@CSBCCorridor@@UAE_NPBVCSteeringAgent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A880C0, 1776 bytes, `ret 4`, 8-byte aligned frame (double locals).
// Skeleton draft: only the disabled-steering early-out and the corridor point count are modelled.
struct C2DVector { float X, Y; };
struct CSteeringAgent { char Pad[0x20]; float Speed; };
struct CPointNode { CPointNode* _M_next; CPointNode* _M_prev; C2DVector Point; };
extern bool g_bSteeringDisabled;                  // 0x013D2D60
struct CSBCCorridor {
    void*       VTable;
    C2DVector   Result;                           // +0x04
    C2DVector   Desired;                          // +0x0c
    bool        Active;                           // +0x14
    CPointNode* Points;                           // +0x18
    float       Speed;                            // +0x1c
    virtual bool Process(const CSteeringAgent* agent);
};
bool CSBCCorridor::Process(const CSteeringAgent* agent)
{
    if (g_bSteeringDisabled) {
        Result = Desired;
        return true;
    }
    Active = true;
    long count = 0;
    for (CPointNode* node = Points->_M_next; node != Points; node = node->_M_next)
        ++count;
    if (count == 0)
        return false;
    double delta = Speed - agent->Speed;
    if (delta < 0)
        delta = -delta;
    if (delta > 1e-4)
        Speed = agent->Speed;
    return true;
}
