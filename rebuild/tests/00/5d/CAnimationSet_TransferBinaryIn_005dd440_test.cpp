// 005dd440 behaviour test: CAnimationSet::TransferBinaryIn (final.cpp body, little-endian byte stream model)
#include <stdio.h>
#include <string.h>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
typedef unsigned int u32;
struct Stream {
    const unsigned char* p; const unsigned char* end;
    u32 ReadU32() { u32 v = 0; if (end - p >= 4) { v = (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24); p += 4; } return v; }
};
class CDefString { public: int Index; CDefString() : Index(-1) { } void TransferBinaryIn(Stream* s) { Index = (int)s->ReadU32(); } };
class CAnimationEntry { public:
    int BankIndex; CDefString AnimName; CDefString GroupName; void* Components;
    CAnimationEntry() : BankIndex(0), Components(0) { }
    void TransferBinaryIn(Stream* s) { BankIndex = (int)s->ReadU32(); AnimName.TransferBinaryIn(s); GroupName.TransferBinaryIn(s); }
};
struct CAnimPair { u32 first; CAnimationEntry second; CAnimPair() : first(0) { } };
class CAnimationSet { public:
    std::vector<CAnimPair> Anims;
    int DefaultFlags, DefaultTransitionTime, DefaultDelay;
    CDefString DefaultGroup;
    void TransferBinaryIn(Stream* stream);
};
void CAnimationSet::TransferBinaryIn(Stream* stream)
{
    u32 count = stream->ReadU32();
    Anims.resize(count, CAnimPair());
    for (u32 i = 0; i < count; ++i) {
        Anims[i].first = stream->ReadU32();
        Anims[i].second.TransferBinaryIn(stream);
    }
    DefaultFlags = (int)stream->ReadU32();
    DefaultTransitionTime = (int)stream->ReadU32();
    DefaultDelay = (int)stream->ReadU32();
    DefaultGroup.TransferBinaryIn(stream);
}
static void put(std::vector<unsigned char>& b, u32 v) { for (int i = 0; i < 4; ++i) b.push_back((unsigned char)(v >> (8 * i))); }
int main()
{
    std::vector<unsigned char> b;
    put(b, 2);
    put(b, 0x11); put(b, 3); put(b, 40); put(b, 41);
    put(b, 0x22); put(b, 4); put(b, 50); put(b, 51);
    put(b, 7); put(b, 250); put(b, 100); put(b, 9);
    Stream s = { &b[0], &b[0] + b.size() };
    CAnimationSet set; set.DefaultFlags = 0;
    set.TransferBinaryIn(&s);
    CHECK(set.Anims.size() == 2);
    CHECK(set.Anims[0].first == 0x11 && set.Anims[0].second.BankIndex == 3 && set.Anims[0].second.AnimName.Index == 40);
    CHECK(set.Anims[1].first == 0x22 && set.Anims[1].second.GroupName.Index == 51);
    CHECK(set.DefaultFlags == 7 && set.DefaultTransitionTime == 250 && set.DefaultDelay == 100 && set.DefaultGroup.Index == 9);
    CHECK(s.p == s.end);
    // an empty set shrinks the animation table
    std::vector<unsigned char> e; put(e, 0); put(e, 1); put(e, 2); put(e, 3); put(e, 4);
    Stream s2 = { &e[0], &e[0] + e.size() };
    set.TransferBinaryIn(&s2);
    CHECK(set.Anims.empty() && set.DefaultFlags == 1 && set.DefaultGroup.Index == 4);
    if (!g_fail) printf("005dd440_TEST PASS\n");
    return g_fail;
}