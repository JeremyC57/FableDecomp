// CRenderManagerCore::Uninit()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0A9D0, oracle length 28 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; lea esi,[ecx+3A4Ch] ; mov ecx,esi ; call <IsInitialised> ; test al,al ; je R ; mov ecx,esi ; pop esi ; jmp <Uninitialise> ; R: pop esi ; ret
// engine/CRenderManagerCore.h (PDB): +0x3A4C CSystemFont SystemFont (a CInitBaseClass). Tears the
// system font down if it was initialised: 0x0099A330 is the CInitBaseClass initialised query and
// 0x00A6C120 its Uninitialise (manifest: CInitBaseClass::Uninitialise), reached by tail jump.
#include "engine/CRenderManagerCore.h"

// Call surface of the CInitBaseClass subobject (the engine header holds only the bytes).
struct CInitBaseClassCalls {
    bool IsInitialised();      // 0x0099A330
    void Uninitialise();       // 0x00A6C120
};

struct CRenderManagerCore_Methods : CRenderManagerCore {
    void Uninit();
};

void CRenderManagerCore_Methods::Uninit()
{
    CInitBaseClassCalls* font = reinterpret_cast<CInitBaseClassCalls*>(&this->SystemFont[0]);
    if (font->IsInitialised())
        font->Uninitialise();
}
