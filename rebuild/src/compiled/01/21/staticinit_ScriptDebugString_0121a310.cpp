// dynamic initializer for CWideString ScriptDebugString @ 0x013B877C -- default-constructs it out of line, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A310, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13b877c ; call 0x99aed0 ; push 0x1228b10 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B877C: it default-constructs it out of line and registers the
// destructor thunk @ 0x01228B10 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 36 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0
// Identity: the default constructor called here is 0x0099AED0, CWideString::CWideString() (labels, chunks 237/247/265/284), so the object is a CWideString.

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'ScriptDebugString'' (0x0100C720) + FableWin.pdb (0x03FDEF90); both
//   PDBs run CStartupDefinitionManager::PClass, REACTION_MANAGER_DEF_INSTANTIATION_NAME, ScriptDebugString = retail 0x0121A2DC,
//   0x0121A2F0, this; default CWideString. Anniv360 Test PDB (third source): ?ScriptDebugString@@3VCWideString@@A. Was
//   g_CCharString_013b877c.
CWideString ScriptDebugString;        // 0x013B877C ?ScriptDebugString@@3VCWideString@@A
