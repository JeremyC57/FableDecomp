// _global::$E1() -- dynamic initializer of the CCharString array NScript::CQ_SunnyvaleMasterData::RelationSuffixes[3] @ 0x0143E934: default-constructs the three strings and registers the array's exit cleanup
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224CE0, 45 bytes (+ padding), plain `ret`, reached from the .CRT$XCU table; catalog
//   "sub_01224ce0" was a gap-scan placeholder.
// Identity: the object 0x0143E934 is bound as ?RelationSuffixes@CQ_SunnyvaleMasterData@NScript@@2PAVCCharString@@A (reloc_bindings.tsv;
//   the landed CQ_SunnyvaleMasterData::Init 0x00CDBA10 assigns "", "_LIKE", "_HATE" to its three elements, the opt-in
//   FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_RELATION_STATICS of engine/CQ_SunnyvaleMasterData.h declares it). The member name is
//   DESCRIPTIVE (chunk 780): neither PDB name set lists this static. The callee 0x0099E4B0 is CCharString::CCharString() (bound
//   ??0CCharString@@QAE@XZ) and the pushed function 0x0122AEF0 is the array's `dynamic atexit destructor' (a 3-step reverse loop
//   calling ~CCharString 0x0099EAE0; catalog "CFontBankStateBlock::CreateCRC" there is a wrong propagated label).
// Shape: VC7.1 expands the array construction in place (no `vector constructor iterator`): a down-counted 3-step loop over the
//   4-byte elements (esi = element, edi = count), then `push _$E2 ; call _atexit ; add esp,4`.
#define FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_RELATION_STATICS
#include "engine/CQ_SunnyvaleMasterData.h"

CCharString NScript::CQ_SunnyvaleMasterData::RelationSuffixes[3];
