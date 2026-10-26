// Behaviour test for 009fd7f0 CGraphicInfoFrame::CFrameData::IsResourceLoaded (copy of the source model).
#include <cstdio>

static int g_calls = 0;
class CBankFileAsyncData { public:
    bool m_finished;
    bool IsFinished() { ++g_calls; return m_finished; }
};
struct CCountedPointer_BankFileAsyncData {
    CBankFileAsyncData* m_pObject;
    void*               m_pCount;
};
struct CFrameData {
    unsigned char                     _pad[0x30];
    CCountedPointer_BankFileAsyncData FileData;
    bool IsResourceLoaded() const;
};

bool CFrameData::IsResourceLoaded() const
{
    if (FileData.m_pObject && !FileData.m_pObject->IsFinished())
        return false;
    return true;
}

int main()
{
    CFrameData f;
    f.FileData.m_pObject = 0; f.FileData.m_pCount = 0;
    int ok = 1;
    if (!f.IsResourceLoaded() || g_calls != 0) ok = 0;          // no read pending
    CBankFileAsyncData read; read.m_finished = false;
    f.FileData.m_pObject = &read;
    if (f.IsResourceLoaded() || g_calls != 1) ok = 0;           // still reading
    read.m_finished = true;
    if (!f.IsResourceLoaded() || g_calls != 2) ok = 0;          // finished
    if (!ok) { std::printf("009fd7f0_TEST FAIL\n"); return 1; }
    std::printf("009fd7f0_TEST PASS\n");
    return 0;
}