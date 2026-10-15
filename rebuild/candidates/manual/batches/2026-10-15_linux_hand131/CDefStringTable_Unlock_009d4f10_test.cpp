// Behaviour test for 0x009D4F10 CDefStringTable::Unlock: a locked, writable table maps every blob record's name to its
// offset (records: 4-byte header, characters, terminator) and unlocks; unlocked or read-only tables are untouched.
#include <cstdio>
#include <cstring>
struct CEntry { char name[32]; unsigned int offset; };
struct CDefStringTable {
    CEntry m_map[8]; int m_count;
    const char* m_data; unsigned int m_size;
    bool Locked, ReadOnly;
    unsigned int& Lookup(const char* n)
    {
        for (int i = 0; i < m_count; ++i) if (!std::strcmp(m_map[i].name, n)) return m_map[i].offset;
        std::strcpy(m_map[m_count].name, n); m_map[m_count].offset = 0; return m_map[m_count++].offset;
    }
    void Unlock()
    {
        if (!Locked) return;
        if (ReadOnly) return;
        for (unsigned int offset = 4; offset < m_size; ) {
            const char* name = m_data + offset + 4;
            Lookup(name) = offset;
            offset += (unsigned int)std::strlen(name) + 5;
        }
        Locked = false;
    }
};
int main()
{
    // header(4) | rec@4: hdr(4) "AB\0" | rec@11: hdr(4) "XYZ\0"
    const char blob[] = "HDR\0hhhhAB\0hhhhXYZ";
    CDefStringTable t; t.m_count = 0; t.m_data = blob; t.m_size = sizeof(blob); t.Locked = true; t.ReadOnly = true;
    t.Unlock();
    if (t.m_count != 0 || !t.Locked) { std::printf("009d4f10_TEST FAIL\n"); return 1; }
    t.ReadOnly = false;
    t.Unlock();
    if (t.m_count != 2 || t.Locked || std::strcmp(t.m_map[0].name, "AB") || t.m_map[0].offset != 4 || std::strcmp(t.m_map[1].name, "XYZ") || t.m_map[1].offset != 11)
        { std::printf("009d4f10_TEST FAIL\n"); return 1; }
    std::printf("009d4f10_TEST PASS\n");
    return 0;
}
