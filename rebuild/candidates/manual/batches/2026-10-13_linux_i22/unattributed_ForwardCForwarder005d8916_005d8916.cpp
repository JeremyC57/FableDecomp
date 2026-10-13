// CForwarder_005d8916::Forward(SHandle, void*) -- forwards the hidden result pointer, the by-value 4-byte handle (copied into a reserved slot) and the pointer to the __thiscall helper @ 0x005D87B6
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D8916, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push [c] ; mov edx,[handle] ; push ecx ; mov eax,esp ; push [result] ; mov [eax],edx ; call Helper ; mov eax,[result] ; ret 0Ch
// A forwarder returning a class through the hidden pointer: `return Helper(handle, c)` passes
// its own result pointer straight through, copies the 4-byte handle class (user copy
// constructor, so the slot is reserved by `push ecx` and the copy stored in place) and the
// trailing pointer, and returns the result pointer. The manifest label
// (`CPersistTraits<C2DVector>::TransferIn`) is propagated. One of 24 bodies of this shape.
struct SHandle {                            // 4-byte class with a user copy constructor
    void* m_pObject;
    SHandle(const SHandle& other) : m_pObject(other.m_pObject) { }
};

struct SResult {                            // returned through the hidden pointer
    void* m_pValue;
    SResult(const SResult& other) : m_pValue(other.m_pValue) { }
};

struct CForwarder_005d8916 {
    SResult Helper(SHandle handle, void* context);   // 0x005D87B6
    SResult Forward_CForwarder_005d8916(SHandle handle, void* context);
};

SResult CForwarder_005d8916::Forward_CForwarder_005d8916(SHandle handle, void* context)
{
    return Helper(handle, context);
}
