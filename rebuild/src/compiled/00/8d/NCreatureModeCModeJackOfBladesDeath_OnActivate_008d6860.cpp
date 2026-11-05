// CModeJackOfBladesDeath::OnActivate() -- remember the thing's position (CThing::GetPos @ 0x004C73D0, a C3DVector reference copied memberwise into +0x74) and clear the counter at +0x80
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6860, length 42 bytes.
// Callees: CThing::GetPos @ 0x004C73D0 (labelled CVertexBufferWin32::DoSizeof, propagated; thiscall returning a const C3DVector&). Vtable slot 5 of CModeJackOfBladesDeath (rtti-vtables.tsv). Shape: no base call, `this` in esi, the vector assigned one dword per register (a user memberwise assignment) with the zero store scheduled between the second and third, `ret`.
#include "engine/C3DVector.h"

class CThing {public:
    const C3DVector& GetPos() const;                // 0x004C73D0
};

struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    CThing* m_pThing;                              // +0x04
};

struct CModeJackOfBladesDeath : CModeBase {
    unsigned char _pad08[0x6c];                    // +0x08 .. +0x74
    C3DVector     m_startPosition;                 // +0x74
    long          m_counter;                       // +0x80
    virtual void OnActivate();
};

void CModeJackOfBladesDeath::OnActivate()
{
    m_startPosition = m_pThing->GetPos();
    m_counter = 0;
}