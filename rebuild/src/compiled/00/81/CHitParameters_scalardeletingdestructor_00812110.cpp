// CHitParameters::`scalar deleting destructor'(unsigned int flags) -- ~CHitParameters 0x00789358, then operator delete on flag bit 0; returns this
// Leaf: ??_GCHitParameters@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00812110, 30 bytes, thiscall `ret 4`: `push esi; mov esi,ecx; call 0x00789358; test byte ptr [esp+8],1; je;
//   push esi; call 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4`. No caller and no address reference in retail (an unreferenced COMDAT the
//   link kept, like its neighbour 0x00812130).
// Names (NF86, q11099): FableWin.pdb 0x025AAC90 ??_GCHitParameters@@QAEPAXI@Z (QAE: CHitParameters' destructor is not virtual, Ego_r/FableWin
//   ??1CHitParameters@@QAE@XZ), emitted next to DeleteData<CHitParameters> 0x025AAB20; the body is exactly the scalar deleting destructor over
//   the proven ~CHitParameters 0x00789358. Was the stand-in _global::ScalarDeletingDtor_00812110
//   (?CLandscapeBackgroundPatch_vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z, the dtor bound as a CLandscapeBackgroundPatch stand-in).
// Shape: VC7.1 generates this COMDAT for the delete expression in CCountedPointer<CHitParameters>::DeleteData (0x00812150, same TU in retail:
//   0x00812110 .. 0x00812280), so the row is that TU: the explicit DeleteData instantiation emits ??_GCHitParameters@@QAEPAXI@Z beside it.
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
