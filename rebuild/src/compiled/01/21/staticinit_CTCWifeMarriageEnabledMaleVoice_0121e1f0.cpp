// dynamic initializer for const CCharString CTCWife::MarriageEnabledMaleVoice @ 0x013BAB2C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121E1F0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x125f2a4 ; mov ecx, 0x13bab2c ; call 0x99ebf0 ; push 0x1229470 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAB2C: it constructs it from the literal and registers the
// destructor thunk @ 0x01229470 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCWIFE_MALEVOICE
#include "engine/CTCWife.h"   // opt-in: public static const MarriageEnabledMaleVoice (agentFAM09 hdr overlay)

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'CTCWife::MarriageEnabledMaleVoice'' (0x01010CA0) + FableWin.pdb
//   (0x03FF3180); both PDBs run REACTION_MANAGER_DEF_INSTANTIATION_NAME x2-3, CTCWife::MarriageEnabledMaleVoice,
//   CourtingAdviceGroupNames = retail 0x0121E1D0, this ("AM3"), 0x0121E210 (landed). Anniv360 Test PDB (third source):
//   ?MarriageEnabledMaleVoice@CTCWife@@2VCCharString@@B. Was g_CCharString_013bab2c.
const CCharString CTCWife::MarriageEnabledMaleVoice("AM3", -1);    // 0x013BAB2C ?MarriageEnabledMaleVoice@CTCWife@@2VCCharString@@B
