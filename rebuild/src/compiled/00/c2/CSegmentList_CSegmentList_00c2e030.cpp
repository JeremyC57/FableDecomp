// CSegmentList::~CSegmentList() -- EH-framed: own vftable, the red-black tree at +0x8 destroyed out of line, then the inline empty PKCTRList<CSegment> / CTypedListBase destructor
// Leaf: ??1CSegmentList@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E030, oracle length 73 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe).
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset CSegmentList::`vftable' ; lea ecx,[esi+0x8] ; mov dword ptr [esp+10h],0 ; call PKCTRRedblackTree::~PKCTRRedblackTree ; mov ecx,[esp+8] ; mov dword ptr [esi],offset CTypedListBase::`vftable' ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// This translation unit was built with C++ exceptions enabled (/EHsc: the fs:[0] frame and the
// `__ehhandler` funclet, which the lander does not compare). Installs the CSegmentList vftable
// (0x012B47D0), destroys the tree @ 0x00C19250 under EH state 0, then the inline empty base
// destructor stores the CTypedListBase vftable (0x012B1AB0).
// NF52 (round 7): moved onto libpk/CSegmentFile.h: retail's out-of-line copy of the header's inline virtual destructor (called by
// ??_GCSegmentList 0x00C2E080 and the CSegmentFile EH funclets), defined here through the overlay opt-in
// FABLE_RETAIL_CSEGMENTLIST_DTOR_OUTOFLINE. Was local CVtable_012b47d0 / CBase_012b1ab0 / PKCTRRedblackTree models named by address.
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#define FABLE_RETAIL_CSEGMENTLIST_DTOR_OUTOFLINE
#include "libpk/CSegmentFile.h"

CSegmentList::~CSegmentList()
{
}
