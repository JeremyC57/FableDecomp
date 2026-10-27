// CTCDRegionExit::OnInitialActivate() -- when a connected-entrance UID was loaded, resolve it to the entrance thing
// Leaf: ?OnInitialActivate@CTCDRegionExit@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00779420, 14 bytes, plain `ret` / tail jump.
// Identity: CTCDRegionExit vtable 0x0126AB54 slot 6 (RTTI .?AVCTCDRegionExit@@). CTCBase slot 6 is OnInitialActivate:
//   the CTCBase virtuals in Ego_r.pdb donor order are OnRemove (slot 2), ConstructFromParams, OnCreate (4),
//   InitialActivate (5), OnInitialActivate (6), RenderUpdate, HasFrameUpdate (8), FrameUpdate (9) ...; Ego_r.pdb
//   `?OnInitialActivate@CTCDRegionExit@@UAEXXZ` is in the name set. The landed callee's own comment already names the slot.
// Callees: CTCDRegionExit::SetUpPEntranceConnectedToFromUID() @ 0x00779340 (tail jump).
// Layout: Ego_r.pdb CTCDRegionExit as in the landed 0x00779340 model: EntranceConnectedToUID (unsigned __int64) at +0x20.
// Shape: the 64-bit non-zero test is `mov eax,lo ; or eax,hi ; je`; the call is a sibling tail jump.
class CThing;

class CTCDRegionExit {
public:
    virtual void OnInitialActivate();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    bool    Active;                             // +0x0c
    bool    InLetterBoxMode;                    // +0x0d
    bool    WaitingForConfirmation;             // +0x0e
    bool    WaitingForActivationOrPlayerToLeave;   // +0x0f
    bool    WaitingForPlayerToLeaveMessageArea;    // +0x10
    float   Radius;                             // +0x14
    float   MessageRadius;                      // +0x18
    unsigned __int64 EntranceConnectedToUID;    // +0x20
    void SetUpPEntranceConnectedToFromUID();    // 0x00779340
};

void CTCDRegionExit::OnInitialActivate()
{
    if (EntranceConnectedToUID)
        SetUpPEntranceConnectedToFromUID();
}