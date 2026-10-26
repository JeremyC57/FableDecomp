// Leaf: DestroyMembers
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065688F, 20 bytes. Catalog name: std::_Dest_val<std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>_>,std::pair<long,CMusicManager::CRegisteredMusicEntry>_>.
// Body: destroy the member at +0x39 (CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10), then tail-call
// the +0x10 member std::vector<CConversation,std::allocator<CConversation> >::~vector @ 0x00440C40. __thiscall, this = ecx.
class CConversation;
namespace std {
template<class T> struct allocator;
template<class T, class A> struct vector {   // {first, last, end of storage}
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    ~vector();                              // vector<CConversation>: 0x00440C40
};
}
struct CBaseIntelligentPointer {            // vptr + data (8 bytes); vtable 0x0129C95C
    void* _vptr;
    void* PData;
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
#pragma pack(push, 1)
struct CObject_0065688f {
    unsigned char _lead[0x10];
    std::vector<CConversation,std::allocator<CConversation> > First;// +0x10
    unsigned char _pad[0x1d];
    CBaseIntelligentPointer Second;             // +0x39
    void DestroyMembers();
};
#pragma pack(pop)

void CObject_0065688f::DestroyMembers()
{
    Second.~CBaseIntelligentPointer();
    First.~vector();
}
