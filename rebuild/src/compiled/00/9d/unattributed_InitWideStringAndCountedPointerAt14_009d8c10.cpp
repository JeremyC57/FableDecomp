// _unattributed::InitWideStringAndCountedPointerAt14() -- constructor: CWideString at +0x14, empty CCountedPointer at +0x18, first 0x14 bytes untouched
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D8C10, oracle length 23 bytes.
// Callees: CWideString::CWideString @ 0x0099AED0 (the default constructor that nulls the string pointer and bumps the
// global CWideString counter at 0x013BCA20; the manifest's "CCharString::CCharString" label on it is propagated --
// the CCharString constructor at 0x0099E4B0 bumps a different counter).
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+14h] ; call CWideString::CWideString ; xor eax,eax ; mov [esi+18h],eax ;
//                mov [esi+1Ch],eax ; mov eax,esi ; pop esi ; ret
// Identity: the manifest label "C3DSkeletalMorphTarget::C3DSkeletalMorphTarget" is propagated and rejected
// (that class starts with a CCharString at +0). No vtable store and no base call, so the first 0x14
// bytes are trivially-constructed members (the class is 0x20 bytes; the neighbours at 0x009D8xxx are
// CRenderManager2D/CShaderRenderManager code). The two zero stores after the string are a
// CCountedPointer with its inline zeroing constructor. Landed as _unattributed with descriptive names.
typedef unsigned short wchar_t;          // VC7.1 without /Zc:wchar_t: wchar_t is the CRT typedef

class CWideString { public:
    wchar_t* m_pString;
    CWideString();                         // 0x0099AED0
};

struct CCountedPointer {
    void* m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

struct CUnattributed_009d8c10 {
    unsigned char   m_leading[0x14];       // +0x00: trivially constructed members, not touched here
    CWideString     m_name;                // +0x14
    CCountedPointer m_pObject;             // +0x18
    CUnattributed_009d8c10();
};

CUnattributed_009d8c10::CUnattributed_009d8c10()
    : m_name(), m_pObject()
{
}