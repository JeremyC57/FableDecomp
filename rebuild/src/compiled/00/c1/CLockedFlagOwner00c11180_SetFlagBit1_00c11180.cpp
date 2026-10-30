// CLockedFlagOwner_00c11180::SetFlagBit1() -- under the scoped lock over the global @ 0x0143BB14, set bit 1 (mask 2) of the flag word at +0x7c8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11180, length 49 bytes.
// Callees: the 8-byte scoped lock's constructor @ 0x00C0FFC0 and destructor @ 0x00C0FFE0 (landed users:
// 0x00C10DD0 ProcessGuard, which sets bits 1 and 2 of the same +0x7c8 word under the same lock).
// The chunk label "CTCActionUseBuyHouse::OnCreate" is propagated (the real OnCreate bodies are landed at
// 0x004DC83E / 0x0065F0FA / 0x0072E7F0 / 0x007B2A90 and the class has no +0x7c8 member): the owner is
// named by address.
// Shape: `sub esp,8` holds the lock; the flag update is a load / `or eax,2` / store split around
// the destructor's `lea ecx`: a one-bit BITFIELD assignment (`m_bit1 = 1`). A plain `m_flags |= 2`
// gives the read-modify-write `or dword ptr [esi+7C8h],2` of 0x00C10DD0. No EH frame: nothing
// between the lock's construction and destruction can throw.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;  // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock (section + entry tick)
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CLockedFlagOwner_00c11180 {
    unsigned char _pad[0x7c8];
    unsigned long m_bit0 : 1;              // +0x7c8 bit 0
    unsigned long m_bit1 : 1;              // +0x7c8 bit 1
    unsigned long m_bit2 : 1;              // +0x7c8 bit 2
    void SetFlagBit1();
};

void CLockedFlagOwner_00c11180::SetFlagBit1()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    m_bit1 = 1;
}