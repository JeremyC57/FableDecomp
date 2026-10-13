// CStringAndBuffer_009d3210::~CStringAndBuffer_009d3210() -- frees the buffer at +0x4 if set, then the CCharString at +0 is destroyed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D3210, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0x4] ; test eax,eax ; je K ; push eax ; call free ; add esp,4 ; K: mov ecx,esi ; pop esi ; jmp CCharString::~CCharString
// Destructor of a small non-polymorphic class: a CCharString at +0 (destroyed by the tail call to
// ~CCharString @ 0x0099EAE0, fable_string.h) and a malloc'd buffer at +0x4 released with
// free @ 0x00BFEA14 (landed-trusted) when non-null. The class is named from the manifest label only
// when it is unique. One of 12 bodies of this shape.
#include "fable_string.h"
extern "C" void __cdecl free(void* block);

struct CStringAndBuffer_009d3210 {
    CCharString m_name;                    // +0
    void* m_buffer;                        // +0x4
    ~CStringAndBuffer_009d3210();
};

CStringAndBuffer_009d3210::~CStringAndBuffer_009d3210()
{
    if (m_buffer)
        free(m_buffer);
}
