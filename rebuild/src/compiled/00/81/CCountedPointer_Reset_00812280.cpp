// CCountedPointer<CHitParameters>::Reset(CHitParameters* data) -- drop the shared count block (DeleteFunc on the last reference, then free it),
//   then adopt data with a fresh block {1, DeleteData 0x00812150, data}
// Leaf: ?Reset@?$CCountedPointer@VCHitParameters@@@@QAEXPAVCHitParameters@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00812280, 103 bytes, thiscall `ret 4`; operator delete 0x00BFE9BC, operator new 0x00BFEA1A, and the address
//   of DeleteData<CHitParameters> 0x00812150 stored into the block.
// Names (NF86, q11099 follow-on): Ego_r.pdb 0x00807DB0 / FableWin.pdb 0x025AA4C0 ?Reset@?$CCountedPointer@VCHitParameters@@@@QAEXPAVCHitParameters@@@Z
//   (right after DeleteData<CHitParameters> in both, as in retail); the landed CEventHitBase constructor 0x00811760 already binds 0x00812280 under
//   this spelling. Was the generated stand-in CCountedPointer_00812150::Reset (?Reset@CCountedPointer_00812150@@QAEXPAX@Z, the deleter bound as
//   ?Destroy_00812150@@YIXPAX@Z).
// Shape: the shared engine/CCountedPointer.h Reset (the 0x00668D90 / 0x00834E40 explicit-instantiation recipe).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#include "fable_compiler.h"
#include <stddef.h>
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CMaterialDef.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CHitParameters.h"
#include "engine/CCountedPointer.h"

template void CCountedPointer<CHitParameters>::Reset(CHitParameters*);
