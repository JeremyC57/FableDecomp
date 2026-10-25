// Behaviour test for GFSerialiseMapBinaryIn(stream, map<CCharString, bool>&) (0x007ADBF0): the map is cleared, the count
// is read as a 32-bit word, then each key string and bool value are read and inserted (duplicates keep the first value).
#include <cstdio>
#include <cstring>

struct CCharString {
    char text[16];
    CCharString() { text[0] = 0; }
    CCharString(const char* t) { std::strcpy(text, t); }
};
struct Stream {                       // a flat byte buffer: [count:4] then per entry [len:1][chars][bool:1]
    const unsigned char* data;
    int position, length;
    bool ok(int n) const { return position + n <= length; }
    void Read(void* out, int n) { if (!ok(n)) return; std::memcpy(out, data + position, n); position += n; }
    unsigned int ReadWord() { unsigned int v = 0; unsigned char b[4] = { 0, 0, 0, 0 }; Read(b, 4); v = b[0] | (b[1] << 8) | (b[2] << 16) | ((unsigned int)b[3] << 24); return v; }
    CCharString ReadString() { CCharString s; unsigned char n = 0; Read(&n, 1); Read(s.text, n); s.text[n] = 0; return s; }
};
struct Pair { CCharString first; bool second; Pair() { } Pair(const CCharString& f, bool s) : first(f), second(s) { } };
struct Map {
    Pair items[8];
    unsigned int Size;
    void clear() { Size = 0; }
    void insert(const Pair& p)
    {
        for (unsigned int i = 0; i < Size; ++i)
            if (std::strcmp(items[i].first.text, p.first.text) == 0) return;
        items[Size++] = p;
    }
};
static void TransferBinaryIn(Stream& in, bool* p) { unsigned char v = 0; in.Read(&v, 1); *p = v != 0; }
static void GFSerialiseMapBinaryIn(Stream& in, Map& out)
{
    if (out.Size != 0) out.clear();
    int count = (int)in.ReadWord();
    for (; count > 0; --count) {
        Pair pair;
        pair.second = false;
        pair.first = in.ReadString();
        TransferBinaryIn(in, &pair.second);
        out.insert(Pair(pair.first, pair.second));
    }
}

int main()
{
    static const unsigned char buf[] = { 3, 0, 0, 0, 2, 'h', 'i', 1, 3, 'f', 'o', 'o', 0, 2, 'h', 'i', 0 };
    Stream in = { buf, 0, (int)sizeof(buf) };
    Map m; m.Size = 1; m.items[0] = Pair(CCharString("old"), true);
    GFSerialiseMapBinaryIn(in, m);
    bool ok = m.Size == 2 && std::strcmp(m.items[0].first.text, "hi") == 0 && m.items[0].second
        && std::strcmp(m.items[1].first.text, "foo") == 0 && !m.items[1].second && in.position == (int)sizeof(buf);
    static const unsigned char none[] = { 0, 0, 0, 0 };
    Stream in2 = { none, 0, 4 };
    GFSerialiseMapBinaryIn(in2, m);
    ok = ok && m.Size == 0;
    if (!ok) { std::printf("007adbf0_TEST FAIL\n"); return 1; }
    std::printf("007adbf0_TEST PASS\n");
    return 0;
}