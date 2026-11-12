// dynamic initializer for CWideString GForceStartingHolySite @ 0x013B866C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01219FE0, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x122e8c0 ; mov ecx, 0x13b866c ; call 0x99b6b0 ; push 0x1228980 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CWideString
// living at 0x013B866C: it default-constructs it out of line and registers the
// destructor thunk @ 0x01228980 (`mov ecx,offset object ; jmp CWideString::~CWideString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 12 bodies of this shape.
#include "fable_string.h"      // CWideString: CWideString(const wchar_t*) @ 0x0099B6B0 (landed), ~CWideString

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'GForceStartingHolySite'' (0x0100C3D0) + FableWin.pdb (0x03FDE290);
//   Ego_r order GInputFileName, GEventPackageFileName, GPacketFileName, GForceLevel, GForceStartingHolySite,
//   GDummyObjectFilter, GRunLUAScript, GForcedStaticMapUpdateList = retail 0x01219F60 .. 0x0121A040 (FableWin adds
//   GEngineMemoryInit / GOverridePlayerStartPos, which have no retail initializer); every retail ctor kind matches the Anniv
//   type. Anniv360 Test PDB (third source): ?GForceStartingHolySite@@3VCWideString@@A. Was g_CWideString_013b866c.
CWideString GForceStartingHolySite(L"MAIN_START_POSITION");        // 0x013B866C ?GForceStartingHolySite@@3VCWideString@@A
