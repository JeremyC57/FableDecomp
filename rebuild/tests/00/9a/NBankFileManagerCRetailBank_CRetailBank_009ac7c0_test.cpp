// Behaviour test for NBankFileManager::CRetailBank::~CRetailBank (0x009AC7C0): members torn down in reverse order -- FilePath,
// the contained-bank map (cleared when non-empty, header reset and freed), ThreadedFile, DiskFile (released, deleted on the last
// reference) -- then the root destructor.
#include <cstdio>
#include <cstring>
static char g_log[64];
static int g_logLen;
static void Log(char c) { if (g_logLen < 63) { g_log[g_logLen++] = c; g_log[g_logLen] = 0; } }
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer { public:                     // engine/CCountedPointer.h destructor
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    void Share(T* data, CCPPointerInfo* info) { Data = data; Info = info; ++info->RefCount; }
};
struct CPayload { char Tag; };
static void DeletePayload(void* data) { Log(((CPayload*)data)->Tag); }

struct CNode { CNode* Parent; CNode* Left; CNode* Right; };
struct CTree {                                                   // STLport _Rb_tree: header, node count
    CNode* Header;
    unsigned int NodeCount;
    CNode* ErasedRoot;
    ~CTree()
    {
        if (NodeCount != 0) {
            ErasedRoot = Header->Parent; Log('e');               // _M_erase(_M_root())
            Header->Left = Header; Header->Parent = 0; Header->Right = Header; NodeCount = 0;
        }
        if (Header) { Log('f'); delete Header; }                 // ~_Rb_tree_base: deallocate the header
    }
};
class CCharString { public: ~CCharString() { Log('s'); } };
struct CBase { ~CBase() { Log('b'); } };
struct CRetailBank : CBase {
    CCountedPointer<CPayload> DiskFile;
    CCountedPointer<CPayload> ThreadedFile;
    bool UseDMA;
    CTree ContainedBanks;
    CCharString FilePath;
};
int main()
{
    CPayload disk = { 'D' }, threaded = { 'T' };
    CCPPointerInfo diskInfo = { 0, DeletePayload, &disk };
    CCPPointerInfo* threadedInfo = new CCPPointerInfo;
    threadedInfo->RefCount = 1; threadedInfo->DeleteFunc = DeletePayload; threadedInfo->Data = &threaded;
    CNode root = { 0, 0, 0 };
    {
        CRetailBank* bank = new CRetailBank;
        bank->DiskFile.Share(&disk, &diskInfo);                  // shared: count 1 -> 2, not deleted
        diskInfo.RefCount = 2;
        bank->ThreadedFile.Data = &threaded; bank->ThreadedFile.Info = threadedInfo;   // sole owner
        bank->ContainedBanks.Header = new CNode; bank->ContainedBanks.Header->Parent = &root;
        bank->ContainedBanks.NodeCount = 3;
        delete bank;
    }
    if (std::strcmp(g_log, "sefTb") != 0 || diskInfo.RefCount != 1) { std::printf("009ac7c0_TEST FAIL\n"); return 1; }
    std::printf("009ac7c0_TEST PASS\n");
    return 0;
}