// Behaviour test for CKsObject @ 0x00C5DFE0: IsValid is TRUE exactly when the handle is not INVALID_HANDLE_VALUE (a null handle counts as valid).
#include <cstdio>
typedef void* HANDLE;
typedef int BOOL;
typedef unsigned long DWORD;
typedef unsigned long ULONG;
struct _OVERLAPPED { int dummy; };
typedef struct _OVERLAPPED OVERLAPPED;
typedef struct { ULONG Size; ULONG TypeSpecificFlags; } KSSTREAM_HEADER;
static BOOL g_ioResult; static DWORD g_ioBytes; static DWORD g_ioCode, g_ioOutSize; static void* g_ioOut; static OVERLAPPED* g_ioOv; static HANDLE g_ioDevice;
static BOOL DeviceIoControl(HANDLE device, DWORD code, void*, DWORD, void* out, DWORD outSize, DWORD* bytes, OVERLAPPED* ov)
{ g_ioDevice = device; g_ioCode = code; g_ioOut = out; g_ioOutSize = outSize; g_ioOv = ov; *bytes = g_ioBytes; return g_ioResult; }
#define IOCTL_KS_WRITE_STREAM 0x002F8013
class CKsObject {
public:
    CKsObject();
    virtual ~CKsObject() { }
    BOOL   IsValid();
    HANDLE GetHandle();
    BOOL   WriteStream(KSSTREAM_HEADER* header, OVERLAPPED* overlapped);
    HANDLE m_handle;
};
CKsObject::CKsObject()
    : m_handle((HANDLE)-1)
{
}
BOOL CKsObject::IsValid()
{
    return m_handle != (HANDLE)-1;
}
HANDLE CKsObject::GetHandle()
{
    return m_handle;
}
BOOL CKsObject::WriteStream(KSSTREAM_HEADER* header, OVERLAPPED* overlapped)
{
    DWORD bytesReturned;
    if (!DeviceIoControl(m_handle, IOCTL_KS_WRITE_STREAM, 0, 0, header, header->Size, &bytesReturned, overlapped)
        && (overlapped || bytesReturned == header->Size))
        return 1;
    return 0;
}
int main()
{
    CKsObject k;
    bool ok = k.IsValid() == 0;
    k.m_handle = (HANDLE)0x40; ok = ok && k.IsValid() == 1;
    k.m_handle = (HANDLE)0; ok = ok && k.IsValid() == 1;
    k.m_handle = (HANDLE)-1; ok = ok && k.IsValid() == 0;
    if (!ok) { std::printf("00c5dfe0_TEST FAIL\n"); return 1; }
    std::printf("00c5dfe0_TEST PASS\n");
    return 0;
}