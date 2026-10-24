// Behaviour test for 0x00719FA0 vector<CSubtitledMessage>::_M_allocate_and_copy: n slots, each copy through the copy ctor.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
struct CSubtitledMessage {
    int SpeechIndex; unsigned char rest[0x40];
    CSubtitledMessage(int i) : SpeechIndex(i) {}
    CSubtitledMessage(const CSubtitledMessage& o) : SpeechIndex(o.SpeechIndex) { ++g_copies; }
};
static CSubtitledMessage* AllocateAndCopy(unsigned int n, const CSubtitledMessage* first, const CSubtitledMessage* last)
{
    CSubtitledMessage* result = n != 0 ? (CSubtitledMessage*)std::malloc(n * sizeof(CSubtitledMessage)) : 0;
    CSubtitledMessage* cur = result;
    for (; first != last; ++first, ++cur) new (cur) CSubtitledMessage(*first);
    return result;
}
int main()
{
    CSubtitledMessage src[2] = { CSubtitledMessage(7), CSubtitledMessage(11) };
    g_copies = 0;
    CSubtitledMessage* p = AllocateAndCopy(4, src, src + 2);
    bool ok = p != 0 && g_copies == 2 && p[0].SpeechIndex == 7 && p[1].SpeechIndex == 11;
    std::free(p);
    if (!ok) { std::printf("00719fa0_TEST FAIL\n"); return 1; }
    std::printf("00719fa0_TEST PASS\n");
    return 0;
}