// CStringAndBuffer_0087b660::~CStringAndBuffer_0087b660() -- frees the buffer at +0xf0 if set, then the CCharString at +0 is destroyed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087B660, oracle length 30 bytes.
// Grade: candidate -- still DIFFER(27v30) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0xf0] ; test eax,eax ; je K ; push eax ; call free ; add esp,4 ; K: mov ecx,esi ; pop esi ; jmp CCharString::~CCharString
// Destructor of a small non-polymorphic class: a CCharString at +0 (destroyed by the tail call to
// ~CCharString @ 0x0099EAE0, fable_string.h) and a malloc'd buffer at +0xf0 released with
// free @ 0x00BFEA14 (landed-trusted) when non-null. The class is named from the manifest label only
// when it is unique. One of 12 bodies of this shape.
#include "fable_string.h"
extern "C" void __cdecl free(void* block);

struct CStringAndBuffer_0087b660 {
    CCharString m_name;                    // +0
    void* m_buffer;                        // +0xf0
    ~CStringAndBuffer_0087b660();
};

CStringAndBuffer_0087b660::~CStringAndBuffer_0087b660()
{
    if (m_buffer)
        free(m_buffer);
}
