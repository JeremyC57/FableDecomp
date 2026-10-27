#include <cstdio>

// Behaviour model of CVtable_012b4f4c::GetSize (0x00C32F40): the manager's GetFileSize for this handle's key.
struct Manager { bool GetFileSize(unsigned int key, unsigned int* size) { if (key != 7) return false; *size = 1234; return true; } };
struct FileHandle { Manager* m_pManager; unsigned int m_key; bool GetSize(unsigned int* size) { return m_pManager->GetFileSize(m_key, size); } };

int main()
{

    Manager m; FileHandle h; h.m_pManager = &m; h.m_key = 7; unsigned int size = 0;
    if (!h.GetSize(&size) || size != 1234) { std::printf("00c32f40_TEST FAIL\n"); return 1; }
    h.m_key = 8;
    if (h.GetSize(&size)) { std::printf("00c32f40_TEST FAIL\n"); return 1; }
    std::printf("00c32f40_TEST PASS\n");
    return 0;
}