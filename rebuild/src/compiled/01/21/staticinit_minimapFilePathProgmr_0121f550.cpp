// dynamic initializer for static CWideString minimapFilePathProgmr @ 0x013BAF3C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F550, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x12725c4 ; mov ecx, 0x13baf3c ; call 0x99b6b0 ; push 0x1229fa0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CWideString
// living at 0x013BAF3C: it default-constructs it out of line and registers the
// destructor thunk @ 0x01229FA0 (`mov ecx,offset object ; jmp CWideString::~CWideString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 12 bodies of this shape.
#include "fable_string.h"      // CWideString: CWideString(const wchar_t*) @ 0x0099B6B0 (landed), ~CWideString

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'minimapFilePathProgmr'' (0x01011650) + FableWin.pdb (0x03FF7210);
//   both PDBs name minimapFilePathArtist, minimapFilePathProgmr (no namespace) = retail 0x0121F530 / 0x0121F550, the Artist /
//   Progmr map paths; Anniv360 Test PDB (third source) has them as S_LDATA32 CWideString (internal linkage: file statics). Was
//   g_CWideString_013baf3c.
static CWideString minimapFilePathProgmr(L"Data\\Levels\\Maps\\Progmr\\");    // 0x013BAF3C (internal linkage)
