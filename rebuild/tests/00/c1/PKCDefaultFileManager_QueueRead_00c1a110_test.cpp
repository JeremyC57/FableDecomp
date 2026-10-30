// Behaviour test for 0x00C1A110 PKCDefaultFileManager::QueueRead: unknown key -> false (nothing posted); known key ->
// pending count +1 and the seven-word request posted; a failed post -> pending count restored and false.
#include <cstdio>

static int g_lockDepth, g_locks;
struct PKCCriticalSection { int dummy; };
struct PKCProtectionInstance {
    PKCCriticalSection* m_pSection;
    PKCProtectionInstance(PKCCriticalSection* s) : m_pSection(s) { ++g_lockDepth; ++g_locks; }
    ~PKCProtectionInstance() { --g_lockDepth; }
};
struct SFileRequest { void* m_hFile; unsigned long m_key; unsigned long m_size; void* m_a; void* m_b; void* m_c; void* m_d; };
struct SOpenFile { void* m_hFile; long m_pendingRequests; };

struct PKCTRRedblackNode { PKCTRRedblackNode* Left; PKCTRRedblackNode* Right; PKCTRRedblackNode* Parent; long Colour; unsigned long Key; void* Value; };
struct PKCTRRedblackMap {
    PKCTRRedblackNode Sentinel; PKCTRRedblackNode* Root; PKCTRRedblackNode* LastFound;
    long Find(const unsigned long* key, void** value)
    {
        PKCTRRedblackNode* node = Root;
        while (node != &Sentinel) {
            if (*key == node->Key) { *value = node->Value; LastFound = node; return 0; }
            if (*key < node->Key) node = node->Left; else node = node->Right;
        }
        return 3;
    }
};
struct PKCMailbox {
    long m_result; int m_sends; SFileRequest m_last; int m_lockDepthAtSend;
    long Send(const SFileRequest* r) { ++m_sends; m_last = *r; m_lockDepthAtSend = g_lockDepth; return m_result; }
};
struct PKCDefaultFileManager {
    PKCCriticalSection m_lock; PKCTRRedblackMap m_files; PKCMailbox m_mailbox;
    bool QueueRead(unsigned long key, unsigned long size, void* a, void* b, void* c, void* d)
    {
        SFileRequest request;
        {
            PKCProtectionInstance lock(&m_lock);
            SOpenFile* file;
            if (m_files.Find(&key, (void**)&file) != 0)
                return false;
            request.m_hFile = file->m_hFile;
            ++file->m_pendingRequests;
        }
        request.m_key = key; request.m_size = size; request.m_a = a; request.m_b = b; request.m_c = c; request.m_d = d;
        if (m_mailbox.Send(&request) != 0) {
            PKCProtectionInstance lock(&m_lock);
            SOpenFile* file;
            if (m_files.Find(&key, (void**)&file) == 0)
                --file->m_pendingRequests;
            return false;
        }
        return true;
    }
};

int main()
{
    int bad = 0, x1, x2, x3, x4;
    static PKCDefaultFileManager m;
    SOpenFile f5 = { (void*)0x55, 0 }, f9 = { (void*)0x99, 2 };
    PKCTRRedblackNode n5, n9;
    n5.Left = &m.m_files.Sentinel; n5.Right = &n9; n5.Key = 5; n5.Value = &f5;
    n9.Left = &m.m_files.Sentinel; n9.Right = &m.m_files.Sentinel; n9.Key = 9; n9.Value = &f9;
    m.m_files.Root = &n5; m.m_mailbox.m_result = 0;
    if (m.QueueRead(7, 100, &x1, &x2, &x3, &x4) || m.m_mailbox.m_sends != 0 || g_lockDepth != 0) bad = 1;
    if (!m.QueueRead(9, 0x800, &x1, &x2, &x3, &x4) || f9.m_pendingRequests != 3 || m.m_mailbox.m_sends != 1) bad = 1;
    SFileRequest& r = m.m_mailbox.m_last;
    if (r.m_hFile != (void*)0x99 || r.m_key != 9 || r.m_size != 0x800 || r.m_a != &x1 || r.m_b != &x2 || r.m_c != &x3
        || r.m_d != &x4 || m.m_mailbox.m_lockDepthAtSend != 0 || m.m_files.LastFound != &n9) bad = 1;
    m.m_mailbox.m_result = 1;                           // queue full: the pending count is given back
    if (m.QueueRead(5, 1, 0, 0, 0, 0) || f5.m_pendingRequests != 0 || m.m_mailbox.m_sends != 2 || g_lockDepth != 0) bad = 1;
    if (g_locks != 4) bad = 1;
    if (bad) { std::printf("00c1a110_TEST FAIL\n"); return 1; }
    std::printf("00c1a110_TEST PASS\n");
    return 0;
}