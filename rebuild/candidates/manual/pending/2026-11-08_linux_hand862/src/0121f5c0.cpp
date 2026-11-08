// _global::$E1() -- dynamic initializer of the anonymous-namespace CEnginePrimitiveHandle hit_dir @ 0x013BAF4C: default-constructs it and registers its exit cleanup
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F5C0, 42 bytes, plain `ret`, reached from the .CRT$XCU table; catalog "sub_0121f5c0" was a
//   gap-scan placeholder.
// Identity: Ego_r.pdb 0x01011EB0 / FableWin.pdb 0x03FF7270 `anonymous namespace'::`dynamic initializer for 'hit_dir'' (both PDB name sets list it next to
//   `dynamic atexit destructor for 'hit_dir''); the object 0x013BAF4C is bound as ?hit_dir@?A0xcbcdafed@@3VCEnginePrimitiveHandle@@A
//   (reloc_bindings.tsv) and the atexit function pushed here, 0x0122A010, is the landed `dynamic atexit destructor for 'hit_dir'' (_$E2 of
//   the same one-object TU). The vptr stored is CEnginePrimitiveHandle's vtable 0x0122F598 (rtti-vtables.tsv).
// Shape: the inline default ctor calls CBaseClass::CBaseClass 0x0099A2D0 out of line, stores the CEnginePrimitiveHandle vptr and zeroes
//   the CIVCountedPointer Data (+4); VC7.1 schedules the atexit push between the base call and the member stores. This is the same TU
//   as the landed _$E2 (source text identical, leaf _$E1).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CEnginePrimitiveHandle.h"
namespace { CEnginePrimitiveHandle hit_dir; }
