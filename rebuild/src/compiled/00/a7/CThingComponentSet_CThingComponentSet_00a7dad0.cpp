// CThingComponentSet destructor @ 00a7dad0
// push esi; mov esi,ecx; mov eax,[esi]; push eax; call free/delete; add esp,4;
// mov [esi],0; mov byte [esi+4],0; pop esi; ret
#include <stdlib.h>
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)

struct CThingComponentSet {
    void*         m_data;   // +0x00 : heap pointer
    unsigned char m_flag;   // +0x04 : bool

    ~CThingComponentSet();
};

CThingComponentSet::~CThingComponentSet()
{
    operator delete[](m_data);
    m_data = 0;
    m_flag = 0;
}