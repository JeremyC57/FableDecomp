#include <cstdio>
struct CThreadedFileObjectDesc { unsigned char _body[0x18]; };
class CThreadedFile;
class CThreadedFileManager {
public:
    const CThreadedFileObjectDesc* m_lastDesc;
    CThreadedFile* m_lastFile;
    unsigned long m_nextUID;
    unsigned long AddObject(const CThreadedFileObjectDesc& desc, CThreadedFile* file)
    {
        m_lastDesc = &desc; m_lastFile = file; return m_nextUID++;
    }
};
CThreadedFileManager g_ThreadedFileManager;
class CThreadedFile {
public:
    void* m_vtable;
    long  DeviceID;
    unsigned long AddObject(const CThreadedFileObjectDesc& desc);
};
unsigned long CThreadedFile::AddObject(const CThreadedFileObjectDesc& desc)
{
    return g_ThreadedFileManager.AddObject(desc, this);
}
int main()
{
    g_ThreadedFileManager.m_nextUID = 100;
    CThreadedFile file; file.m_vtable = 0; file.DeviceID = 1;
    CThreadedFileObjectDesc d1, d2;
    unsigned long u1 = file.AddObject(d1);
    if (u1 != 100 || g_ThreadedFileManager.m_lastDesc != &d1 || g_ThreadedFileManager.m_lastFile != &file) { std::printf("0098f350_TEST FAIL\n"); return 1; }
    unsigned long u2 = file.AddObject(d2);
    if (u2 != 101 || g_ThreadedFileManager.m_lastDesc != &d2) { std::printf("0098f350_TEST FAIL\n"); return 1; }
    std::printf("0098f350_TEST PASS\n");
    return 0;
}