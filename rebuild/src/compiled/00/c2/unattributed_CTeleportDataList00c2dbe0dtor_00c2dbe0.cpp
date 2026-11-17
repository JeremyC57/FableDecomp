// CSegmentFile::~CSegmentFile() -- EH-framed destructor: every item the container member at +0x80 still yields (virtual slot 11) is removed (virtual slot 8) and deleted (its string at +8 released @ 0x00C0BA40, then the global operator delete, null-checked), the container destroyed inline (vtable 0x012B47D0, node at +8 @ 0x00C19250 at state 2, root vtable 0x012B1AB0), then the out-of-line base destructor (@ 0x00C3A9D0)
// Vendor naming wave V1 (one address, one name): 0x00C2DBE0 ??1CSegmentFile@@QAE@XZ (was ??1CTeleportDataList_00c2dbe0@@QAE@XZ)
// Leaf: ??1CSegmentFile@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2DBE0, oracle length 168 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push ebx ; push esi ; mov ebx,ecx ; push edi ; mov [esp+0Ch],ebx ; mov eax,[ebx+80h] ; lea esi,[ebx+80h] ; mov ecx,esi ; mov dword ptr [esp+1Ch],1 ; call [eax+2Ch] ; mov edi,eax ; test edi,edi ; je done ; loop: mov edx,[esi] ; push edi ; mov ecx,esi ; call [edx+20h] ; test edi,edi ; je next ; lea ecx,[edi+8] ; call ~CString ; push edi ; call operator delete ; add esp,4 ; next: mov eax,[esi] ; mov ecx,esi ; call [eax+2Ch] ; mov edi,eax ; test edi,edi ; jne loop ; done: mov [esp+10h],esi ; mov dword ptr [esi],offset vtable ; lea ecx,[esi+8] ; mov byte ptr [esp+1Ch],2 ; call ~CNode ; mov ecx,ebx ; mov dword ptr [esi],offset root ; mov dword ptr [esp+1Ch],-1 ; call ~CBase ; ...
// The most-derived class stores no vtable (`__declspec(novtable)`); the body runs at state 1
// because a member before the container has a non-trivial but empty destructor (state 0). The
// manifest label is propagated; the classes are named by address.
// NF52 (round 7): moved onto libpk/CSegmentFile.h (FABLE_RETAIL_CSEGMENTFILE_PASS23): the container at +0x80 is the CSegmentList Segments
// (vftable ??_7CSegmentList@@6B@ 0x012B47D0, stored by its inline virtual destructor; Tree +8 is PKCTRRedblackTree, destroyed at state 2;
// the PKCTRList<CSegment> root stores 0x012B1AB0), the items are CSegment (CString Name +8), slot 11 / slot 8 are PKCTRList::GetFirst /
// Remove, and Reader (CBufferedRangeReader, destroyed out of line at 0x00C3A9D0) is the first member. Were local CTeleportBase /
// CMarker / CString / CItem / CNode / CContainerRoot / CContainer / CSegmentFile models named by address.
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#include "libpk/CSegmentFile.h"

CSegmentFile::~CSegmentFile()
{
    CSegment* segment = Segments.GetFirst();
    while (segment) {
        Segments.Remove(segment);
        delete segment;
        segment = Segments.GetFirst();
    }
}
