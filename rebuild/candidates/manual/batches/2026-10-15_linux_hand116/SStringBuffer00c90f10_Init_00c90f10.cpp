// SStringBuffer_00c90f10::Init() -- clear the five-word buffer record, then allocate a 256-byte buffer as both its start and cursor, empty it and set the capacity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C90F10, length 51 bytes. Callee: malloc through the import slot @ 0x01440158.
// Leaf: Init
// The catalog label "CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo" is propagated. It is no constructor: it returns the
// buffer (eax) rather than `this`. Callers build it on a stack record (0x00C8EA70 Audio_CreateAlignedBuffer) and a one-jump thunk
// at 0x00C90F50 forwards to it.
// Shape: `xor eax,eax` zero stores through ecx BEFORE `mov esi,ecx`, and the first buffer byte cleared with an IMMEDIATE
// `mov byte [eax],0`: the clear is the intrinsic memset of the whole record -- its zero register is private to the expansion,
// while five plain zero stores make VC keep 0 in ebx across the call and store `bl` into the buffer.
#include <string.h>

extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);

struct SStringBuffer_00c90f10 {
    long  m_a;          // +0x00
    long  m_b;          // +0x04
    char* m_pBuffer;    // +0x08
    char* m_pCursor;    // +0x0c
    long  m_capacity;   // +0x10
    void Init();
};

void SStringBuffer_00c90f10::Init()
{
    memset(this, 0, sizeof(*this));
    m_pBuffer = (char*)malloc(0x100);
    m_pCursor = m_pBuffer;
    *m_pCursor = 0;
    m_capacity = 0x100;
}
