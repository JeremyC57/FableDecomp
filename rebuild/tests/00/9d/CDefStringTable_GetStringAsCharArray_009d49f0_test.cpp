// Behaviour test for CDefStringTable::GetStringAsCharArray (0x009D49F0): "" for -1, else the text 4 bytes past `index` in the stream.
#include <stdio.h>
#include <string.h>
class CMemoryDataOutputStream { public: const void* Base; const void* PeekData() const { return Base; } };
class CDefStringTable { public:
    char pad0[0xc]; CMemoryDataOutputStream m_stream;
    const char* GetStringAsCharArray(long index) const
    {
        if (index == -1) return "";
        const void* base = m_stream.PeekData();
        return (const char*)base + index + 4;
    }
};
int main()
{
    static const char data[] = "\x03\x00\x00\x00" "abc\0" "\x02\x00\x00\x00" "de";
    CDefStringTable t; t.m_stream.Base = data;
    if (strcmp(t.GetStringAsCharArray(-1), "") != 0) { printf("009d49f0_TEST FAIL\n"); return 1; }
    if (strcmp(t.GetStringAsCharArray(0), "abc") != 0) { printf("009d49f0_TEST FAIL\n"); return 1; }
    if (strcmp(t.GetStringAsCharArray(8), "de") != 0) { printf("009d49f0_TEST FAIL\n"); return 1; }
    printf("009d49f0_TEST PASS\n");
    return 0;
}
