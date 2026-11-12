// dynamic initializer for CWideString GResetProgressTipText @ 0x013B89D8 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A3E0, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x122d70c ; mov ecx, 0x13b89d8 ; call 0x99b6b0 ; push 0x1228c40 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CWideString
// living at 0x013B89D8: it default-constructs it out of line and registers the
// destructor thunk @ 0x01228C40 (`mov ecx,offset object ; jmp CWideString::~CWideString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 12 bodies of this shape.
#include "fable_string.h"      // CWideString: CWideString(const wchar_t*) @ 0x0099B6B0 (landed), ~CWideString

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'GResetProgressTipText'' (0x0100C7F0) + FableWin.pdb (0x03FDF210);
//   Ego_r order REACTION_MANAGER_DEF_INSTANTIATION_NAME, GResetProgressTipText, GResetProgressTipTitleText,
//   GOldAutoSaveProgressDisplay = retail 0x0121A3C0 .. 0x0121A420; landed 0x00498490 already binds this spelling here. Anniv360
//   Test PDB (third source): ?GResetProgressTipText@@3VCWideString@@A. Was g_CWideString_013b89d8.
CWideString GResetProgressTipText(L"");        // 0x013B89D8 ?GResetProgressTipText@@3VCWideString@@A
