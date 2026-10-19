#include "rebuild_abi.h"
// Leaf: FableDestVal_004d6902
// std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_> @ 0x004D6902 (retail, 19 bytes)
// push esi; mov esi,ecx; lea ecx,[esi+0xc]; call 0x0099EAE0; mov ecx,esi; pop esi; jmp 0x0099EAE0
// Destroys a record of two CCharString members, the one at +0xc first and the one at +0x0 as the tail jump;
// both calls go to CCharString::~CCharString @ 0x0099EAE0 (release 0x0099E9B0, instance count 0x013BD800).
struct CCharString
{
    void* PStringData;
    ~CCharString();                 // 0x0099EAE0
};
struct FableDestVal_004d6902_t
{
    CCharString First;              // +0x0
    char _pad[0x8];
    CCharString Second;             // +0xc
};
extern "C" void FABLE_FASTCALL FableDestVal_004d6902(FableDestVal_004d6902_t* self)
{
    self->Second.~CCharString();
    self->First.~CCharString();
}
