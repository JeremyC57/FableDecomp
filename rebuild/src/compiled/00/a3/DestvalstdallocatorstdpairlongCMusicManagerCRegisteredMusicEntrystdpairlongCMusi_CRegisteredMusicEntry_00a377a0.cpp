// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A377A0, 19 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0x10 (CCharString::~CCharString @ 0x0099EAE0), then tail-call
// the +0x0 member CList_00a376a0::~CList_00a376a0 @ 0x00A376A0. __thiscall, this = ecx.
struct CList_00a376a0 {                     // unattributed STLport list (one word: the sentinel node; nodes and sentinel freed with free)
    void* _M_node;
    ~CList_00a376a0();                      // 0x00A376A0 (catalog label propagated from another body)
};
struct CCharString {                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
#pragma pack(push, 1)
struct CObject_00a377a0 {
    CList_00a376a0 First;                       // +0x0
    unsigned char _pad[0xc];
    CCharString Second;                         // +0x10
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_00a377a0::DestroyMembers()
{
    Second.~CCharString();
    First.~CList_00a376a0();
}
