// CGraphicBankManager::CGraphicDataBankInfo::~CGraphicDataBankInfo() -- release the bank's counted CGraphicDataBank pointer, then destroy the BankHandle string
// Leaf: ??1CGraphicDataBankInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F88C0, length 61 bytes, `jmp` tail; no call or address references (an
// unreferenced COMDAT among the CGraphicBankManager bodies 0x009F86C0..0x009F8A20; the landed ??_G 0x009F87C0 inlines the
// same body). The manifest label (_Dest_val<allocator<pair<CCharString const,CCountedPointer<C3DSkeletalMorphTarget>>>>) is
// propagated over the shape and wrong.
// Layout: Ego_r.pdb CGraphicDataBankInfo (0x38) = CGraphicDataBankInit Init +0 (0x2c, trivially destructible), CCharString
// BankHandle +0x2c, CCountedPointer<CGraphicDataBank> GraphicDataBank +0x30 -- the landed ??_G's model.
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete @ 0x00BFE9BC (the block),
// CCharString::~CCharString @ 0x0099EAE0 (tail).
// Shape: the implicit destructor emitted through a qualified explicit destructor call; ~CCountedPointer re-reads Info after
// the decrement, frees the block with an untested operator delete, then zeroes Data and Info; the string is the tail jmp.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};

struct CCPPointerInfo {                                    // Ego_r.pdb: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);               // called with Data in ecx, nothing on the stack
    void* Data;
};
template<class T> struct CCountedPointer {
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};

struct CGraphicDataBank;
struct CGraphicDataBankInit { unsigned char _bytes[0x2c]; }; // pixel formats and flags, trivially destructible

class CGraphicBankManager {
public:
    struct CGraphicDataBankInfo {                            // 0x38 bytes
        CGraphicDataBankInit              Init;              // +0x00
        CCharString                       BankHandle;        // +0x2c
        CCountedPointer<CGraphicDataBank> GraphicDataBank;   // +0x30
    };
};

void DestroyGraphicDataBankInfo(CGraphicBankManager::CGraphicDataBankInfo* info)   // emits the implicit destructor; not part of the retail body
{
    info->CGraphicBankManager::CGraphicDataBankInfo::~CGraphicDataBankInfo();
}