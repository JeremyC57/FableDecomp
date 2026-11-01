// Behaviour test for 0x0084C490 CCreatureAction_PlayConversationAnimation::Clone() const: a new object of the same dynamic type, the base copied through its copy
// constructor and UseAnimationMovement / Duration copied.
#include <cstdio>
static int g_baseCopies = 0;
class CCreatureActionBase {
public:
    CCreatureActionBase() {}
    CCreatureActionBase(const CCreatureActionBase&) { ++g_baseCopies; }
    virtual ~CCreatureActionBase() {}
    virtual CCreatureActionBase* Clone() const = 0;
    virtual int Kind() const { return 0; }
    unsigned char _body[0xa8 - sizeof(void*)];
};
class CCreatureAction_PlayConversationAnimation : public CCreatureActionBase {
public:
    CCreatureAction_PlayConversationAnimation() : UseAnimationMovement(false), Duration(0) {}
    virtual CCreatureActionBase* Clone() const;
    virtual int Kind() const { return 7; }
    bool UseAnimationMovement;
    float Duration;
};
CCreatureActionBase* CCreatureAction_PlayConversationAnimation::Clone() const
{
    return new CCreatureAction_PlayConversationAnimation(*this);
}
int main()
{
    CCreatureAction_PlayConversationAnimation a;
    a.UseAnimationMovement = true;
    a.Duration = 2.5f;
    CCreatureActionBase* c = a.Clone();
    CCreatureAction_PlayConversationAnimation* d = (CCreatureAction_PlayConversationAnimation*)c;
    if (c == 0 || c->Kind() != 7 || g_baseCopies != 1 || d->UseAnimationMovement != true || d->Duration != 2.5f) {
        std::printf("0084c490_TEST FAIL\n"); return 1;
    }
    delete c;
    std::printf("0084c490_TEST PASS\n");
    return 0;
}