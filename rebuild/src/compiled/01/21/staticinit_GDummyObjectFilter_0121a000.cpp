// dynamic initializer for CCharString GDummyObjectFilter @ 0x013B8670 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A000, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x122d70e ; mov ecx, 0x13b8670 ; call 0x99ebf0 ; push 0x1228990 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B8670: it constructs it from the literal and registers the
// destructor thunk @ 0x01228990 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'GDummyObjectFilter'' (0x0100C3F0) + FableWin.pdb (0x03FDE2E0); Ego_r
//   order GInputFileName, GEventPackageFileName, GPacketFileName, GForceLevel, GForceStartingHolySite, GDummyObjectFilter,
//   GRunLUAScript, GForcedStaticMapUpdateList = retail 0x01219F60 .. 0x0121A040 (FableWin adds GEngineMemoryInit /
//   GOverridePlayerStartPos, which have no retail initializer); every retail ctor kind matches the Anniv type. Anniv360 Test
//   PDB (third source): ?GDummyObjectFilter@@3VCCharString@@A. Was g_CCharString_013b8670.
CCharString GDummyObjectFilter("", -1);        // 0x013B8670 ?GDummyObjectFilter@@3VCCharString@@A
