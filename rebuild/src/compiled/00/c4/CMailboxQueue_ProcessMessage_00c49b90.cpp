// CMailboxQueue::ProcessMessage(void*) -- under a scoped queue lock (ctor @ 0x00C0FFC0, dtor @ 0x00C0FFE0): 1 when the mailbox at +0x18 has a pending item (virtual slot 3), else release the semaphore at +0x1C (@ 0x00C2A150), push the message (virtual slot 1) and return 0; EH-framed
// Vendor naming V2 (plan phase 1, header-first): PKCSemaphore comes from the shared libpk/PKCSemaphore.h (was a local copy).
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C2A150 ?Release@PKCSemaphore@@QAEXXZ (was ?Release@CSemaphore@@QAEXXZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49B90, oracle length 155 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push esi ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+14h],0 ; call [eax+0Ch] ; test eax,eax ; je E ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov eax,1 ; pop esi ; mov ecx,[esp+8] ; mov fs:[0],ecx ; add esp,14h ; ret 4 ; E: lea ecx,[esi+1Ch] ; call PKCSemaphore::Release ; mov ecx,[esi+18h] ; mov eax,[message] ; mov edx,[ecx] ; push eax ; call [edx+4] ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// Mailbox queue step under an 8-byte scoped lock (constructed over the queue, EH state 0 while it
// lives, destroyed on both exits): when the mailbox at +0x18 reports a pending item through its
// virtual slot 3 the call returns 1; otherwise the semaphore at +0x1C is released (@ 0x00C2A150),
// the message is pushed through virtual slot 1 and 0 is returned. Built under /EHsc (the
// `__ehhandler` is not compared; the unwind funclet lands separately through the funclet pass).
// The manifest label is propagated. Name-fix NF38: the class is libpk/CCodaFrameStreamer.h CMailboxQueue (descriptive; its
// callers 0x00C47F10 / 0x00C49470 bind ?ProcessMessage@CMailboxQueue@@QAEJPAX@Z); was the address-named local CMailboxQueue_00c49b90. One of 8
// bodies of this shape.
// Leaf: ?ProcessMessage@CMailboxQueue@@QAEJPAX@Z
#define FABLE_RETAIL_CCODAFRAMESTREAMER_PASS23
#include "libpk/CCodaFrameStreamer.h"
#include "libpk/PKCProtectionInstance.h"

long CMailboxQueue::ProcessMessage(void* message)
{
    PKCProtectionInstance lock(&m_criticalSection);
    if (m_pQueue->IsFull())
        return 1;
    m_semaphore.Release();
    m_pQueue->Push(static_cast<CCodaFrame* const*>(message));
    return 0;
}
