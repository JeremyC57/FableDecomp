// CThing::`dynamic atexit destructor for 'TCsToBeRemoved'' -- the inline ~list: clear the nodes (0x004CC370, out of line) and free the sentinel
// Retail: Fable.exe (TLC, VC7.1) @ 0x01228CE0, 27 bytes.
// Leaf: _$E2
// Identity (NF92, q11218): the static is CThing::TCsToBeRemoved, Ego_d.pdb PUB32
//   ?TCsToBeRemoved@CThing@@0V?$list@U?$pair@V?$CIntelligentPointer@VCThing@@@@PAVCTCBase@@@std@@V?$allocator@U?$pair@V?$CIntelligentPointer@VCThing@@@@PAVCTCBase@@@std@@@2@@std@@A
//   (private static list of pair<CIntelligentPointer<CThing>, CTCBase*>: the 0x14-byte node = next, prev, 8-byte CIntelligentPointer, CTCBase*),
//   used by CThing::AddTC 0x004C9D60 / RemoveTC 0x004C9840 / WorldUpdate 0x004C8050 / ~CThing 0x004C8310. Ego_r.pdb + FableWin.pdb name
//   `dynamic initializer for 'CThing::TCsToBeRemoved'` and its `dynamic atexit destructor`; Ego_r runs it after
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass and before the CConversationAttitude maps, as retail's .CRT$XCU runs 0x0121A430 ->
//   0x0121A440 -> 0x0121A460 (CConversationAttitude::GeneralAttitudeToAnimName). The initializer registers the destructor 0x01228CE0
//   (`push 1228CE0h`); the destructor clears the list (STLport _List_base::clear 0x004CC370, out of line) and frees the sentinel.
//   The landed catalog name `CThing::$E_TCsToBeRemoved` on 0x0121A670 was wrong (that initializer builds the NThingManager map at 0x013B8A8C).
//   The same source as the two-row pair 0x0121F0E0 / 0x01229BE0 (NF89): the static defined on the shared header (engine/CThing.h NF90 opt-in
//   FABLE_RETAIL_CTHING_TCSTOBEREMOVED), the compiler emitting both functions.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_TCSTOBEREMOVED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"

std::list<std::pair<CIntelligentPointer<CThing>, CTCBase*> > CThing::TCsToBeRemoved;   // 0x013B8A24
