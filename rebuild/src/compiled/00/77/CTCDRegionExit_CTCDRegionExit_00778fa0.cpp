// CTCDRegionExit::CTCDRegionExit(CThing*) -- CTCDriverBase(thing), own vtable, the exit flags (active, waiting for activation / for the player to leave the message area), unit radius, no connected entrance, the entrance intelligent pointer constructed, then the parent thing's cutscene pause behaviour set to 2
// Leaf: ??0CTCDRegionExit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00778FA0, length 94 bytes (+ int3 padding).
// Callees: CTCDriverBase::CTCDriverBase(CThing*) @ 0x00771200 (installs vtable 0x0126A79C = CTCDriverBase),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the CIntelligentPointer vtable
// 0x01238C6C), CThing::SetCutscenePauseBehaviour @ 0x004C78A0. Vtable 0x0126AB54 = CTCDRegionExit
// (rtti-vtables.tsv): the chunk label "CTCChest::CTCChest" is propagated.
// Layout = Ego_r.pdb CTCDRegionExit (0x38 bytes, no retail shift): Active +0xc, InLetterBoxMode +0xd,
// WaitingForConfirmation +0xe, WaitingForActivationOrPlayerToLeave +0xf,
// WaitingForPlayerToLeaveMessageArea +0x10, Radius +0x14, MessageRadius +0x18,
// EntranceConnectedToUID +0x20, ReversedOnMiniMap +0x28, HiddenOnMiniMap +0x29, PEntranceConnectedTo
// +0x2a (CIntelligentPointer has alignment 1: it is declared under pack(1)).
// Shape: plain initialisers (the three `true` bytes share cl and are scheduled before the vtable store,
// the zeroes share eax, the 1.0f radius is an immediate), the pointer member addressed once in edi,
// the cutscene call is the constructor body.
struct CThing {
    void SetCutscenePauseBehaviour(long behaviour);         // 0x004C78A0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* m_pItem;                                          // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
};
struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
#pragma pack(pop)

struct CTCDriverBase_Calls {                                // CTCBase layout (0xc bytes), vtable 0x0126A79C
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCDriverBase_Calls(CThing* thing);                     // 0x00771200
};

struct CTCDRegionExit : CTCDriverBase_Calls {               // vtable 0x0126AB54
    bool                Active;                             // +0x0c
    bool                InLetterBoxMode;                    // +0x0d
    bool                WaitingForConfirmation;             // +0x0e
    bool                WaitingForActivationOrPlayerToLeave;// +0x0f
    bool                WaitingForPlayerToLeaveMessageArea; // +0x10
    float               Radius;                             // +0x14
    float               MessageRadius;                      // +0x18
    unsigned __int64    EntranceConnectedToUID;             // +0x20
    bool                ReversedOnMiniMap;                  // +0x28
    bool                HiddenOnMiniMap;                    // +0x29
    CIntelligentPointer PEntranceConnectedTo;               // +0x2a
    virtual void v00();
    CTCDRegionExit(CThing* thing);
};

CTCDRegionExit::CTCDRegionExit(CThing* thing)
    : CTCDriverBase_Calls(thing)
    , Active(true)
    , InLetterBoxMode(false)
    , WaitingForConfirmation(false)
    , WaitingForActivationOrPlayerToLeave(true)
    , WaitingForPlayerToLeaveMessageArea(true)
    , Radius(1.0f)
    , MessageRadius(0.0f)
    , EntranceConnectedToUID(0)
    , ReversedOnMiniMap(false)
    , HiddenOnMiniMap(false)
    , PEntranceConnectedTo()
{
    ParentThing->SetCutscenePauseBehaviour(2);
}