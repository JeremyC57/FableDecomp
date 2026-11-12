// dynamic initializer for CCharString CSubtitledMessage::MostRecentGroupQueried @ 0x013B8A50 -- default-constructs it out of line, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A610, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13b8a50 ; call 0x99e4b0 ; push 0x1228dc0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B8A50: it default-constructs it out of line and registers the
// destructor thunk @ 0x01228DC0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 36 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0
class CSubtitledMessage {                                   // no shared header: only the static this row defines
private:
    static CCharString MostRecentGroupQueried;             // 0x013B8A50
};

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'CSubtitledMessage::MostRecentGroupQueried'' (0x0100C9C0) +
//   FableWin.pdb (0x03FDF450); both PDBs run the CConversationAttitude statics (retail 0x0121A460 .. 0x0121A580),
//   CSubtitledMessage::MostRecentGroupQueried, AttitudeSymbols (retail 0x0121A630); default CCharString. Anniv360 Test PDB
//   (third source): ?MostRecentGroupQueried@CSubtitledMessage@@0VCCharString@@A. Was g_CCharString_013b8a50.
CCharString CSubtitledMessage::MostRecentGroupQueried;    // 0x013B8A50 ?MostRecentGroupQueried@CSubtitledMessage@@0VCCharString@@A
