// CSegmentList::CSegmentList(PKCTRAllocator*) -- EH-framed: the list's container pointer (+0x4) aimed at the embedded red-black tree (+0x8), which is constructed from the argument
// Leaf: ??0CSegmentList@@QAE@PAUPKCTRAllocator@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2DFE0, oracle length 79 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe).
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; lea ecx,[esi+0x8] ; mov [esp+4],esi ; mov [esi+0x4],ecx ; mov eax,[esp+18h] ; push eax ; mov dword ptr [esp+14h],0 ; mov dword ptr [esi],offset CSegmentList::`vftable' ; call PKCTRRedblackTree::PKCTRRedblackTree ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// Built with C++ exceptions enabled (/EHsc, the fs:[0] frame; the `__ehhandler` funclet is not
// compared). The PKCTRList<CSegment> base is handed the tree's address (its empty inline
// destructor gives the frame EH state 0, its own vtable store is dead and dropped), the
// CSegmentList vftable (0x012B47D0) is installed and the tree is constructed out of line
// @ 0x00C194F0 from the one argument.
// NF52 (round 7): moved onto libpk/CSegmentFile.h (overlay opt-in FABLE_RETAIL_CSEGMENTLIST_CTOR_ALLOCATOR declares this
// constructor; the header's inline default constructor is the same body with a null allocator). Was local CVtable_012b47d0 /
// CVtable_012b47d0_Base / CMember_00c194f0 models named by address. Descriptive name: libpk ships no PDB; the class is
// CSegmentList by its vftable 0x012B47D0 (CSegmentFile.h), the parameter is PKCTRRedblackTree's constructor argument.
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#define FABLE_RETAIL_CSEGMENTLIST_CTOR_ALLOCATOR
#include "libpk/CSegmentFile.h"

CSegmentList::CSegmentList(PKCTRAllocator* allocator)
    : PKCTRList<CSegment>(&Tree)
    , Tree(allocator)
{
}
