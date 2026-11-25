// CTCTalk::IsTalking() -- true while the current speech sample is still inside its frame window
// Leaf: ?IsTalking@CTCTalk@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E28A0, length 33 bytes.
// Callee: the world's frame-counter getter @ 0x00416225 (`mov eax,[ecx+48h] ; ret`, called out of line on the
// global world pointer @ 0x013B86A0; its manifest label "DrawGetEnvironment" is a propagated name).
// Ego_r.pdb CTCTalk: SampleStartedFrame +0x10, SampleDurationFrames +0x14 (both long). The test is the signed
// `frame - started < duration`; the `xor ecx,ecx ; setl cl ; mov al,cl` result is the plain `return cond;` bool
// form (GOTCHAS 2026-10-13: a bool written `return cond;` goes through a full register).
class CWorld { public:
};
extern CWorld* g_pWorld;                     // 0x013B86A0

// NF59 (round 7, queue q10519): the talk component is engine/CTCTalk.h (SampleStartedFrame +0x10 / SampleDurationFrames +0x14; it declares
//   the const ?IsTalking@CTCTalk@@QBE_NXZ, Ego_r.pdb 0x0070D6C0); the local CTCTalk model declared it non-const (QAE).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCTalk.h"

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00416225 ?GetCurrentServerFrame@CMainGameComponent@@QBEJXZ (was ?GetFrame@CWorld@@QAEJXZ)
class CMainGameComponent { public: long GetCurrentServerFrame() const; };
bool CTCTalk::IsTalking() const
{
    return reinterpret_cast<const CMainGameComponent*>(g_pWorld)->GetCurrentServerFrame() - SampleStartedFrame < SampleDurationFrames;
}