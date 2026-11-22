#ifndef FABLE_ENGINE_NCAMERAMODES_CSCRIPTMODULEOPENGATE_H
#define FABLE_ENGINE_NCAMERAMODES_CSCRIPTMODULEOPENGATE_H
#ifdef FABLE_RETAIL_NCAMERAMODES_CSCRIPTMODULEOPENGATE_PASS26
#include "engine/NCameraModes_CScriptModuleBase.h"
namespace NCameraModes {
// Both original PDBs, native RTTI 01275B0C and native constructor 008531E0.
// Ego_r layout has PGate at 54h, Pos at 5Ch and DeltaPos at 68h.
class CScriptModuleOpenGate : public CScriptModuleSelfTerminateBase {
protected:
    virtual void OnInitLeaf();
private:
    CIntelligentPointer<const CThing> PGate;
    C3DVector Pos;
    C3DVector DeltaPos;
};
}
#endif
#endif
