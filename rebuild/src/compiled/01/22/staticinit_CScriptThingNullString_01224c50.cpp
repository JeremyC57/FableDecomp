// dynamic initializer for CCharString CScriptThing::NullString @ 0x0143E8EC -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224C50, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x122d70e ; mov ecx, 0x143e8ec ; call 0x99ebf0 ; push 0x122ae60 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x0143E8EC: it constructs it from the literal and registers the
// destructor thunk @ 0x0122AE60 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 46 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"   // CScriptThing::PImp
#include "engine/CScriptThing.h"   // static CCharString NullString (0x0143E8EC)

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'CScriptThing::NullString'' (0x0101B230) + FableWin.pdb (0x0400C310);
//   both PDBs run CScriptDefinitionManager::PClass, CScriptThing::NullString (0x1D bytes in Ego_r), ScriptActiveStatuses =
//   retail 0x01224C40, this (0x1D bytes, ""), 0x01224C6D; declared by engine/CScriptThing.h at 0x0143E8EC. Anniv360 Test PDB
//   (third source): ?NullString@CScriptThing@@1VCCharString@@A. Was g_CCharString_0143e8ec.
CCharString CScriptThing::NullString("", -1);    // 0x0143E8EC ?NullString@CScriptThing@@2VCCharString@@A (engine/CScriptThing.h default access, the spelling landed callers bind)
