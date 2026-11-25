// CGamePlayerInterface::GetMouseMovement(CInputEvent*)
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004458F0, oracle length 40 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov ecx,[esp+8] ; sub esp,0Ch ; lea eax,[esp] ; push eax ; call <event movement> ; fld [esp] ; mov eax,[esp+10h] ; fstp [eax] ; fld [esp+4] ; fchs ; fstp [eax+4] ; add esp,0Ch ; ret 8
// `this` is unused. The input event's movement comes back as a 12-byte vector (0x00A043D0,
// __fastcall on the event, hidden result pointer) and is turned into screen-space mouse movement
// by negating Y. C2DVector is returned through the caller's hidden pointer (first stack slot; the
// event pointer is the second).

#include "engine/C3DVector.h"

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};

struct CInputEventCalls {
};

struct CGamePlayerInterface_Methods {
    C2DVector GetMouseMovement(CInputEventCalls* event);
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00A043D0 ?GetMouseMovement@CInputEvent@@QBE?AVC3DVector@@XZ (was ?GetMovement@CInputEventCalls@@QAE?AVC3DVector@@XZ)
class CInputEvent { public: C3DVector GetMouseMovement() const; };
C2DVector CGamePlayerInterface_Methods::GetMouseMovement(CInputEventCalls* event)
{
    C3DVector movement = reinterpret_cast<const CInputEvent*>(event)->GetMouseMovement();
    return C2DVector(movement.X, -movement.Y);
}