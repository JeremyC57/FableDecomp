// CClass_00ca48c0::~CClass_00ca48c0() -- releases the handle at +0 through the one-argument `__stdcall` import @ 0x0143FE10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA48C0, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx] ; push eax ; call dword ptr [0x143fe10] ; ret 
// Tiny-body shape `DtorReleaseImport`: a destructor that passes its one handle member to a
// `__stdcall` import (callee cleans the slot). The import's identity is a relocation; names are descriptive.
extern "C" __declspec(dllimport) long __stdcall Import_0143fe10(void*);   // IAT slot 0x0143FE10

struct CClass_00ca48c0 {
    void* m_handle;                        // +0
    ~CClass_00ca48c0();
};

CClass_00ca48c0::~CClass_00ca48c0()
{
    Import_0143fe10(m_handle);
}