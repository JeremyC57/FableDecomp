#ifndef FABLE_ENGINE_CJOYSTICKPOSITIONTRACKER_H
#define FABLE_ENGINE_CJOYSTICKPOSITIONTRACKER_H
// Hand-written (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): the stick-gesture history CInputProcessCombat::PositionTracker.
// Ego_r.pdb: one member, RecentPositions, a deque<C2DVector> (STLport 4.6 deque: 0x28 bytes in retail, so CInputProcessCombat's
// next member sits at +0x50). The constructor is out of line: retail 0x00A0E0A0 (`lea ecx,[edi+28h] ; call 0x00A0E0A0` in
// InitInterfaces), Ego_r.pdb ??0CJoystickPositionTracker@@QAE@XZ (0x00B78760) and FableWin.pdb; ??1 is FableWin-only (inline in Ego_r).
// The deque is not modelled (no STLport deque in stlport/fable_containers.h yet): opaque bytes carry its size.
#include "rebuild_abi.h"
class CJoystickPositionTracker {
public:
    CJoystickPositionTracker();                                          // 0x00A0E0A0
    unsigned char RecentPositions[0x28];                                 // +0x00 deque<C2DVector> (STLport 4.6)
};
FABLE_STATIC_ASSERT(sizeof(CJoystickPositionTracker) == 0x28);
#endif // FABLE_ENGINE_CJOYSTICKPOSITIONTRACKER_H
