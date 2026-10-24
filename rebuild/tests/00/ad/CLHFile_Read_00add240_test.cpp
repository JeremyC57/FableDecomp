// Behaviour test for 0x00ADD240 CLHFile::Read (descriptor built from block size, priority by the WMA flag, queued once).
#include <cstdio>

struct SDesc { void* PDst; unsigned long FilePos; unsigned long Size; void* PCallback; bool QueueWhenFinished; int Priority;
               SDesc() : PDst(0), FilePos(0), Size(0), PCallback(0), QueueWhenFinished(true), Priority(3) { } };
static SDesc g_queued; static int g_queueCount = 0;
struct SThreadedFile { bool OpenFlag; void AddObject(const SDesc& d) { g_queued = d; ++g_queueCount; } };
struct SCallback { void* Client; void* User; SCallback(void* c, void* u) : Client(c), User(u) { } };
static int g_streamed = 4, g_normal = 3;
struct SLHFile {
    SThreadedFile* m_pFile; bool m_bIsWMA; long m_blockSize;
    long GetBlockSize() { return m_blockSize; }
    // reconstructed body
    bool Read(unsigned long block, unsigned long count, unsigned long, void* buffer, void* user, void* client)
    {
        SThreadedFile* file = m_pFile;
        if (file && file->OpenFlag) {
            SDesc desc;
            desc.PCallback = new SCallback(client, user);
            desc.FilePos = GetBlockSize() * block;
            desc.PDst = buffer;
            desc.Size = GetBlockSize() * count;
            desc.QueueWhenFinished = false;
            if (m_bIsWMA) desc.Priority = g_streamed; else desc.Priority = g_normal;
            m_pFile->AddObject(desc);
            return true;
        }
        return false;
    }
};

int main()
{
    SThreadedFile tf; tf.OpenFlag = true;
    SLHFile f; f.m_pFile = &tf; f.m_bIsWMA = false; f.m_blockSize = 0x800;
    char buf[16]; int client = 0, user = 0;
    if (!f.Read(3, 2, 0, buf, &user, &client) || g_queueCount != 1) { std::printf("00add240_TEST FAIL\n"); return 1; }
    SCallback* cb = (SCallback*)g_queued.PCallback;
    if (g_queued.PDst != buf || g_queued.FilePos != 0x1800 || g_queued.Size != 0x1000 || g_queued.QueueWhenFinished || g_queued.Priority != 3 || cb->Client != &client || cb->User != &user) { std::printf("00add240_TEST FAIL\n"); return 1; }
    delete cb;
    f.m_bIsWMA = true; f.m_blockSize = 0x200;
    if (!f.Read(1, 1, 0, buf, 0, &client) || g_queued.Priority != 4 || g_queued.FilePos != 0x200) { std::printf("00add240_TEST FAIL\n"); return 1; }
    delete (SCallback*)g_queued.PCallback;
    tf.OpenFlag = false;
    if (f.Read(1, 1, 0, buf, 0, &client) || g_queueCount != 2) { std::printf("00add240_TEST FAIL\n"); return 1; }
    f.m_pFile = 0;
    if (f.Read(1, 1, 0, buf, 0, &client) || g_queueCount != 2) { std::printf("00add240_TEST FAIL\n"); return 1; }
    std::printf("00add240_TEST PASS\n");
    return 0;
}