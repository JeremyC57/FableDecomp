// Behaviour test for 00c1a070: CFileManager::GetFileSize -- under the manager's lock, looks the key up in the open-file map; false when absent, else stores the Win32 file size and returns false only for INVALID_FILE_SIZE
#include <cstdio>

static int g_locks = 0, g_unlocks = 0, g_heldDuringQuery = 0;
struct CScopedLock {
    CScopedLock(void*) { ++g_locks; }
    ~CScopedLock() { ++g_unlocks; }
};

static unsigned int FakeGetFileSize(unsigned int hFile, unsigned int* sizeHigh)
{
    if (sizeHigh) return 0;                // never asked for the high word
    g_heldDuringQuery = g_locks - g_unlocks;
    return hFile == 0xDEAD ? 0xFFFFFFFFu : hFile * 3;
}

struct SOpenFile { unsigned int m_hFile; };

struct Node { Node* Left; Node* Right; Node* Parent; long Colour; unsigned int Key; void* Value; };
struct COpenFileMap {
    void* PAllocator;
    Node  Sentinel;
    Node* Root;
    Node* LastFound;
    long Find(const unsigned int* key, void** value)
    {
        Node* node = Root;
        while (node != &Sentinel) {
            if (*key == node->Key) { *value = node->Value; LastFound = node; return 0; }
            node = (*key < node->Key) ? node->Left : node->Right;
        }
        return 3;
    }
};

struct CFileManager {
    unsigned char m_section[0x18];
    COpenFileMap m_files;
    bool GetFileSize(unsigned int key, unsigned int* size)
    {
        CScopedLock lock(m_section);
        SOpenFile* file;
        if (m_files.Find(&key, (void**)&file) != 0)
            return false;
        unsigned int result = FakeGetFileSize(file->m_hFile, 0);
        *size = result;
        if (result == 0xFFFFFFFFu)
            return false;
        return true;
    }
};

int main()
{
    CFileManager m;
    SOpenFile good = { 100 }, bad = { 0xDEAD };
    Node n1, n2;
    m.m_files.Sentinel.Left = m.m_files.Sentinel.Right = &m.m_files.Sentinel;
    n1.Key = 5; n1.Value = &good; n1.Left = &m.m_files.Sentinel; n1.Right = &n2; n1.Parent = 0;
    n2.Key = 9; n2.Value = &bad;  n2.Left = n2.Right = &m.m_files.Sentinel; n2.Parent = &n1;
    m.m_files.Root = &n1;
    unsigned int size = 7;
    if (!m.GetFileSize(5, &size) || size != 300 || g_heldDuringQuery != 1) { std::printf("00c1a070_TEST FAIL\n"); return 1; }
    size = 7;
    if (m.GetFileSize(9, &size) || size != 0xFFFFFFFFu) { std::printf("00c1a070_TEST FAIL\n"); return 1; }   // stored, then false
    size = 7;
    if (m.GetFileSize(6, &size) || size != 7) { std::printf("00c1a070_TEST FAIL\n"); return 1; }             // absent: untouched
    if (g_locks != 3 || g_unlocks != 3) { std::printf("00c1a070_TEST FAIL\n"); return 1; }
    std::printf("00c1a070_TEST PASS\n");
    return 0;
}