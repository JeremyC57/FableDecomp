// _global::$E1() -- dynamic initializer of the anonymous-namespace CEnginePrimitiveHandle hit_pos @ 0x013BAF44: default-constructs it and registers its exit cleanup
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F590, 42 bytes, plain `ret`, reached from the .CRT$XCU table; catalog "sub_0121f590" was a
//   gap-scan placeholder.
// Identity: Ego_r.pdb 0x01011E80 / FableWin.pdb 0x03FF7240 `anonymous namespace'::`dynamic initializer for 'hit_pos'' (both PDB name sets list it next to
//   `dynamic atexit destructor for 'hit_pos''); the object 0x013BAF44 is bound as ?hit_pos@?A0x04e16aae@@3VCEnginePrimitiveHandle@@A
//   (reloc_bindings.tsv) and the atexit function pushed here, 0x01229FC0, is the landed `dynamic atexit destructor for 'hit_pos'' (_$E2 of
//   the same one-object TU). The vptr stored is CEnginePrimitiveHandle's vtable 0x0122F598 (rtti-vtables.tsv).
// Shape: the inline default ctor calls CBaseClass::CBaseClass 0x0099A2D0 out of line, stores the CEnginePrimitiveHandle vptr and zeroes
//   the CIVCountedPointer Data (+4); VC7.1 schedules the atexit push between the base call and the member stores. This is the same TU
//   as the landed _$E2 (source text identical, leaf _$E1).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
namespace { CEnginePrimitiveHandle hit_pos; }