#include <cstdio>
#include <cstring>
struct File { unsigned char buf[128]; int pos; void Write(const void* d, long n, long) { std::memcpy(buf + pos, d, n); pos += n; } };
struct Pair { unsigned int lo, hi; bool open; };
struct Base { int baseSaves; void SaveToFile(File& f) const { const_cast<Base*>(this)->baseSaves++; } };
struct Switchable : Base {
    Pair* b; Pair* e;
    void SaveToFile(File& f) const
    {
        Base::SaveToFile(f);
        int count = (int)(e - b);
        f.Write(&count, 4, 0);
        for (const Pair* it = b; it != e; ++it) f.Write(&it->lo, 8, 0);
    }
};
int main()
{
    Pair p[2] = { { 1, 2, true }, { 3, 4, false } };
    Switchable s; s.baseSaves = 0; s.b = p; s.e = p + 2;
    File f; f.pos = 0;
    s.SaveToFile(f);
    unsigned int w[5]; std::memcpy(w, f.buf, 20);
    if (s.baseSaves != 1 || f.pos != 20 || w[0] != 2 || w[1] != 1 || w[2] != 2 || w[3] != 3 || w[4] != 4) { std::printf("00a77c30_TEST FAIL\n"); return 1; }
    std::printf("00a77c30_TEST PASS\n");
    return 0;
}