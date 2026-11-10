// CKsObject::WriteStream(KSSTREAM_HEADER* header, OVERLAPPED* overlapped) -- submit one stream header to the kernel-streaming pin (IOCTL_KS_WRITE_STREAM, the header as the out buffer, header->Size bytes) and report TRUE when DeviceIoControl did not complete it at once but the write is queued (an OVERLAPPED was given) or the full header size came back
// Leaf: ?WriteStream@CKsObject@@QAEHPAUKSSTREAM_HEADER@@PAU_OVERLAPPED@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5E000, 72 bytes, thiscall `ret 8`. Callee: KERNEL32!DeviceIoControl through the import slot
//   0x0143FEC0 (the landed SetProperty 0x00C5E050 calls the same slot).
// Identity: class CKsObject -- libpk/GameCODA's kernel-streaming handle object (vtable 0x012B90D4: one slot, the deleting
//   destructor 0x00C5EC00; destructor 0x00C5EC50 closes +4 unless INVALID_HANDLE_VALUE; Reset 0x00C5E3F0; SetProperty 0x00C5E050
//   issues IOCTL_KS_PROPERTY on +4). The landed rows spell the class CKsObject (SetProperty, the name this file keeps) and, for
//   the destructor / Reset / deleting destructor, the vtable placeholder CVtable_012b90d4_00c5ec50 (one class: the same handle
//   object at +0xc of its owner, 0x00C5E901 / 0x00C5EAD0). libpk has no PDB or RTTI: names are descriptive.
//   No reference: an out-of-line member nobody in Fable.exe calls.
// Evidence: 0x002F8013 = CTL_CODE(FILE_DEVICE_KS 0x2F, 4, METHOD_NEITHER, FILE_WRITE_ACCESS) = IOCTL_KS_WRITE_STREAM (ks.h); the
//   out-buffer size is the buffer's first dword, KSSTREAM_HEADER::Size. Retail returns 0 whenever DeviceIoControl returns nonzero
//   (`test eax,eax ; jne -> xor eax,eax`), else 1 when the OVERLAPPED pointer is set or bytesReturned equals the (re-read) Size.
//   bytesReturned has no initial store: it lives in the dead `overlapped` argument slot (the pointer is kept in edi).
typedef void* HANDLE;
typedef int BOOL;
typedef unsigned long DWORD;
typedef unsigned long ULONG;

typedef struct _OVERLAPPED OVERLAPPED;
typedef struct {                                    // ks.h (the members after Size are never touched here)
    ULONG Size;
    ULONG TypeSpecificFlags;
    __int64 PresentationTime_Time;                  // KSTIME
    ULONG PresentationTime_Numerator;
    ULONG PresentationTime_Denominator;
    __int64 Duration;
    ULONG FrameExtent;
    ULONG DataUsed;
    void* Data;
    ULONG OptionsFlags;
} KSSTREAM_HEADER;

extern "C" __declspec(dllimport) BOOL __stdcall DeviceIoControl(HANDLE device, DWORD ioControlCode, void* inBuffer, DWORD inBufferSize,
    void* outBuffer, DWORD outBufferSize, DWORD* bytesReturned, OVERLAPPED* overlapped);   // import slot 0x0143FEC0

#define IOCTL_KS_WRITE_STREAM 0x002F8013

class CKsObject {                                   // vtable 0x012B90D4, 8 bytes
public:
    CKsObject();                                    // 0x00C5DFD0
    virtual ~CKsObject();                           // slot 0: the deleting destructor 0x00C5EC00 (destructor 0x00C5EC50)
    BOOL   IsValid();                               // 0x00C5DFE0
    HANDLE GetHandle();                             // 0x00C5DFF0
    BOOL   WriteStream(KSSTREAM_HEADER* header, OVERLAPPED* overlapped);   // 0x00C5E000
    HANDLE m_handle;                                // +0x04 the kernel-streaming object's handle, INVALID_HANDLE_VALUE when closed
};

BOOL CKsObject::WriteStream(KSSTREAM_HEADER* header, OVERLAPPED* overlapped)
{
    DWORD bytesReturned;
    if (!DeviceIoControl(m_handle, IOCTL_KS_WRITE_STREAM, 0, 0, header, header->Size, &bytesReturned, overlapped)
        && (overlapped || bytesReturned == header->Size))
        return 1;
    return 0;
}