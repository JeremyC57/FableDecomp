// CProcessedInput::Clear() -- reset the processed input: no type, no player, no game events, default priority; every event slot uninitialised
// Grade: DIFFER(43v48) -- under optimize("s") the loop stays rolled as in retail but the encodings are /Os ones (`or dword [ecx],-1`, `push 4 ; pop ecx`) where retail has `mov dword [ecx],-1` / `mov ecx,4`; plain /O2 unrolls the four slots (66 bytes)
// Leaf: ?Clear@CProcessedInput@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0D300, 48 bytes. Name: Ego_r/FableWin PDB ?Clear@CProcessedInput@@QAEXXZ, the
//   binding of its caller 0x00446220; the Ego_r donor order Clear (0x00B74BF0) .. AddGameEvent (0x00B74C50) matches the
//   retail pair 0x00A0D300 / landed AddGameEvent 0x00A0D340. Layout = Ego_r.pdb CProcessedInput (0xB0 bytes): Player +0,
//   Type +4, GameEvents[4] (CGameEvent 0x28) +8, NoGameEvents +0xA8, Priority +0xAC. The catalog label
//   "CGameEventPackage::CGameEventPackage" is propagated (that class holds 40 events, and a ctor would return this).
// Shape: CGameEvent::Uninitialise inlined over the four slots (`if (Valid) Valid = false;` -- the compare-then-store on
//   the byte at +0x26 of each 0x28-byte slot), the counted loop kept rolled as a down counter: a size-optimised TU
//   (`#pragma optimize("s", on)`: no unrolling, the zero in edx).
#pragma optimize("s", on)
enum EProcessedEventType { EPET_NONE };
enum EGameEventPriority { EGEP_LOW, EGEP_MEDIUM, EGEP_NORMAL };

class CGameEvent {
public:
    long          Type;          // +0x00
    signed char   Player;        // +0x04
    unsigned char Data[32];      // +0x05
    unsigned char EndPos;        // +0x25
    bool          Valid;         // +0x26
    bool          Replacement;   // +0x27
    void Uninitialise()
    {
        if (Valid)
            Valid = false;
    }
};

class CProcessedInput {
public:
    long                Player;          // +0x00
    EProcessedEventType Type;            // +0x04
    CGameEvent          GameEvents[4];   // +0x08
    unsigned char       NoGameEvents;    // +0xa8
    EGameEventPriority  Priority;        // +0xac
    void Clear();
};

void CProcessedInput::Clear()
{
    Type = EPET_NONE;
    Player = -1;
    NoGameEvents = 0;
    Priority = EGEP_NORMAL;
    for (long i = 0; i < 4; ++i)
        GameEvents[i].Uninitialise();
}
