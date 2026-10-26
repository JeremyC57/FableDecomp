#include <cstdio>
#include <cstring>
// Behaviour model of CSkeletalMorphs::TransferBinaryIn (0x0081AED0): count, resize to count default
// pairs {0, name -1}, then per entry key (4), name table pos (4), short index (2), enabled byte (1); Dirty cleared.
struct Stream {
    const unsigned char* data;
    int remaining;
    void Read(void* buffer, int size)
    {
        if (remaining >= size) { std::memcpy(buffer, data, size); data += size; remaining -= size; }
    }
    unsigned int ReadULong() { unsigned int value = 0; Read(&value, sizeof(value)); return value; }
};
class CDefString { public:
    int TablePos;
    CDefString() : TablePos(-1) {}
    void TransferBinaryIn(Stream* stream) { int v = 0; stream->Read(&v, 4); TablePos = v; }
};
struct CEntry { CDefString Name; short m_index; bool m_enabled; };
struct CMorphPair { unsigned int first; CEntry second; CMorphPair() : first(0) {} };
struct CMorphVector {
    CMorphPair items[8];
    unsigned int size;
    CMorphPair* _M_start;
    void resize(unsigned int count, const CMorphPair& value)
    {
        for (unsigned int i = size; i < count; ++i) items[i] = value;
        size = count; _M_start = items;
    }
};
class CSkeletalMorphs { public:
    CMorphVector Morphs;
    bool Dirty;
    void TransferBinaryIn(Stream* stream)
    {
        unsigned int count = stream->ReadULong();
        Morphs.resize(count, CMorphPair());
        for (unsigned int i = 0; i < count; ++i) {
            Morphs._M_start[i].first = stream->ReadULong();
            CEntry& entry = Morphs._M_start[i].second;
            entry.Name.TransferBinaryIn(stream);
            short index = 0;
            stream->Read(&index, sizeof(index));
            entry.m_index = index;
            unsigned char enabled = 0;
            stream->Read(&enabled, sizeof(enabled));
            entry.m_enabled = enabled != 0;
        }
        Dirty = false;
    }
};
static void put32(unsigned char* p, unsigned int v) { std::memcpy(p, &v, 4); }
int main()
{
    unsigned char buf[64];
    int n = 0;
    put32(buf + n, 2); n += 4;
    put32(buf + n, 77); n += 4; put32(buf + n, 5); n += 4;
    short s1 = 300; std::memcpy(buf + n, &s1, 2); n += 2; buf[n++] = 9;
    put32(buf + n, 1234); n += 4; put32(buf + n, 6); n += 4;
    short s2 = -4; std::memcpy(buf + n, &s2, 2); n += 2; buf[n++] = 0;
    Stream st = { buf, n };
    CSkeletalMorphs m; m.Morphs.size = 0; m.Morphs._M_start = 0; m.Dirty = true;
    m.TransferBinaryIn(&st);
    bool ok = m.Morphs.size == 2 && !m.Dirty && st.remaining == 0
        && m.Morphs.items[0].first == 77 && m.Morphs.items[0].second.Name.TablePos == 5
        && m.Morphs.items[0].second.m_index == 300 && m.Morphs.items[0].second.m_enabled
        && m.Morphs.items[1].first == 1234 && m.Morphs.items[1].second.Name.TablePos == 6
        && m.Morphs.items[1].second.m_index == -4 && !m.Morphs.items[1].second.m_enabled;
    // empty record: zero count, no entries, map still marked clean
    unsigned char zero[4] = { 0, 0, 0, 0 };
    Stream st0 = { zero, 4 };
    CSkeletalMorphs e; e.Morphs.size = 0; e.Morphs._M_start = 0; e.Dirty = true;
    e.TransferBinaryIn(&st0);
    ok = ok && e.Morphs.size == 0 && !e.Dirty;
    if (!ok) { std::printf("0081aed0_TEST FAIL\n"); return 1; }
    std::printf("0081aed0_TEST PASS\n");
    return 0;
}