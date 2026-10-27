// behaviour test for 0x008D3850 CGameScriptThing::MsgAttackedWithMeleeWeaponWithoutHittingAnything() (model over std::list; the component lookup is a stub)
#include <cstdio>
#include <list>
struct CEvent { int type; long WorldFrame; int GetType() const { return type; } };
class CTCEntityEvents { public: std::list<CEvent*> Events; };
class CThing { public: unsigned char dead; CTCEntityEvents* tc; bool GetTC(CTCEntityEvents** out) const { if (!tc) return false; *out = tc; return true; } };
static long g_last = 10, g_current = 20;
static bool Msg(const CThing* thing)
{
    if (thing && !(thing->dead & 1)) {
        long lastFrame = g_last, currentFrame = g_current;
        CTCEntityEvents* events;
        if (thing->GetTC(&events)) {
            for (std::list<CEvent*>::const_iterator it = events->Events.begin(); it != events->Events.end(); ++it) {
                const CEvent* event = *it;
                if (event->GetType() == 0x1E && event->WorldFrame > lastFrame && event->WorldFrame <= currentFrame)
                    return true;
            }
        }
    }
    return false;
}
int main()
{
    CEvent early = { 0x1E, 10 }, other = { 0x7F, 15 }, hit = { 0x1E, 20 };
    CTCEntityEvents tc; tc.Events.push_back(&early); tc.Events.push_back(&other);
    CThing t = { 0, &tc };
    int ok = !Msg(&t);
    tc.Events.push_back(&hit);
    ok = ok && Msg(&t);
    t.dead = 1; ok = ok && !Msg(&t);
    if (!ok) { std::printf("008d3850_TEST FAIL\n"); return 1; }
    std::printf("008d3850_TEST PASS\n");
    return 0;
}