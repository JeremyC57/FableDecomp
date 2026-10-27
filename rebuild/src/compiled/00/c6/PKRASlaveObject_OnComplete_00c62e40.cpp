// PKRASlaveObject::OnComplete(bool success, long flags) -- vtable slot 1 of the libpk RA slave object: report the result to the owning master object (ReceiveObject), then delete this slave
// Leaf: ?OnComplete@PKRASlaveObject@@UAEX_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C62E40, 28 bytes (+ int3 padding), thiscall `ret 8` (`flags` unused);
//   slot 1 of the vtable 0x012B93D4 (word at 0x012B93D8).
// Callees: 0x00C4C7A0 on the owner at +4 (thiscall `ret 4`; catalog "_global::RACMasterObject_ReceiveObject", descriptive
//   PKRACMasterObject::ReceiveObject), then the virtual deleting destructor (slot 0, argument 1) = `delete this`.
// Name: the landed PKRASlaveObject rows (0x00C62E20, 0x00C62E60) declare slot 1 as OnComplete; the first parameter is a
//   bool here because the derived override 0x00C62C30 tests it as a byte (`test al,al`) and both pass the whole word on
//   (the landed PassToSlave declares it `void* data`). Descriptive (libpk, no PDB).
// Shape: `delete this` through a virtual destructor is `mov edx,[esi] ; push 1 ; mov ecx,esi ; call [edx]`.
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

void PKRASlaveObject::OnComplete(bool success, long flags)
{
    m_pOwner->ReceiveObject(success);
    delete this;
}