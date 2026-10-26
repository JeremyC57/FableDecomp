// CVtable_012b4f74::Close() -- on a live object: unregisters it from the global notification manager when both its registered bits are set, frees its owned buffer, then clears the state word
// Leaf: ?Close@CVtable_012b4f74@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C450, length 70 bytes (+ int3 padding).
// Callees: the manager at 0x0143BB5C (vtable slot 1, `call [edx+4]`, the object pushed),
//          FreeBuffer 0x00C3C370 (__cdecl, forwards to the free hook held in 0x0139C980).
// Catalog label "CTCHeroStats::ProcessEventNotification" is a Ghidra guess: the body is slot 1 of
// the no-RTTI vtable 0x012B4F74 (slot 0 = the deleting destructor 0x00C33170, slot 2 = the setup
// method 0x00C3C4E0 that allocates the buffer through 0x00C3C360 and sets bit 1), and the
// destructor 0x00C33120 calls it directly before its base destructor. No class name survives
// (no complete-object locator), so the class is named after its vtable.
// Layout: +0x28 the owned buffer, +0x2c the state word (bit 1 = buffer owned, bits 2 and 3 =
// registered with the manager).
// Shape: the state word is loaded once for the zero test and both bit probes reuse that register
// (`mov ecx,eax ; shr ecx,2 ; test cl,1` then `shr eax,3 ; test al,1`): the probes are inline
// accessors returning `(unsigned char)(word >> n) & 1`. The buffer test after the virtual call
// re-reads memory (`test byte ptr [esi+2Ch],2`) -- a plain `& 2` on the member.
struct CVtable_012b4f74;

struct CNotificationManager {
    virtual void v00();
    virtual void Unregister(CVtable_012b4f74* client);     // slot 1
};

extern CNotificationManager* g_pNotificationManager;       // 0x0143BB5C

void __cdecl FreeBuffer_00c3c370(void* buffer);                     // 0x00C3C370

struct CVtable_012b4f74 {
    virtual void v00();                                    // slot 0: deleting destructor 0x00C33170
    virtual void Close();                                  // slot 1
    unsigned char _pad_0x04[0x24];
    void*         m_pBuffer;                               // +0x28
    unsigned long m_state;                                 // +0x2c

    unsigned char IsRegisteredA() const { return (unsigned char)(m_state >> 2) & 1; }
    unsigned char IsRegisteredB() const { return (unsigned char)(m_state >> 3) & 1; }
};

void CVtable_012b4f74::Close()
{
    if (m_state != 0) {
        if (IsRegisteredA() && IsRegisteredB())
            g_pNotificationManager->Unregister(this);
        if ((m_state & 2) && m_pBuffer)
            FreeBuffer_00c3c370(m_pBuffer);
        m_state = 0;
    }
}