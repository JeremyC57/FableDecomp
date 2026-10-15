#include <cstdio>
struct CTCBase;
struct CThing {
    unsigned char m_type; unsigned char m_flags; CTCBase* m_registered; int m_calls; bool m_lastRegistered;
    bool IsDead() const { return (m_flags & 1) != 0; }
    void SetComponentRegistered(bool registered, CTCBase* component) { ++m_calls; m_lastRegistered = registered; m_registered = registered ? component : 0; }
};
struct CTCBase { void* vptrSlot; CThing* ParentThing; };
struct CTCForcePushed : CTCBase {
    void OnKill()
    {
        CThing* thing = ParentThing;
        if (thing && !thing->IsDead() && thing->m_type == 1)
            thing->SetComponentRegistered(false, 0);
    }
};
int main()
{
    CTCForcePushed f; f.ParentThing = 0; f.OnKill();                      // no thing: nothing
    CThing t; t.m_type = 1; t.m_flags = 1; t.m_calls = 0; t.m_registered = &f; f.ParentThing = &t;
    f.OnKill(); bool dead = t.m_calls == 0;
    t.m_flags = 0; t.m_type = 3; f.OnKill(); bool other = t.m_calls == 0;
    t.m_type = 1; f.OnKill();
    if (!dead || !other || t.m_calls != 1 || t.m_lastRegistered || t.m_registered != 0) { std::printf("00798510_TEST FAIL\n"); return 1; }
    std::printf("00798510_TEST PASS\n");
    return 0;
}