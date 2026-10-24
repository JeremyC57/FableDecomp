// Behaviour test for 0x00711D50 CAnimateThing::CDelayedAnimation::CDelayedAnimation() (empty list: self-linked sentinel).
#include <cstdio>
#include <cstdlib>

struct SNode { SNode* next; SNode* prev; unsigned char data[0x54]; };
struct CDelayedAnimation {
    SNode* CombinationAnims;                              // list<CAnimSet>: sentinel pointer
    CDelayedAnimation()
    {
        CombinationAnims = 0;
        SNode* n = (SNode*)std::malloc(sizeof(SNode));
        n->next = n;
        n->prev = n;
        CombinationAnims = n;
    }
};

int main()
{
    CDelayedAnimation d;
    bool ok = d.CombinationAnims != 0 && d.CombinationAnims->next == d.CombinationAnims
        && d.CombinationAnims->prev == d.CombinationAnims;
    CDelayedAnimation e;
    ok = ok && e.CombinationAnims != d.CombinationAnims;
    std::free(d.CombinationAnims);
    std::free(e.CombinationAnims);
    if (!ok) { std::printf("00711d50_TEST FAIL\n"); return 1; }
    std::printf("00711d50_TEST PASS\n");
    return 0;
}