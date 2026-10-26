// Behaviour test for CGraphicDataBankInfo::~CGraphicDataBankInfo: the bank pointer is released (data deleted at zero), then BankHandle (0x009F88C0).
#include <cstdio>
static int g_log[32]; static int g_n = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
class CCPPointerInfo { public: unsigned long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static int g_dataDeleted = 0;
static void DeleteData(void* data) { (void)data; ++g_dataDeleted; g_log[g_n++] = 50; }
template<class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    void ReleaseInfo() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};
static CCPPointerInfo* NewInfo(unsigned long refs) { CCPPointerInfo* i = new CCPPointerInfo; i->RefCount = refs; i->DeleteFunc = DeleteData; i->Data = 0; return i; }
class CGraphicDataBank; struct CGraphicDataBankInfo { unsigned char Init[0x2c]; CCharString BankHandle; CCountedPointer<CGraphicDataBank> GraphicDataBank; };
int main()
{
    CCPPointerInfo* shared = NewInfo(2);
    { CGraphicDataBankInfo a; a.BankHandle.id = 1; a.GraphicDataBank.Data = 0; a.GraphicDataBank.Info = shared; }
    bool ok1 = g_n == 1 && g_log[0] == 1 && shared->RefCount == 1 && g_dataDeleted == 0;
    g_n = 0;
    { CGraphicDataBankInfo b; b.BankHandle.id = 2; b.GraphicDataBank.Data = 0; b.GraphicDataBank.Info = shared; }
    bool ok2 = g_n == 2 && g_log[0] == 50 && g_log[1] == 2 && g_dataDeleted == 1;
    g_n = 0;
    { CGraphicDataBankInfo c; c.BankHandle.id = 3; c.GraphicDataBank.Data = 0; c.GraphicDataBank.Info = 0; }
    bool ok3 = g_n == 1 && g_log[0] == 3;
    if (!(ok1 && ok2 && ok3)) { std::printf("009f88c0_TEST FAIL\n"); return 1; }
    std::printf("009f88c0_TEST PASS\n");
    return 0;
}