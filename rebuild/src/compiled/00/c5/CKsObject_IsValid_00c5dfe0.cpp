// CKsObject::IsValid() -- TRUE while the object holds an open handle (+4 is not INVALID_HANDLE_VALUE)
// Leaf: ?IsValid@CKsObject@@QAEHXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5DFE0, 12 bytes, thiscall `ret`: `mov edx,[ecx+4] ; xor eax,eax ; cmp edx,-1 ; setne al` (a BOOL:
//   a bool result is `cmp [ecx+4],-1 ; setne al` with no zeroed eax).
// Identity: class CKsObject -- libpk/GameCODA's kernel-streaming handle object (vtable 0x012B90D4: one slot, the deleting
//   destructor 0x00C5EC00; destructor 0x00C5EC50 closes +4 unless INVALID_HANDLE_VALUE; Reset 0x00C5E3F0; SetProperty 0x00C5E050
//   issues IOCTL_KS_PROPERTY on +4). The landed rows spell the class CKsObject (SetProperty, the name this file keeps) and, for
//   the destructor / Reset / deleting destructor, the vtable placeholder CVtable_012b90d4_00c5ec50 (one class: the same handle
//   object at +0xc of its owner, 0x00C5E901 / 0x00C5EAD0). libpk has no PDB or RTTI: names are descriptive.
//   No reference: an out-of-line copy of an inline member.
typedef void* HANDLE;
typedef int BOOL;
typedef unsigned long DWORD;
typedef unsigned long ULONG;

class CKsObject {                                   // vtable 0x012B90D4, 8 bytes
public:
    CKsObject();                                    // 0x00C5DFD0
    virtual ~CKsObject();                           // slot 0: the deleting destructor 0x00C5EC00 (destructor 0x00C5EC50)
    BOOL   IsValid();                               // 0x00C5DFE0
    HANDLE GetHandle();                             // 0x00C5DFF0
    HANDLE m_handle;                                // +0x04 the kernel-streaming object's handle, INVALID_HANDLE_VALUE when closed
};

BOOL CKsObject::IsValid()
{
    return m_handle != (HANDLE)-1;
}