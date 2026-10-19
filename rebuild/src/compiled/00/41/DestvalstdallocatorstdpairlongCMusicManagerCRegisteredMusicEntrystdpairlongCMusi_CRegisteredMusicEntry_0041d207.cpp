// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041D207, 20 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0x60 (CList_0042a141::~CList_0042a141 @ 0x0042A141), then tail-call
// the +0x5c member CList_0042a168::~CList_0042a168 @ 0x0042A168. __thiscall, this = ecx.
struct CList_0042a168 {                     // unattributed STLport list (one word: the sentinel node; nodes and sentinel freed with free)
    void* _M_node;
    ~CList_0042a168();                      // 0x0042A168 (catalog label propagated from another body)
};
struct CList_0042a141 {                     // unattributed STLport list (one word: the sentinel node; nodes and sentinel freed with free)
    void* _M_node;
    ~CList_0042a141();                      // 0x0042A141 (catalog label propagated from another body)
};
#pragma pack(push, 1)
struct CObject_0041d207 {
    unsigned char _lead[0x5c];
    CList_0042a168 First;                       // +0x5c
    CList_0042a141 Second;                      // +0x60
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_0041d207::DestroyMembers()
{
    Second.~CList_0042a141();
    First.~CList_0042a168();
}
