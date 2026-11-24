// Leaf: ?AddToInterface@CAInputProcess@@UAE_NAAVCInputProcessManager@@@Z
// Header route (NF325, 2026-11-24): defined on the real class of engine/CAInputProcess.h (FABLE_RETAIL_INPUTPROCESS + the landed
//   FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE declaration, slot 1); was ?AddToInterface@CAInputProcess_Methods@@UAE_NPAVCInputProcessManager@@@Z
//   on a local alias with made-up slots v0 / v1 / OnAdded. Callees: ?AddInputProcess@CInputProcessManager@@IAEXAAVCAInputProcess@@@Z
//   (0x00A0D4F0, both PDBs, protected: the add-only friend opt-in FABLE_RETAIL_CINPUTPROCESSMANAGER_FRIEND_CAINPUTPROCESS; was the
//   stand-in ?Register@CInputProcessManager@@QAEXPAVCAInputProcess@@@Z) and the virtual slot 2 Init (0x00486360, RTTI + FableWin.pdb
//   ?Init@CAInputProcess@@UAEXXZ; was OnAdded), declared by the add-only FABLE_RETAIL_CAINPUTPROCESS_INIT.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_CINPUTPROCESSMANAGER_FRIEND_CAINPUTPROCESS
#include "engine/CAInputProcess.h"
class CEnginePrimitivePrepareInfo;   // engine/CInputProcessManager.h names it undeclared (PreparePrimitivesForEngine); declared before the include
#include "engine/CInputProcessManager.h"

bool CAInputProcess::AddToInterface(CInputProcessManager& manager) {
    PPlayerInterface = &manager;
    manager.AddInputProcess(*this);
    Init();
    return true;
}
