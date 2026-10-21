#include <cstdio>
class CCreatureActionBase;
struct CCPPointerInfo {
    unsigned long RefCount;                              // +0x00
    void (__fastcall* DeleteFunc)(void* data);           // +0x04
    void*         Data;                                  // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    void Reset()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
class CBrainOutput {
public:
    unsigned long                        Types;          // +0x00
    long                                 LookFlags;      // +0x04
    unsigned char                        NavigationInput[0x18];  // +0x08 CNavigationInput
    CCountedPointer<CCreatureActionBase> PAction;        // +0x20
    void Clear();
};
void CBrainOutput::Clear()
{
    Types = 0;
    LookFlags = 0;
    PAction.Reset();
}

static int g_deleted; static void* g_data;
static void __fastcall DeleteIt(void* p) { ++g_deleted; g_data = p; }
int main()
{
    int obj;
    CBrainOutput b; b.Types = 5; b.LookFlags = 6;
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 2; info->DeleteFunc = DeleteIt; info->Data = &obj;
    b.PAction.Data = (CCreatureActionBase*)&obj; b.PAction.Info = info;
    b.Clear();
    bool ok = b.Types == 0 && b.LookFlags == 0 && b.PAction.Data == 0 && b.PAction.Info == 0 && info->RefCount == 1 && g_deleted == 0;
    b.PAction.Data = (CCreatureActionBase*)&obj; b.PAction.Info = info;
    b.Clear();
    ok = ok && g_deleted == 1 && g_data == &obj && b.PAction.Info == 0;
    b.Types = 1; b.Clear();
    ok = ok && b.Types == 0 && g_deleted == 1;
    std::printf(ok ? "00835330_TEST PASS\n" : "FAIL\n");
    return 0;
}