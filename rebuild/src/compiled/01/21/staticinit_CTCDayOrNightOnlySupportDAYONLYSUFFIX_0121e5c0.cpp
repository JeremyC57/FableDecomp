// dynamic initializer for const CCharString CTCDayOrNightOnlySupport::DAY_ONLY_SUFFIX @ 0x013BAC18 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121E5C0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1264dac ; mov ecx, 0x13bac18 ; call 0x99ebf0 ; push 0x1229610 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAC18: it constructs it from the literal and registers the
// destructor thunk @ 0x01229610 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0
class CTCDayOrNightOnlySupport {                            // no shared header: only the two suffix statics
public:
    static const CCharString DAY_ONLY_SUFFIX;              // 0x013BAC18
    static const CCharString NIGHT_ONLY_SUFFIX;            // 0x013BAC1C
};

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'CTCDayOrNightOnlySupport::DAY_ONLY_SUFFIX'' (0x01010F60) +
//   FableWin.pdb (0x03FF46A0); both PDBs run CTCDayOrNightOnlySupport::DAY_ONLY_SUFFIX, NIGHT_ONLY_SUFFIX = retail 0x0121E5C0
//   ("DayOnly"), 0x0121E5E0 ("NightOnly"). Anniv360 Test PDB (third source):
//   ?DAY_ONLY_SUFFIX@CTCDayOrNightOnlySupport@@2VCCharString@@B. Was g_CCharString_013bac18.
const CCharString CTCDayOrNightOnlySupport::DAY_ONLY_SUFFIX("DayOnly", -1);    // 0x013BAC18 ?DAY_ONLY_SUFFIX@CTCDayOrNightOnlySupport@@2VCCharString@@B
