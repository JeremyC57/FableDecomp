// CAudioBank_00cab990::LoadAsync(request) -- EH-framed (the manifest's "CAudioBank_LoadAsync"): no +0x18 source object means 0x80040209; the source is asked for its stream interface (COM `QueryInterface` with the IID at 0x012A93F4 into a one-word smart pointer on the frame, released through slot 2 inline and through 0x00CAA6E0 on unwind; state 0); a stream that does not accept the request (slot 3) hands it to the synchronous loader (@ 0x00CA9B70) and a negative result is returned, any other result as 0; otherwise the engine at +0xd4 with a live +0xd0 context queues it through its slot 3 (&+0xc, 0, request, 0, context, 2) or E_FAIL without them
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAB990, oracle length 305 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; push esi ; mov esi,ecx ; mov eax,[esi+18h] ; test eax,eax ; jne go ; mov eax,80040209h ; ... ret 4 ; go: push edi ; mov dword ptr [esp+8],0 ; mov eax,[esi+18h] ; mov ecx,[eax] ; lea edx,[esp+8] ; push edx ; push offset IID_Stream ; push eax ; mov dword ptr [esp+20h],0 ; call [ecx] ; mov eax,[esp+8] ; test eax,eax ; mov edi,[esp+1Ch] ; je engine ; mov ecx,[eax] ; push edi ; push eax ; call [ecx+0Ch] ; test eax,eax ; jne reload ; push edi ; mov ecx,esi ; call CAudioBank::LoadSync ; mov esi,eax ; test esi,esi ; mov eax,[esp+8] ; mov dword ptr [esp+14h],-1 ; jge zero ; test eax,eax ; je out ; mov edx,[eax] ; push eax ; call [edx+8] ; pop edi ; mov eax,esi ; ... ret 4 ; zero: test eax,eax ; je a ; mov ecx,[eax] ; push eax ; call [ecx+8] ; a: pop edi ; xor eax,eax ; ... ret 4 ; reload: mov eax,[esp+8] ; engine: mov ecx,[esi+0D4h] ; test ecx,ecx ; je fail ; mov ecx,[esi+0D0h] ; test ecx,ecx ; je fail ; mov eax,[esi+0D4h] ; mov edx,[eax] ; push 2 ; push ecx ; push 0 ; push edi ; push 0 ; add esi,0Ch ; push esi ; push eax ; call [edx+0Ch] ; mov esi,eax ; mov eax,[esp+8] ; jmp rel ; fail: mov esi,80004005h ; rel: test eax,eax ; mov dword ptr [esp+14h],-1 ; je out ; mov ecx,[eax] ; push eax ; call [ecx+8] ; out: ... mov eax,esi ; ... ret 4
// The classes are named by address.
struct GUID_00cab990 { unsigned long a; unsigned short b, c; unsigned char d[8]; };
extern const GUID_00cab990 IID_Stream_00cab990;   // 0x012A93F4

struct IUnknown_00cab990 {                 // COM: `this` on the stack
    virtual long __stdcall QueryInterface(const GUID_00cab990& iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};
struct IStream_00cab990 : IUnknown_00cab990 {
    virtual long __stdcall Accepts(void* request);   // slot 0xc
};
struct IEngine_00cab990 : IUnknown_00cab990 {
    virtual long __stdcall Queue(void* slot, long a, void* request, long b, void* context, long kind);   // slot 0xc
};
struct CStreamPtr_00cab990 {               // one-word smart pointer: released on scope exit (0x00CAA6E0 on unwind)
    IStream_00cab990* m_p;
    CStreamPtr_00cab990() : m_p(0) { }
    ~CStreamPtr_00cab990() { if (m_p) m_p->Release(); }
};

struct CAudioBank_00cab990 {
    unsigned char _pad[0xc];
    unsigned char m_slot[0xc];             // +0xc
    IUnknown_00cab990* m_pSource;          // +0x18
    unsigned char _pad2[0xb4];
    void* m_pContext;                      // +0xd0
    IEngine_00cab990* m_pEngine;           // +0xd4
    long LoadSync(void* request);          // 0x00CA9B70
    long LoadAsync(void* request);
};

long CAudioBank_00cab990::LoadAsync(void* request)
{
    if (!m_pSource)
        return 0x80040209;
    CStreamPtr_00cab990 stream;
    m_pSource->QueryInterface(IID_Stream_00cab990, (void**)&stream.m_p);
    if (stream.m_p) {
        if (!stream.m_p->Accepts(request)) {
            long result = LoadSync(request);
            if (result < 0)
                return result;
            return 0;
        }
    }
    long result;
    if (m_pEngine && m_pContext)
        result = m_pEngine->Queue(m_slot, 0, request, 0, m_pContext, 2);
    else
        result = 0x80004005;
    return result;
}
