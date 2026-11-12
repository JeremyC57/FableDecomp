// dynamic initializer for CWideString CUserProfileManager::m_strProfileName @ 0x013B7D54 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01219EC0, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x122e580 ; mov ecx, 0x13b7d54 ; call 0x99b6b0 ; push 0x12288c0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CWideString
// living at 0x013B7D54: it default-constructs it out of line and registers the
// destructor thunk @ 0x012288C0 (`mov ecx,offset object ; jmp CWideString::~CWideString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 12 bodies of this shape.
#include "fable_string.h"      // CWideString: CWideString(const wchar_t*) @ 0x0099B6B0 (landed), ~CWideString
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PROFILENAME
#include "engine/CUserProfileManager.h"   // opt-in: private static m_strProfileName (agentFAM09 hdr overlay)

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'CUserProfileManager::m_strProfileName'' (0x0100C270) + FableWin.pdb
//   (0x03FDDC90); TU order CUserProfileManager::PClass -> m_strProfileName in both PDBs = retail 0x01219EB0 (atexit-only
//   PClass) -> this; literal L"Profile.bin". Anniv360 Test PDB (third source):
//   ?m_strProfileName@CUserProfileManager@@0VCWideString@@A. Was g_CWideString_013b7d54.
CWideString CUserProfileManager::m_strProfileName(L"Profile.bin");    // 0x013B7D54 ?m_strProfileName@CUserProfileManager@@0VCWideString@@A
