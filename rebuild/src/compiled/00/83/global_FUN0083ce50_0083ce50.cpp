// _global::FUN_0083ce50(const CNullPointer&) -- a one-pointer handle built null from an ignored argument; returns this
// Leaf: ??0CHandle_0083ce50
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083CE50, 11 bytes, `mov eax,ecx ; mov dword [eax],0 ; ret 4`: a constructor
//   (returns this) taking one argument it never reads. Unreferenced; the owning pointer class is not identified,
//   modelled as a handle with a null-tag constructor.
struct CNullTag_0083ce50 { };

struct CHandle_0083ce50 {
    void* m_p;                                   // +0x00
    CHandle_0083ce50(const CNullTag_0083ce50& tag);
};

CHandle_0083ce50::CHandle_0083ce50(const CNullTag_0083ce50&)
    : m_p(0)
{
}