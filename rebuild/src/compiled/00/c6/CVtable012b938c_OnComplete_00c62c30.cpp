// CVtable_012b938c::OnComplete(bool success, long flags) -- vtable slot 1 of the libpk buffered RA slave: on success copy the received bytes (+0x0C bytes from the buffer +0x14 at offset +0x10) to the caller's destination (+0x18), then report to the master object and delete this slave
// Leaf: ?OnComplete@CVtable_012b938c@@UAEX_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C62C30, 64 bytes (+ int3 padding), thiscall `ret 8`; slot 1 of the vtable
//   0x012B938C (word at 0x012B9390). The class: ctor 0x00C62C70 (the PKRASlaveObject base 0x00C62E20, count +8, size +0xC,
//   offset +0x10, buffer +0x14, destination +0x18) and dtor 0x00C62BB0, landed under the propagated labels
//   "CSBAgents_00c62c70" / "CTCInventoryItem_00c62bb0"; named by its vtable here.
// Callees: the inline memcpy intrinsic (`rep movsd` + `rep movsb`), 0x00C4C7A0 on the owner (PKRACMasterObject::
//   ReceiveObject, descriptive), the virtual deleting destructor = `delete this`.
// Name: descriptive (libpk, no PDB/RTTI); the slot overrides PKRASlaveObject::OnComplete 0x00C62E40 (this chunk).
// Shape: `this` in ebx; the byte flag tested once, the whole word passed on.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
class PKRACMasterObject {                           // the RA master the slave reports to (".\PKRACMasterObject.cpp")
public:
    void ReceiveObject(bool success);               // 0x00C4C7A0 (thiscall `ret 4`; catalog "RACMasterObject_ReceiveObject")
};
class PKRASlaveObject {                             // vtable 0x012B93D4 (ctor 0x00C62E20, PassToSlave 0x00C62E60 landed)
public:
    PKRASlaveObject(void* owner);
    virtual ~PKRASlaveObject();                     // slot 0 (deleting wrapper 0x00C62F30)
    virtual void OnComplete(bool success, long flags);   // slot 1 (0x00C62E40)
    PKRACMasterObject* m_pOwner;                    // +0x04
};
class CVtable_012b938c : public PKRASlaveObject {  // vtable 0x012B938C
public:
    virtual void OnComplete(bool success, long flags);   // slot 1 (0x00C62C30)
    unsigned long  m_count;                         // +0x08
    unsigned long  m_size;                          // +0x0c bytes to copy
    unsigned long  m_offset;                        // +0x10
    unsigned char* m_pBuffer;                       // +0x14
    void*          m_pDest;                         // +0x18
};

void CVtable_012b938c::OnComplete(bool success, long flags)
{
    if (success)
        memcpy(m_pDest, m_pBuffer + m_offset, m_size);
    m_pOwner->ReceiveObject(success);
    delete this;
}