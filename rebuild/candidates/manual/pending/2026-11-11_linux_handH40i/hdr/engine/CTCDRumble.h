#ifndef FABLE_ENGINE_CTCDRUMBLE_H
#define FABLE_ENGINE_CTCDRUMBLE_H
// New header (chunk H40i, row 0x0078FD60 CTCDivineWrath::FrameUpdate): the static rumble factories of CTCDRumble, Ego_r.pdb
// ?Create@CTCDRumble@@SIPAVCThing@@ABVC3DVector@@W4EQuakeStrength@@W4EQuakeLength@@_N@Z (Ego_r 0x007EB500; retail 0x007F1B70,
// the landed row global_Create_007f1b70 whose callers bind it as "CreateCameraShake") and the float overload
// ?Create@CTCDRumble@@SIPAVCThing@@ABVC3DVector@@MM_N@Z (Ego_r 0x007EB2C0). Declarations only.
#include "engine/NCreatureActionEvents.h"
class CThing;class C3DVector;
class CTCDRumble {
public:
    static CThing* __fastcall Create(const C3DVector& pos, EQuakeStrength strength, EQuakeLength length, bool p3);   // 0x007F1B70
    static CThing* __fastcall Create(const C3DVector& pos, float strength, float length, bool p3);
};
#endif
