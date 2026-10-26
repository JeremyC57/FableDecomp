// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043F3A0, 19 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0x10 (CCharString::~CCharString @ 0x0099EAE0), then tail-call
// the +0x0 member CWideString::~CWideString @ 0x0099B510. __thiscall, this = ecx.
class CWideString { public:                        // one word: CWideStringData*
    void* PStringData;
    ~CWideString();                         // 0x0099B510 (NoRefs at data+0xc, free(), instance count 0x013BCA20)
};
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
#pragma pack(push, 1)
struct CObject_0043f3a0 {
    CWideString First;                          // +0x0
    unsigned char _pad[0xc];
    CCharString Second;                         // +0x10
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_0043f3a0::DestroyMembers()
{
    Second.~CCharString();
    First.~CWideString();
}
