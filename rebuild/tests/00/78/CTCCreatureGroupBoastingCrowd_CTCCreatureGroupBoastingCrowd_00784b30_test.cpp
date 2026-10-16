#include <cstdio>
#include <cstdlib>
// Behaviour model of CTCCreatureGroupBoastingCrowd::CTCCreatureGroupBoastingCrowd (0x00784B30).
struct State { unsigned char flags; };
struct Thing { State* state; int forced; void SetForceFrameUpdate(bool f) { if (f) ++forced; } };
struct Node { unsigned char color; Node* parent; Node* left; Node* right; };
struct Crowd {
    Thing* parent; unsigned int MemberCount, ActiveMemberCount; bool ShutdownOnEmpty; Node* header; unsigned int count;
    void* hero; int CrowdState, CurrentEncouragementState; bool HeroBoasted, HeroMadeGoodBoast;
    int CurrentOpinionDeedHandle, HeroOpinionSourcesPushed, FrameToClear;
    Crowd(Thing* p) : parent(p), MemberCount(0), ActiveMemberCount(0), ShutdownOnEmpty(true), header(0), count(0)
    {
        header = (Node*)std::malloc(sizeof(Node)); header->color = 0; header->parent = 0; header->left = header; header->right = header;
        hero = 0; CrowdState = 0; CurrentEncouragementState = 2; HeroBoasted = false; HeroMadeGoodBoast = false;
        CurrentOpinionDeedHandle = -1; HeroOpinionSourcesPushed = 0; FrameToClear = 0;
        if (p->state && !(p->state->flags & 2)) p->SetForceFrameUpdate(true);
        ShutdownOnEmpty = false;
    }
};
int main()
{
    State s; s.flags = 0; Thing t; t.state = &s; t.forced = 0;
    Crowd c(&t);
    if (t.forced != 1 || c.ShutdownOnEmpty || c.CurrentEncouragementState != 2 || c.CurrentOpinionDeedHandle != -1 ||
        c.header->left != c.header || c.header->right != c.header || c.header->parent != 0 || c.count != 0) { std::printf("00784b30_TEST FAIL\n"); return 1; }
    s.flags = 2; Crowd d(&t);
    if (t.forced != 1) { std::printf("00784b30_TEST FAIL\n"); return 1; }
    t.state = 0; Crowd e(&t);
    if (t.forced != 1) { std::printf("00784b30_TEST FAIL\n"); return 1; }
    std::printf("00784b30_TEST PASS\n");
    return 0;
}