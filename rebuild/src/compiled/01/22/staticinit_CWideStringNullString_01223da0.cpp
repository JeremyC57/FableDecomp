// dynamic initializer for CWideString CWideString::NullString @ 0x013BCA24 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223DA0, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x122d70c ; mov ecx, 0x13bca24 ; call 0x99b6b0 ; push 0x122a550 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CWideString
// living at 0x013BCA24: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122A550 (the destructor is inlined into the thunk, so the object's address and type are not recoverable here)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 12 bodies of this shape.
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING   // fable_string.h: public static CWideString::NullString
#include "fable_string.h"      // CWideString: CWideString(const wchar_t*) @ 0x0099B6B0 (landed), ~CWideString

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'CWideString::NullString'' (0x0101AB90) + FableWin.pdb (0x0400A520);
//   both PDBs name `dynamic initializer for 'CWideString::NullString'' = retail this (L""), declared by fable_string.h at
//   0x013BCA24. Anniv360 Test PDB (third source): ?NullString@CWideString@@2V1@A. Was g_CWideString_013bca24.
CWideString CWideString::NullString(L"");    // 0x013BCA24 ?NullString@CWideString@@2V1@A
