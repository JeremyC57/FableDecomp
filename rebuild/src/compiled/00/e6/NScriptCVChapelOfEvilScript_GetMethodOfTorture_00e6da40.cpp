// CV_ChapelOfEvilScript::GetMethodOfTorture() -- pick the next torture: the first seven in order (TortureCount), afterwards a random one of the seven
// Retail: Fable.exe @ 0x00E6DA40, 63 bytes (+ int3 padding). Called once (0x00E6C52C).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (the terminate check before each action), rand @ 0x00BFEB16.
// Layout: Ego_r.pdb CV_ChapelOfEvilScript - 0x14: TortureCount +0x70, TortureType +0x8c (ChapelOfEvilTortureType).
// Shape: if/else with the terminate check first in each arm; `TortureType = TortureCount++` stores the old value and
// then the incremented register; `rand() % 7` is a real `idiv` by 7 (signed remainder).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" int __cdecl rand();                           // 0x00BFEB16

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04..+0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* Game;                          // +0x40
    void* Owner;                                         // +0x44
};

namespace NScript {
class CV_ChapelOfEvilScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x70 - 0x48];
    long TortureCount;                                   // +0x70
    unsigned char _pad74[0x8c - 0x74];
    long TortureType;                                    // +0x8c
    void GetMethodOfTorture();
};
} // namespace NScript
using NScript::CV_ChapelOfEvilScript;

void NScript::CV_ChapelOfEvilScript::GetMethodOfTorture()
{
    if (TortureCount < 7) {
        if (IsActiveThreadTerminating())
            return;
        TortureType = TortureCount++;
    } else {
        if (IsActiveThreadTerminating())
            return;
        TortureType = rand() % 7;
    }
}