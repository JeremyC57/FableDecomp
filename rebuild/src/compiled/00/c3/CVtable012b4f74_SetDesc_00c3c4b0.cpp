// CVtable_012b4f74::SetDesc(const SNotificationDesc* desc) -- give the backend audio source its notification descriptor: an already registered source re-applies it (slot 3), a fresh one sets it up (slot 2)
// Leaf: ?SetDesc@CVtable_012b4f74@@QAEJPBUSNotificationDesc@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3C4B0, 18 bytes, thiscall with one stack argument left in place for the tail call
//   (`mov eax,[ecx+2Ch]; mov edx,[ecx]; shr eax,2; test al,1; je; jmp [edx+0Ch]; jmp [edx+8]`). One caller, 0x00C360F9.
// Identity (descriptive; libpk has no PDB or RTTI): the class named by its vtable 0x012B4F74 (landed Close 0x00C3C450 slot 1,
//   SetupEventNotification 0x00C3C4E0 slot 2); slot 3 is 0x00C3C4D0 (`mov eax,-9; ret 4`: the base cannot re-apply). The state
//   word +0x2C bit 2 is the landed model's IsRegisteredA. `SetDesc` is a descriptive name (NOTES).
// Extent: the catalog row ran through int3 padding into slot 3 (0x00C3C4D0, 8 bytes); see SPLITS.tsv.
struct SNotificationDesc;
struct CVtable_012b4f74 {
    virtual void v00();                                              // slot 0: deleting destructor 0x00C33170
    virtual void Close();                                            // slot 1: 0x00C3C450
    virtual long SetupEventNotification(const SNotificationDesc*);   // slot 2: 0x00C3C4E0
    virtual long ChangeEventNotification(const SNotificationDesc*);  // slot 3: 0x00C3C4D0 (returns -9 here)
    unsigned char _pad_0x04[0x24];
    void*         m_pBuffer;                                         // +0x28
    unsigned long m_state;                                           // +0x2c

    unsigned char IsRegisteredA() const { return (unsigned char)(m_state >> 2) & 1; }
    long SetDesc(const SNotificationDesc* desc);
};

long CVtable_012b4f74::SetDesc(const SNotificationDesc* desc)
{
    if (IsRegisteredA())
        return ChangeEventNotification(desc);
    return SetupEventNotification(desc);
}