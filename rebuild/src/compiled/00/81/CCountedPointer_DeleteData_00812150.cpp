// CCountedPointer<CHitParameters>::DeleteData(void* data) -- the counted pointer's per-type deleter: `delete static_cast<CHitParameters*>(data)`,
//   null-guarded; CHitParameters has a non-virtual destructor, so the delete expands to ~CHitParameters 0x00789358 + operator delete 0x00BFE9BC
// Leaf: ?DeleteData@?$CCountedPointer@VCHitParameters@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00812150, 23 bytes: `push esi; mov esi,ecx; test esi,esi; je; call 0x00789358; push esi; call delete; ...; ret`
//   (a static __fastcall member, data in ecx); its address is stored as the count block's DeleteFunc by Reset 0x00812280 (`mov [eax+4], 0x00812150`).
// Names (NF86, q11099): Ego_r.pdb 0x00807D90 ?DeleteData@?$CCountedPointer@VCHitParameters@@@@SIXPAX@Z then ?Reset@..VCHitParameters.. 0x00807DB0,
//   FableWin.pdb 0x025AAB20 (same name); retail keeps the same TU order (DeleteData 0x00812150, CHitParameters copy ctor 0x00812170,
//   Reset 0x00812280), and the landed CEventHitBase ctor 0x00811760 already binds 0x00812280 as ?Reset@?$CCountedPointer@VCHitParameters@@@@.
//   Was the stand-in _global::DeleteObject_00812150 (?OnDie@@YIXPAVCTCMapwho@@@Z, the dtor bound as @Helper_die@4).
// Shape: engine/CCountedPointer.h DeleteData on a non-polymorphic T with the declared CHitParameters destructor (FABLE_RETAIL_CHITPARAMETERS_CTOR);
//   the explicit instantiation emits the COMDAT.
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

template void FABLE_FASTCALL CCountedPointer<CHitParameters>::DeleteData(void*);
