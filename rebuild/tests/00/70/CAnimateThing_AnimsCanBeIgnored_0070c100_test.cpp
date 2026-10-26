// Behaviour test for CAnimateThing::AnimsCanBeIgnored (0x0070C100): flagged transition/combination
// sets block ignoring; a one/two-anim main set with matching alphas and the flag blocks it too.
#include <cstdio>

class CAnimSet { public:
    short anims[2]; long animCount; long alphaCount; unsigned long Flags;
};
struct SNode { SNode* next; CAnimSet value; };

static bool AnimsCanBeIgnored(SNode* transHead, SNode* combHead, const CAnimSet& main)
{
    SNode* node;
    for (node = transHead->next; node != transHead; node = node->next)
        if (node->value.Flags & 8) return false;
    for (node = combHead->next; node != combHead; node = node->next)
        if (node->value.Flags & 8) return false;
    long count = main.animCount;
    if ((count == 1 || (count == 2 && main.anims[1] > 0)) && main.anims[0] > 0
        && count == main.alphaCount && (main.Flags & 8))
        return false;
    return true;
}

int main()
{
    SNode th, ch, n1;
    th.next = &th; ch.next = &ch;
    CAnimSet main = { { 1, 0 }, 1, 1, 8 };
    if (AnimsCanBeIgnored(&th, &ch, main)) { std::printf("0070c100_TEST FAIL\n"); return 1; }
    main.Flags = 0;
    if (!AnimsCanBeIgnored(&th, &ch, main)) { std::printf("0070c100_TEST FAIL\n"); return 1; }
    main.Flags = 8; main.animCount = 2; main.alphaCount = 2;
    if (!AnimsCanBeIgnored(&th, &ch, main)) { std::printf("0070c100_TEST FAIL\n"); return 1; }   // second anim 0
    main.anims[1] = 3;
    if (AnimsCanBeIgnored(&th, &ch, main)) { std::printf("0070c100_TEST FAIL\n"); return 1; }
    main.alphaCount = 1;
    if (!AnimsCanBeIgnored(&th, &ch, main)) { std::printf("0070c100_TEST FAIL\n"); return 1; }
    main.animCount = 3; main.alphaCount = 3;
    if (!AnimsCanBeIgnored(&th, &ch, main)) { std::printf("0070c100_TEST FAIL\n"); return 1; }
    n1.next = &ch; n1.value.Flags = 8; ch.next = &n1;
    if (AnimsCanBeIgnored(&th, &ch, main)) { std::printf("0070c100_TEST FAIL\n"); return 1; }
    std::printf("0070c100_TEST PASS\n");
    return 0;
}