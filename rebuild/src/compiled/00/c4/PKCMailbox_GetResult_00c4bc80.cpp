// PKCMailbox::GetResult() -- EH-framed: the semaphore member at +0x1c is waited on (@ 0x00C2A090); under a scoped lock over `this` (state 0) an empty queue at +0x18 (virtual slot 2 not above 0) logs `d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h(63) : the queue is out of step with the mailbox handler` through a stack string (in its own block, state 1; chained `__fastcall` builders, optional sink @ 0x0143BB2C, release @ 0x00C0BA40) with an empty trace temporary on a dead frame byte (@ 0x00C16750); returns the queue's virtual slot 0
// Vendor naming V2 (plan phase 1, header-first): PKCSemaphore comes from the shared libpk/PKCSemaphore.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C2A090 ?Wait@PKCSemaphore@@QAEXXZ (was ?Wait@CSemaphore_00c4bc80@@QAEXXZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4BC80, oracle length 236 bytes. Built under /EHsc.
// Retail bytes:  ... lea ecx,[esi+1Ch] ; call CSemaphore::Wait ; push esi ; lea ecx,[esp+10h] ; call CScopedLock::CScopedLock ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+1Ch],0 ; call [eax+8] ; test eax,eax ; ja pop ; mov ecx,[g_pchNil] ; mov [esp+8],ecx ; mov edx,offset file ; lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],1 ; call Append ; ... AppendNumber(3Fh) ... ; mov eax,[g_pfnLogSink] ; test eax,eax ; je rel ; mov ecx,[esp+8] ; call eax ; rel: lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],0 ; call ~CString ; lea ecx,[esp+7] ; call CTrace::CTrace ; pop: mov ecx,[esi+18h] ; mov edx,[ecx] ; call [edx] ; lea ecx,[esp+0Ch] ; mov esi,eax ; mov dword ptr [esp+1Ch],-1 ; call ~CScopedLock ; ... mov eax,esi ... ret
// Byte-identical twin of 0x0x00C49FF0. The manifest label is a description. Name-fix NF38: the class is libpk/PKCMailbox.h PKCMailbox
// (descriptive; the landed caller 0x00C4AC70 binds ?GetResult@PKCMailbox@@QAEKXZ); was the address-named local CMailbox_00c4bc80.
// Leaf: ?GetResult@PKCMailbox@@QAEKXZ
// Flags: /EHsc
#define FABLE_RETAIL_PKCMAILBOX_BUFFEREDREADER_PASS23
#define FABLE_RETAIL_PKCBUFFEREDREADER_RETURN_CONTEXT_PASS23
#define FABLE_RETAIL_CCODAFRAMESTREAMER_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "libpk/PKCMailbox.h"
#include "libpk/CCodaFrameStreamer.h"
#include "libpk/PKCProtectionInstance.h"
#include "engine/CString.h"

// The queue at +0x18 is the libpk static queue (slot 0 Pop, slot 2 GetCount); the mailbox stores it untyped.
unsigned long PKCMailbox::GetResult()
{
    m_semaphore.Wait();
    PKCProtectionInstance lock(&m_criticalSection);
    PKCStaticQueue<unsigned long>* queue = static_cast<PKCStaticQueue<unsigned long>*>(m_pQueue);
    if (!((unsigned long)queue->GetCount() > 0)) {
        {
            CString line;
            line << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h" << "(" << 63L << ") : " << "the queue is out of step with the mailbox handler" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    return static_cast<PKCStaticQueue<unsigned long>*>(m_pQueue)->Pop();
}
