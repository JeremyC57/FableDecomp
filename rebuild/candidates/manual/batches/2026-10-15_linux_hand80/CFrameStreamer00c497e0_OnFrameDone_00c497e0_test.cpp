#include <cstdio>
#include <cstring>

static int g_frees = 0; static void* g_freed = 0;
static void FreeBlock(void* block) { ++g_frees; g_freed = block; }
struct SFrame { int id; void* m_pData; };
struct CFrameStack { SFrame* m_next; SFrame* Pop() { SFrame* f = m_next; m_next = 0; return f; } };
struct CMailboxQueue { SFrame* m_posted; int m_count; long ProcessMessage(void* message) { m_posted = *(SFrame**)message; ++m_count; return 0; } };
struct CFrameStreamer {
    CMailboxQueue m_doneQueue; CFrameStack m_freeFrames;
    static void OnFrameDone(void*, void*, CFrameStreamer* streamer)
    {
        SFrame* frame = streamer->m_freeFrames.Pop();
        if (frame) {
            FreeBlock(frame->m_pData);
            frame->m_pData = 0;
            streamer->m_doneQueue.ProcessMessage(&frame);
        }
    }
};
int main()
{
    static char block[16];
    SFrame f; f.id = 4; f.m_pData = block;
    CFrameStreamer s; s.m_doneQueue.m_posted = 0; s.m_doneQueue.m_count = 0; s.m_freeFrames.m_next = &f;
    CFrameStreamer::OnFrameDone(0, 0, &s);
    if (g_frees != 1 || g_freed != block || f.m_pData != 0 || s.m_doneQueue.m_count != 1 || s.m_doneQueue.m_posted != &f) { std::printf("00c497e0_TEST FAIL\n"); return 1; }
    CFrameStreamer::OnFrameDone(0, 0, &s);
    if (g_frees != 1 || s.m_doneQueue.m_count != 1) { std::printf("00c497e0_TEST FAIL\n"); return 1; }
    std::printf("00c497e0_TEST PASS\n");
    return 0;
}
