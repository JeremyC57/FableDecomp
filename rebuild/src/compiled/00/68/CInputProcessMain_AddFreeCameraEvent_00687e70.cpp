// CInputProcessMain::AddFreeCameraEvent(unsigned char on, CProcessedInput& input) -- queue game event 0x0C (the free-camera toggle) carrying the one-byte on/off flag
// Leaf: ?AddFreeCameraEvent@CInputProcessMain@@
// Flags: /GS
//
// Retail 0x00687E70, 89 bytes (+ int3 padding to 96), plain `ret` (private static __fastcall: the flag in cl, the input in
//   edx); /GS frame (cookie 0x0139C8A8 above the 40-byte event, __security_check_cookie 0x00BFE9F9).
// Callees: CMemoryBuffer::Copy(unsigned long length, const void* data) @ 0x009F1760 (the inline AddUBYTE: append at
//   Data + EndPos, then EndPos += length), CProcessedInput::AddGameEvent(const CGameEvent&) @ 0x00A0D340.
// Identity: byte twin of CInputProcessMain::AddManualCameraEvent 0x00687E10 (Ego_r.pdb names only that one,
//   ?AddManualCameraEvent@CInputProcessMain@@CIXEAAVCProcessedInput@@@Z) with game event 0x0C; callers 0x00687F1E /
//   0x00687F41 in the landed CInputProcessMain::HandleDebugCameraAction 0x00687ED0, which sends it off when the player
//   is in mode 1 with UsingFreeCamera set and on in the default arm -- the name is descriptive (the TLC build's own
//   name for this second helper is not in either PDB name set).
// Layout: engine/CGameEvent.h (Ego_r.pdb, 40 bytes: Type +0, Player +4, Data[32] +5, EndPos +0x25, Valid +0x26,
//   Replacement +0x27) with a methods-only builder view for the inline constructor and AddUBYTE.
// Shape: `CGameEvent event(0x0C); event.AddUBYTE(on); input.AddGameEvent(event);` -- the constructor's stores (Player -1,
//   EndPos 0, Valid true, Replacement 0) scheduled around the Copy arguments; AddUBYTE homes its by-value byte in the
//   frame word below the event.
#include "engine/CGameEvent.h"

struct CGameEventBuilder : CGameEvent {                     // CGameEvent's inline members
    CGameEventBuilder(long type)
    {
        Type = type;
        Player = -1;
        EndPos = 0;
        Valid = true;
        Replacement = false;
    }
    void Copy(unsigned long length, const void* data);      // 0x009F1760 (CMemoryBuffer::Copy)
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};

class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);             // 0x00A0D340
};

class CInputProcessMain {
public:
    static void __fastcall AddFreeCameraEvent(unsigned char on, CProcessedInput& input);
};

void __fastcall CInputProcessMain::AddFreeCameraEvent(unsigned char on, CProcessedInput& input)
{
    CGameEventBuilder event(0x0C);
    event.AddUBYTE(on);
    input.AddGameEvent(event);
}