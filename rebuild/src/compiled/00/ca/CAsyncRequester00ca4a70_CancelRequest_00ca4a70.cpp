// CAsyncRequester_00ca4a70::CancelRequest() -- cancel the pending request at +0x68 through the device interface at +0x18 (`__stdcall` slot 6, COM style), reset the completion event at +0x54, report whether there was nothing pending
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4A70, length 48 bytes.
// Callees: the device's slot 6 (+0x18, `__stdcall`: the interface pointer pushed as the first argument, the request second), KERNEL32 ResetEvent (IAT 0x0143FED4).
// Shape: the request pointer kept in edi across both calls and tested again for the result (`xor eax,eax ; test edi,edi ; sete al`), the null store inside the branch.
// The chunk label "CGuiWindow::OnResize" is propagated (0x00CAxxxx module); the class is named by address.
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void* event);   // IAT 0x0143FED4

struct CAsyncRequest_00ca4a70;
struct IAsyncDevice_00ca4a70 {                     // a COM-style interface: __stdcall slots, `this` on the stack
    virtual void __stdcall v00();
    virtual void __stdcall v01();
    virtual void __stdcall v02();
    virtual void __stdcall v03();
    virtual void __stdcall v04();
    virtual void __stdcall v05();
    virtual void __stdcall Cancel(CAsyncRequest_00ca4a70* request);   // slot 6 (+0x18)
};

struct CAsyncRequester_00ca4a70 {
    unsigned char           _pad[0x18];
    IAsyncDevice_00ca4a70*  m_pDevice;             // +0x18
    unsigned char           _pad1c[0x38];
    void*                   m_hEvent;              // +0x54
    unsigned char           _pad58[0x10];
    CAsyncRequest_00ca4a70* m_pRequest;            // +0x68
    bool CancelRequest();
};

bool CAsyncRequester_00ca4a70::CancelRequest()
{
    CAsyncRequest_00ca4a70* request = m_pRequest;
    if (request) {
        m_pDevice->Cancel(request);
        m_pRequest = 0;
    }
    ResetEvent(m_hEvent);
    return request == 0;
}