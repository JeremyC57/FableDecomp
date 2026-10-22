// _global::FUN_007b55c0(const CNullPointer&) -- a one-pointer handle built null from an ignored argument; returns this
// Leaf: ??0CHandle_007b55c0
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B55C0, 11 bytes, `mov eax,ecx ; mov dword [eax],0 ; ret 4`: a constructor
//   (returns this) taking one argument it never reads. Unreferenced; the owning pointer class is not identified,
//   modelled as a handle with a null-tag constructor.
struct CNullTag_007b55c0 { };

struct CHandle_007b55c0 {
    void* m_p;                                   // +0x00
    CHandle_007b55c0(const CNullTag_007b55c0& tag);
};

CHandle_007b55c0::CHandle_007b55c0(const CNullTag_007b55c0&)
    : m_p(0)
{
}
