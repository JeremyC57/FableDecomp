// behaviour test for 0x00EC0B40 CV_ParanoidWhispersScript::CSecretWhisperingFriend::OnPredicateFail: only a kill (by anyone: the empty killer name) sets CV_ParanoidWhispersScript::ParanoidState to 5
#include <cstdio>
#include <cstring>
struct Thing { bool killed; const char* askedKiller; bool MsgIsKilledBy(const char* who) { askedKiller = who; return killed; } };
struct Owner { long field; };
static void OnPredicateFail(Thing& me, Owner& owner)        // the function under test
{
    if (me.MsgIsKilledBy(""))
        owner.field = (long)(5);
}
int main()
{
    int fail = 0;
    Thing t; t.killed = false; t.askedKiller = 0; Owner o; o.field = 99;
    OnPredicateFail(t, o);
    if (o.field != 99 || !t.askedKiller || std::strlen(t.askedKiller) != 0) fail |= 1;
    t.killed = true; OnPredicateFail(t, o);
    if (o.field != (long)(5)) fail |= 2;
    if (fail) { std::printf("00ec0b40_TEST FAIL %d\n", fail); return 1; }
    std::printf("00ec0b40_TEST PASS\n");
    return 0;
}