// CInputProcessStrafe::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- for the strafe action's analogue vector (game action 0x3c; a keyboard event only when it is a button release), queue game event 0x2b for the live controlled creature: its unique id, the vector, and the key (-1 for non-keyboard devices)
// Leaf: ?ProcessInput@CInputProcessStrafe@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00688A80, length 283 bytes (+ int3 padding), `ret 8`.
// Callees: CPlayer::GetControlledCreature @ 0x00487DD0, CInputProcessGameBase::GetEventGameActionVector(long, const
//   CInputEvent&, C2DVector*) @ 0x00687D00 and IsEventButtonReleased @ 0x00687B30 (landed names), CInputEvent::
//   GetDeviceType @ 0x00A03B50 / GetKey @ 0x00A03B70, CMemoryBuffer::Copy @ 0x009F1760, CProcessedInput::AddGameEvent
//   @ 0x00A0D340, __security_check_cookie @ 0x00BFE9F9.
// Layout: engine/CInputProcessGameBase.h (MyPlayer +0x18), modelled locally as in the landed 0x00690C70; CGameEvent
//   (Ego_r.pdb, 40 bytes) as there; CThing UniqueID (ulong64) +0x18 and the Dead bit at +0x91 bit 0 (retail offsets).
// Shape: the event is built before the creature test; the inline Add* helpers home their by-value argument (the
//   floats copied through integer registers) in the dead `event` slot, the 64-bit id in a frame pair.
class CInputEvent {
public:
    long GetDeviceType() const;                    // 0x00A03B50
    long GetKey() const;                           // 0x00A03B70
};
struct C2DVector { float X, Y; };
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddUID(unsigned __int64 value) { Copy(sizeof(value), &value); }
    void AddFloat(float value) { Copy(sizeof(value), &value); }
    void AddLong(long value) { Copy(sizeof(value), &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
};
struct CThing {
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                     // +0x18
    unsigned char    _pad20[0x91 - 0x20];
    unsigned char    Dead : 1;                     // +0x91 bit 0
};
class CPlayer {
public:
    CThing* GetControlledCreature() const;         // 0x00487DD0
};
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonReleased(const CInputEvent& event);            // 0x00687B30
    bool GetEventGameActionVector(long action, const CInputEvent& event, C2DVector* vector); // 0x00687D00
};
class CInputProcessStrafe : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessStrafe::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* creature = MyPlayer->GetControlledCreature();
    C2DVector strafe;
    if (!GetEventGameActionVector(0x3c, event, &strafe))
        return;
    if (event.GetDeviceType() == 2 && !IsEventButtonReleased(event))
        return;
    CGameEvent gameEvent(0x2b);
    if (creature == 0 || creature->Dead)
        return;
    long key = -1;
    if (event.GetDeviceType() == 2)
        key = event.GetKey();
    gameEvent.AddUID(creature->UniqueID);
    gameEvent.AddFloat(strafe.X);
    gameEvent.AddFloat(strafe.Y);
    gameEvent.AddLong(key);
    processed.AddGameEvent(gameEvent);
}