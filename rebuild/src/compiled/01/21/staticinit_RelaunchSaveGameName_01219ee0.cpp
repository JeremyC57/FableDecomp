// dynamic initializer for CWideString RelaunchSaveGameName @ 0x013B7D5C -- default-constructs it out of line, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01219EE0, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13b7d5c ; call 0x99aed0 ; push 0x1228910 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B7D5C: it default-constructs it out of line and registers the
// destructor thunk @ 0x01228910 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 36 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0
// Identity: the default constructor called here is 0x0099AED0, CWideString::CWideString() (labels, chunks 237/247/265/284), so the object is a CWideString.

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'RelaunchSaveGameName'' (0x0100C290) + FableWin.pdb (0x03FDDA70);
//   Ego_r order gIME, CUserProfileManager::PClass / m_strProfileName, RelaunchSaveGameName, GGame = retail 0x01219E90 ..
//   0x01219F00 (GGame landed); default CWideString. Anniv360 Test PDB (third source): ?RelaunchSaveGameName@@3VCWideString@@A.
//   Was g_CCharString_013b7d5c.
CWideString RelaunchSaveGameName;        // 0x013B7D5C ?RelaunchSaveGameName@@3VCWideString@@A
