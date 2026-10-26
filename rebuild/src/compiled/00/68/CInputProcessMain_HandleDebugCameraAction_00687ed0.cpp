// CInputProcessMain::HandleDebugCameraAction(CProcessedInput& input) -- turn the debug-camera button into manual-camera game events according to the player's mode: mode 19 adds the first event (off), mode 1 with the free camera on adds the second (off), modes 4..13 and 23 do nothing, every other mode adds both (on); each handled case sets the input priority to 5
// Leaf: HandleDebugCameraAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x00687ED0, 131 bytes + the jump table (4 targets) and its 23-entry byte index
//   table, `ret 4`.
// Callees: CPlayer::GetCurrentMode @ 0x00633BE0, the file-static `__fastcall` game-event helpers @ 0x00687E10 (game
//   event 0x0b) and @ 0x00687E70 (game event 0x0c), both labelled AddManualCameraEvent (bool in cl, the processed input
//   in edx), CProcessedInput::SetPriority @ 0x00A0D330 (Priority +0xac).
// Layout: Ego_r.pdb CInputProcessGameBase: World +0x14, MyPlayer +0x18 (the engine header CInputProcessMain.h flattens
//   the whole base into a blob, so the class is modelled locally); CPlayer::UsingFreeCamera +0x220 (retail).
// Shape: a dense switch on the mode (the byte-indexed four-target jump table); case 1 is an if/else whose else arm
//   repeats the default body (VC cross-jumps it into the default after the input load, hoisting that load above the
//   free-camera test; a fall-through into `default:` does not); every handled case sets the priority itself; the
//   do-nothing cases just break.
class CProcessedInput { public:
    void SetPriority(long priority);               // 0x00A0D330
};
void __fastcall AddManualCameraEvent_0B(bool on, CProcessedInput& input);   // 0x00687E10
void __fastcall AddManualCameraEvent_0C(bool on, CProcessedInput& input);   // 0x00687E70

class CPlayer { public:
    unsigned char _pad000[0x220];
    bool          UsingFreeCamera;                 // +0x220
    long GetCurrentMode();                         // 0x00633BE0
};
class CInputProcessMain { public:
    virtual void v00();
    void*    PPlayerInterface;                     // +0x04 (CAInputProcess)
    unsigned char InputProcessListPos[0xc];        // +0x08
    void*    World;                                // +0x14 (CInputProcessGameBase)
    CPlayer* MyPlayer;                             // +0x18
    void HandleDebugCameraAction(CProcessedInput& input);
};

void CInputProcessMain::HandleDebugCameraAction(CProcessedInput& input)
{
    switch (MyPlayer->GetCurrentMode()) {
    case 19:
        AddManualCameraEvent_0B(false, input);
        input.SetPriority(5);
        break;
    case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 23:
        break;
    case 1:
        if (MyPlayer->UsingFreeCamera) {
            AddManualCameraEvent_0C(false, input);
            input.SetPriority(5);
        } else {
            AddManualCameraEvent_0B(true, input);
            AddManualCameraEvent_0C(true, input);
            input.SetPriority(5);
        }
        break;
    default:
        AddManualCameraEvent_0B(true, input);
        AddManualCameraEvent_0C(true, input);
        input.SetPriority(5);
        break;
    }
}