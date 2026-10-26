// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EA320, 19 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0x4 (CCharString::~CCharString @ 0x0099EAE0), then tail-call
// the base CBaseClass::~CBaseClass @ 0x0099A2E0. __thiscall, this = ecx.
// The +0x0 callee is a polymorphic class destructor, so this is a derived class's destructor body;
// the class itself is not attributed and is named by address.
class CBaseClass { public:                         // vtable 0x01231710 (.?AVCBaseClass@@)
    virtual ~CBaseClass();                          // 0x0099A2E0
};
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
#pragma pack(push, 1)
struct CObject_009ea320 : CBaseClass {
    CCharString Second;                         // +0x4
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_009ea320::DestroyMembers()
{
    Second.~CCharString();
    this->CBaseClass::~CBaseClass();
}
