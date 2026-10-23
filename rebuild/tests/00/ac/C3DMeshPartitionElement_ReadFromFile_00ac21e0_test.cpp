// Behaviour test for C3DMeshPartitionElement::ReadFromFile (0x00AC21E0): box, sphere, triangle range, recursive children.
#include <cstdio>
#include <cstring>
struct Stream {
    const unsigned char* p;
    float ReadFloat() { float v; std::memcpy(&v, p, 4); p += 4; return v; }
    int ReadLong() { int v; std::memcpy(&v, p, 4); p += 4; return v; }
    bool ReadBool() { return *p++ != 0; }
};
static int g_live = 0;
struct Element {
    float Min[3], Max[3], Centre[3], Radius; int Start, Count; Element* Sub[2];
    Element() { Sub[0] = Sub[1] = 0; Radius = 0; ++g_live; }
    ~Element() { delete Sub[0]; delete Sub[1]; --g_live; }
    void Read(Stream& in)
    {
        delete Sub[0]; delete Sub[1]; Sub[0] = Sub[1] = 0;
        int i;
        for (i = 0; i < 3; ++i) Min[i] = in.ReadFloat();
        for (i = 0; i < 3; ++i) Max[i] = in.ReadFloat();
        for (i = 0; i < 3; ++i) Centre[i] = in.ReadFloat();
        Radius = in.ReadFloat();
        Start = in.ReadLong(); Count = in.ReadLong();
        if (in.ReadBool()) { Sub[0] = new Element; Sub[1] = new Element; Sub[0]->Read(in); Sub[1]->Read(in); }
    }
};
static unsigned char* node(unsigned char* q, float base, int start, bool kids)
{
    for (int i = 0; i < 10; ++i) { float f = base + (float)i; std::memcpy(q, &f, 4); q += 4; }
    std::memcpy(q, &start, 4); q += 4; int c = start + 1; std::memcpy(q, &c, 4); q += 4;
    *q++ = kids ? 1 : 0;
    return q;
}
int main()
{
    unsigned char buf[512]; unsigned char* q = buf;
    q = node(q, 0.0f, 10, true); q = node(q, 100.0f, 20, false); q = node(q, 200.0f, 30, false);
    Element* root = new Element; Stream s = { buf }; root->Read(s);
    if (s.p != q || g_live != 3 || root->Max[2] != 5.0f || root->Radius != 9.0f || root->Start != 10 || root->Count != 11
        || root->Sub[0]->Start != 20 || root->Sub[1]->Centre[0] != 206.0f) { std::printf("00ac21e0_TEST FAIL\n"); return 1; }
    q = node(buf, 1.0f, 5, false); Stream t = { buf }; root->Read(t);
    if (g_live != 1 || root->Sub[0] || root->Min[0] != 1.0f) { std::printf("00ac21e0_TEST FAIL\n"); return 1; }
    delete root;
    std::printf("00ac21e0_TEST PASS\n");
    return 0;
}