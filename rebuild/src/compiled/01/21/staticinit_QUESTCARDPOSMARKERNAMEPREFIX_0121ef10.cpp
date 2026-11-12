// dynamic initializer for CCharString `anonymous namespace'::QUEST_CARD_POS_MARKER_NAME_PREFIX @ 0x013BADAC -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121EF10, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x126b004 ; mov ecx, 0x13badac ; call 0x99ebf0 ; push 0x1229b10 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BADAC: it constructs it from the literal and registers the
// destructor thunk @ 0x01229B10 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::QUEST_CARD_POS_MARKER_NAME_PREFIX''
//   (0x010118B0) + FableWin.pdb (0x03FF4300); Ego_r runs REACTION_MANAGER_DEF_INSTANTIATION_NAME,
//   QUEST_CARD_POS_MARKER_NAME_PREFIX, DoorTypeSymbolMap = retail 0x0121EEF0, this ("HeroCentreQuestCardPos"), 0x0121EF30
//   (CSymbolMap). Anniv360 Test PDB (third source): S_LDATA32 CCharString QUEST_CARD_POS_MARKER_NAME_PREFIX (internal linkage).
//   Was g_CCharString_013badac.
namespace {
CCharString QUEST_CARD_POS_MARKER_NAME_PREFIX("HeroCentreQuestCardPos", -1);    // 0x013BADAC
}
