// dynamic initializer for CCharString CLevelLoader::LoadAllSectionName @ 0x013BAB08 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121E140, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x125d9f0 ; mov ecx, 0x13bab08 ; call 0x99ebf0 ; push 0x12293e0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAB08: it constructs it from the literal and registers the
// destructor thunk @ 0x012293E0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CLEVELLOADER_LOADALL
#include "engine/CLevelLoader.h"   // public static LoadAllSectionName

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'CLevelLoader::LoadAllSectionName'' (0x01010C00) + FableWin.pdb
//   (0x03FF3060); both PDBs run CLevelLoader::LoadAllSectionName, CLevelLoader::GlobalSectionName = retail 0x0121E140
//   ("XXX_ALL"), 0x0121E160 ("NULL"). Anniv360 Test PDB (third source): ?LoadAllSectionName@CLevelLoader@@2VCCharString@@A. Was
//   g_CCharString_013bab08.
CCharString CLevelLoader::LoadAllSectionName("XXX_ALL", -1);    // 0x013BAB08 ?LoadAllSectionName@CLevelLoader@@2VCCharString@@A
