// CPlayer::UnInitActiveMode(EPlayerMode mode) -- run the current player mode's uninit handler from the static mode table with a fresh CProcessedInput and queue that input on the player interface when the handler produced an event
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633BF0, length 192 bytes, `ret 4`; /GS frame (cookie 0x0139C8A8,
// __security_check_cookie 0x00BFE9F9).
// Identity: Ego_r.pdb `?UnInitActiveMode@CPlayer@@AAEXW4EPlayerMode@@@Z`; the table is the static
// `CPlayer::PlayerModeInfo` (0x013B8C88, 0x20-byte CPlayerModeInfo {InitFunc +0, UninitFunc +0x10}).
// Callees: CProcessedInput::Clear 0x00A0D300 (from the inline constructor), CProcessedInput::SetPlayer(long) 0x00A0D3A0
// (catalogued "__EH_epilog3", a one-store setter of +0), CInputProcessManager::AddQueuedProcessedInput 0x00A0D460, and
// the mode's UninitFunc through its member pointer (`this` + delta at +4).
// Layout: PlayerInterface +0x0C (the CInputProcessManager side of CGamePlayerInterface), Number +0x28, PlayerModes
// +0x210 (retail list = the 4-byte sentinel pointer; engine/CPlayer.h carries the 12-byte PDB list, so the members past
// it sit 8 bytes high there and a local model is used). CProcessedInput (engine/CProcessedInput.h, 0xB0 bytes):
// Player +0, Type +4, CGameEvent[4] +8 (each 0x28: Player +4 = -1, Valid +0x26 = false), NoGameEvents +0xA8, Priority +0xAC.
// Shape: GetCurrentMode() is the inline `PlayerModes.back()` (sentinel->prev->value) read once into a local, so the
// record offset (mode * 0x20) stays in edi and both table reads fold the table base into the displacement (a
// reference to the record adds the base with `add esi,offset` instead). The CProcessedInput constructor inlines the four CGameEvent
// constructors (unrolled, `or al,0FFh` / `bl`) before Clear(). The member pointers are called through
// {function, delta} only, so the TU declares `pointers_to_members(full_generality, multiple_inheritance)` (8-byte pmf) and each 16-byte PDB
// slot keeps 8 pad bytes (a pmf of unspecified inheritance is copied whole to the frame before the call).
typedef long EPlayerMode;
typedef long EProcessedEventType;
typedef long EGameEventPriority;

class CGameEvent { public:                                     // 0x28 bytes
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent() : Player(-1), Valid(false) { }
};

class CProcessedInput { public:                                // 0xb0 bytes
    long                Player;                         // +0x00
    EProcessedEventType Type;                           // +0x04
    CGameEvent          GameEvents[4];                  // +0x08
    unsigned char       NoGameEvents;                   // +0xa8
    EGameEventPriority  Priority;                       // +0xac
    CProcessedInput() { Clear(); }
    void Clear();                                       // 0x00A0D300
    void SetPlayer(long player);                        // 0x00A0D3A0
};

class CInputProcessManager { public:
    void AddQueuedProcessedInput(CProcessedInput* input);   // 0x00A0D460
};
class CGamePlayerInterface : public CInputProcessManager { public: };

#pragma pointers_to_members(full_generality, multiple_inheritance)
class CPlayer;
typedef void (CPlayer::*TModeFunc)(EPlayerMode, CProcessedInput*);   // {function, this delta}
struct CPlayerModeInfo {                                // 0x20 bytes
    TModeFunc     InitFunc;                             // +0x00
    unsigned char _pad08[8];
    TModeFunc     UninitFunc;                           // +0x10
    unsigned char _pad18[8];
};

struct SModeListNode {                                  // list<EPlayerMode> node
    SModeListNode* Next;
    SModeListNode* Prev;
    EPlayerMode    Value;
};

class CPlayer { public:
    void*                 __vftable;
    unsigned char         _pad04[0x0c - 0x04];
    CGamePlayerInterface* PlayerInterface;              // +0x0c
    unsigned char         _pad10[0x28 - 0x10];
    long                  Number;                       // +0x28
    unsigned char         _pad2c[0x210 - 0x2c];
    SModeListNode*        PlayerModes;                  // +0x210 list<EPlayerMode> (sentinel)
    static CPlayerModeInfo PlayerModeInfo[];            // 0x013B8C88
    EPlayerMode GetCurrentMode() const { return PlayerModes->Prev->Value; }
    void UnInitActiveMode(EPlayerMode mode);
};

void CPlayer::UnInitActiveMode(EPlayerMode mode)
{
    EPlayerMode current = GetCurrentMode();
    if (PlayerModeInfo[current].UninitFunc) {
        CProcessedInput input;
        input.SetPlayer(Number);
        (this->*PlayerModeInfo[current].UninitFunc)(mode, &input);
        if (input.Type != 0)
            PlayerInterface->AddQueuedProcessedInput(&input);
    }
}