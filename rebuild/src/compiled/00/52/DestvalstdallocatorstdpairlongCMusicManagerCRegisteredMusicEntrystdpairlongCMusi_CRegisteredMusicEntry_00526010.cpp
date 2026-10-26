// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00526010, 19 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0x4 (CWideString::~CWideString @ 0x0099B510), then tail-call
// the +0x0 member CCharString::~CCharString @ 0x0099EAE0. __thiscall, this = ecx.
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
class CWideString { public:                        // one word: CWideStringData*
    void* PStringData;
    ~CWideString();                         // 0x0099B510 (NoRefs at data+0xc, free(), instance count 0x013BCA20)
};
#pragma pack(push, 1)
struct CObject_00526010 {
    CCharString First;                          // +0x0
    CWideString Second;                         // +0x4
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_00526010::DestroyMembers()
{
    Second.~CWideString();
    First.~CCharString();
}
