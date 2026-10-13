// AcquireInterfaceReference(IUnknown*, void**) -- COM helper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CACBE0, oracle length 33 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov ecx,[esp+8] ; test ecx,ecx ; jne L ; mov eax,80004003h ; ret 8 ; L: mov eax,[esp+4] ; mov [ecx],eax ; mov ecx,[eax] ; push eax ; call [ecx+4] ; xor eax,eax ; ret 8
// The manifest labels this `CMovableResourceMemoryPool::CanMoveResource(void*, long)`; the bytes
// disagree: ecx is never used as `this`, the first stack argument is a COM object pointer, the
// second an out-pointer. Null out-pointer => E_POINTER (0x80004003); otherwise store the object,
// AddRef it (IUnknown slot 1, __stdcall with the object pushed) and return S_OK. This is the
// classic "hand out a referenced interface" helper used by the movable-resource pool's COM shim.

struct IUnknownCalls {
    virtual long __stdcall QueryInterface(const void* riid, void** out);
    virtual unsigned long __stdcall AddRef();        // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();
};

long __stdcall AcquireInterfaceReference(IUnknownCalls* object, void** out)
{
    if (!out)
        return (long)0x80004003;        // E_POINTER
    *out = object;
    object->AddRef();
    return 0;                           // S_OK
}
