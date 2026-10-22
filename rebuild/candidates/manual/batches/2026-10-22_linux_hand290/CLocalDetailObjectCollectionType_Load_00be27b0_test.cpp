// Behaviour test for CLocalDetailObjectCollectionType::Load (0x00BE27B0): binary ids or text names, the 0x34-byte block,
// then the three ids resolved through the bank (the kind-1 getter for the first id when block word 9 is 1).
#include <cstdio>
#include <cstring>

struct Bank {
    int animatedCalls;
    long FindEntry(const char* name) { return name[0] == 'x' ? 0 : (long)std::strlen(name); }
    long GetEntry(long i) { return 100 + i; }
    long GetAnimatedEntry(long i) { ++animatedCalls; return 200 + i; }
};
static Bank g_bank;

struct Stream {
    const unsigned char* data;
    long remaining;
    unsigned long position;
    const char* names[3];
    int nameIndex;
    void Read(void* out, long size)
    {
        if (remaining >= size) { std::memcpy(out, data, size); data += size; remaining -= size; position += size; }
    }
    long ReadLong() { long v = 0; Read(&v, 4); return v; }
    long ReadIndex()
    {
        const char* n = names[nameIndex++];
        if (n == 0 || n[0] == 0) return 0;
        return g_bank.FindEntry(n);
    }
};

struct Coll {
    long m_4, m_8, m_c, m_10, m_14, m_18, m_1c;
    int block[13];
    void Load(Stream& s, bool binary)
    {
        if (binary) { m_14 = s.ReadLong(); m_1c = s.ReadLong(); m_18 = s.ReadLong(); }
        else { m_14 = s.ReadIndex(); m_1c = s.ReadIndex(); m_18 = s.ReadIndex(); }
        s.Read(block, 0x34);
        if (m_14 > 0) m_4 = block[9] == 1 ? g_bank.GetAnimatedEntry(m_14) : g_bank.GetEntry(m_14);
        if (m_1c > 0) m_c = g_bank.GetEntry(m_1c);
        if (m_18 > 0) m_8 = g_bank.GetEntry(m_18);
    }
};

int main()
{
    int buf[3 + 13];
    for (int i = 0; i < 16; ++i) buf[i] = 0;
    buf[0] = 5; buf[1] = 0; buf[2] = 7; buf[3 + 9] = 1;
    Stream s; s.data = (const unsigned char*)buf; s.remaining = sizeof(buf); s.position = 0; s.nameIndex = 0;
    Coll c; std::memset(&c, 0, sizeof(c));
    g_bank.animatedCalls = 0;
    c.Load(s, true);
    if (c.m_14 != 5 || c.m_1c != 0 || c.m_18 != 7 || c.m_4 != 205 || c.m_c != 0 || c.m_8 != 107 || c.block[9] != 1 || s.remaining != 0) {
        std::printf("00be27b0_TEST FAIL\n"); return 1;
    }
    buf[3 + 9] = 0;
    Stream t; t.data = (const unsigned char*)(buf + 3); t.remaining = 0x34; t.position = 0; t.nameIndex = 0;
    t.names[0] = "abc"; t.names[1] = ""; t.names[2] = "xy";
    Coll d; std::memset(&d, 0, sizeof(d));
    d.Load(t, false);
    if (d.m_14 != 3 || d.m_1c != 0 || d.m_18 != 0 || d.m_4 != 103 || d.m_8 != 0 || g_bank.animatedCalls != 1) {
        std::printf("00be27b0_TEST FAIL\n"); return 1;
    }
    std::printf("00be27b0_TEST PASS\n");
    return 0;
}
