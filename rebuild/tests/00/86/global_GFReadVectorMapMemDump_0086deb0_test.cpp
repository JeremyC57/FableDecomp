// Behaviour test for GFReadVectorMapMemDump (0x0086DEB0): count, skipped byte size, resize, raw entry bytes, Dirty flag.
#include <cstdio>
#include <cstring>

struct Stream {
    const unsigned char* data;
    long position, length;
    void Read(void* out, long size)
    {
        if (size > 0) {
            if (position + size > length) return;
            std::memcpy(out, data + position, size);
            position += size;
        }
    }
    long ReadLong() { unsigned char b[4] = { 0, 0, 0, 0 }; Read(b, 4); return (long)(b[0] | (b[1] << 8) | (b[2] << 16) | ((unsigned long)b[3] << 24)); }
    bool ReadBool() { unsigned char v = 0; Read(&v, 1); return v != 0; }
};
struct CTextureMorph { unsigned int NewTexture, GroupID; CTextureMorph() : NewTexture(0), GroupID(0) { } };
struct Pair { unsigned int first; CTextureMorph second; Pair() : first(0), second() { } };
struct VectorMap {
    Pair items[8];
    unsigned int count;
    bool Dirty;
    unsigned int size() const { return count; }
    void resize(unsigned int n, const Pair& x) { for (unsigned int i = count; i < n; ++i) items[i] = x; count = n; }
    void* GetData() { return size() ? (void*)items : (void*)this; }
};
static void GFReadVectorMapMemDump(Stream& stream, VectorMap& map)
{
    long count = stream.ReadLong();
    long bytes = stream.ReadLong();
    (void)bytes;
    map.resize(count, Pair());
    stream.Read(map.GetData(), map.size() * sizeof(Pair));
    map.Dirty = stream.ReadBool();
}

int main()
{
    unsigned char buf[4 + 4 + 2 * sizeof(Pair) + 1];
    unsigned int two = 2, size = 2 * sizeof(Pair);
    std::memcpy(buf, &two, 4); std::memcpy(buf + 4, &size, 4);
    Pair entries[2];
    entries[0].first = 5; entries[0].second.NewTexture = 50; entries[0].second.GroupID = 1;
    entries[1].first = 9; entries[1].second.NewTexture = 90; entries[1].second.GroupID = 2;
    std::memcpy(buf + 8, entries, sizeof(entries));
    buf[8 + sizeof(entries)] = 1;
    Stream in = { buf, 0, (long)sizeof(buf) };
    VectorMap map; map.count = 0; map.Dirty = false;
    GFReadVectorMapMemDump(in, map);
    bool ok = map.count == 2 && map.items[0].first == 5 && map.items[1].second.NewTexture == 90 && map.items[1].second.GroupID == 2
        && map.Dirty && in.position == (long)sizeof(buf);
    unsigned char empty[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    Stream in2 = { empty, 0, 9 };
    GFReadVectorMapMemDump(in2, map);      // shrink to nothing: no entry bytes, Dirty cleared
    ok = ok && map.count == 0 && !map.Dirty && in2.position == 9;
    if (!ok) { std::printf("0086deb0_TEST FAIL\n"); return 1; }
    std::printf("0086deb0_TEST PASS\n");
    return 0;
}