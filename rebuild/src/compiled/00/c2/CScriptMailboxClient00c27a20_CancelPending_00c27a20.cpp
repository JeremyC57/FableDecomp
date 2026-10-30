// CScriptMailboxClient_00c27a20::CancelPending() -- under the module's scoped lock, posts an empty (0, 0) notification to the object at +0x0C and clears the pending flag at +0x24
// Leaf: CancelPending
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27A20, length 51 bytes (the 64-byte catalog row ends in int3 padding).
// Callees: PKCProtectionInstance::PKCProtectionInstance @ 0x00C0FFC0 / ~PKCProtectionInstance @ 0x00C0FFE0 (8-byte lock over the global
//          @ 0x0143BAFC), the `__stdcall` notifier @ 0x00C35C30 (tests the target's flags & 0x217 and forwards
//          the two values; manifest label "CTCSpecialAbilities::OnKill" is propagated).
// Label: the manifest's "CTCInventory::ProcessInventoryInputLeft" is propagated; the body is a virtual
//        (vtable 0x012B4048, no RTTI) of the 0x00C2xxxx script module, so the class is named by address.
// Shape: no EH frame (the TU is built without /EHsc); the flag clear sits between the call and the
//        lock's destructor exactly as written.

class PKCCriticalSection;
extern PKCCriticalSection g_soundLock;                 // 0x0143BAFC

class PKCProtectionInstance { public:                                      // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long               m_state;
    PKCProtectionInstance(PKCCriticalSection* target);               // 0x00C0FFC0
    ~PKCProtectionInstance();                                       // 0x00C0FFE0
};

struct CMailboxTarget;
void __stdcall NotifyTarget(CMailboxTarget* target, long a, long b);   // 0x00C35C30

struct CScriptMailboxClient_00c27a20 {
    unsigned char   _pad_0x00[0x0C];
    CMailboxTarget* m_pTarget;                            // +0x0C
    unsigned char   _pad_0x10[0x24 - 0x10];
    bool            m_pending;                            // +0x24
    void CancelPending();
};

void CScriptMailboxClient_00c27a20::CancelPending()
{
    PKCProtectionInstance lock(&g_soundLock);
    NotifyTarget(m_pTarget, 0, 0);
    m_pending = false;
}