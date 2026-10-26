#include "rebuild_abi.h"
// Leaf: FableDestVal_00768ba0
// std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_> @ 0x00768BA0 (retail, 19 bytes)
// push esi; mov esi,ecx; lea ecx,[esi+0x4]; call 0x0099EAE0; mov ecx,esi; pop esi; jmp 0x0099EAE0
// Destroys a record of two CCharString members, the one at +0x4 first and the one at +0x0 as the tail jump;
// both calls go to CCharString::~CCharString @ 0x0099EAE0 (release 0x0099E9B0, instance count 0x013BD800).
class CCharString
{ public:
    void* PStringData;
    ~CCharString();                 // 0x0099EAE0
};
struct FableDestVal_00768ba0_t
{
    CCharString First;              // +0x0
    CCharString Second;             // +0x4
};
extern "C" void FABLE_FASTCALL FableDestVal_00768ba0(FableDestVal_00768ba0_t* self)
{
    self->Second.~CCharString();
    self->First.~CCharString();
}
