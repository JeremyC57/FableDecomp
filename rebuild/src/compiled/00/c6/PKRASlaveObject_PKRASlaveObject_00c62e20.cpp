// PKRASlaveObject::PKRASlaveObject(void* owner) -- construct a libpk RA slave object: its vtable (0x012B93D4) and the owner the completion slot reports to (+0x04)
// Leaf: ??0PKRASlaveObject@@QAE@PAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C62E20, 18 bytes, thiscall `ret 4`: `mov eax,ecx ; mov ecx,[esp+4] ;
//   mov dword ptr [eax],offset vtable ; mov [eax+4],ecx ; ret 4`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 442 "CVtable_012b93d4 0xC62E20 = PKRASlaveObject ctor"): 0x012B93D4 is the
//   vtable of agent442's landed PKRASlaveObject::PassToSlave 0x00C62E60 (slot 2; the class is named by the file string
//   ".\PKRASlaveObject.cpp" of its error report): slot 0 the deleting destructor 0x00C62F30, slot 1 0x00C62E40 (hands the
//   data to the +0x04 owner through 0x00C4C7A0, then deletes this). Callers 0x00C4CB81 and 0x00C62C94 (bound there as
//   ??0CSlaveObject_00c62c70@@QAE@PAX@Z). The landed CVtable_012b93d4 name is a placeholder; member names descriptive.
// Shape: no base constructor call (a root class); the argument is stored after the vtable.
class PKRASlaveObject {                             // vtable 0x012B93D4
public:
    PKRASlaveObject(void* owner);
    virtual ~PKRASlaveObject();                                  // slot 0 (deleting wrapper 0x00C62F30)
    virtual void OnComplete(void* data, long flags);             // slot 1 (0x00C62E40)
    virtual void PassToSlave(void* data, PKRASlaveObject* slave);   // slot 2 (0x00C62E60)
    void* m_pOwner;                                 // +0x04
};

PKRASlaveObject::PKRASlaveObject(void* owner)
    : m_pOwner(owner)
{
}
