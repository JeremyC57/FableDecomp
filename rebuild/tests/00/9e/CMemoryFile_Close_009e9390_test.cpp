// Test: CMemoryFile::Close -- clears Opened and empties the buffer (finish = start), keeping the capacity
#include <cstdio>
class CMemoryFile { public:
    unsigned char storage[8]; unsigned char* start; unsigned char* finish; unsigned char* end_of_storage; bool Opened;
    void Close() { Opened = false; finish = start; }
};
int main()
{
    CMemoryFile m; m.start = m.storage; m.finish = m.storage + 5; m.end_of_storage = m.storage + 8; m.Opened = true;
    m.Close();
    if (m.Opened || m.finish != m.start || m.end_of_storage != m.storage + 8) { std::printf("009e9390_TEST FAIL\n"); return 1; }
    std::printf("009e9390_TEST PASS\n");
    return 0;
}