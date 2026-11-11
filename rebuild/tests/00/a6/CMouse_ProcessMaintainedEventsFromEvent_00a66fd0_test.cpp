// Behaviour test for 0x00A66FD0 CMouse::ProcessMaintainedEventsFromEvent: the row's logic over a std::list model.
#include <stdio.h>
#include <list>
struct CEv { long Type; float X, Y; double Start, Cur; long Button; };
enum { LP = 4, LH = 5, LR = 6, RP = 7, RH = 8, RR = 9, MP = 10, MH = 11, MR = 12, XP0 = 22, XH0 = 27, XR0 = 32 };
struct CMouseModel {
    std::list<CEv> held;
    void Process(const CEv* e)
    {
        long t = e->Type;
        if (t == LP || t == RP || t == MP) {
            held.push_back(CEv());
            CEv& h = held.back();
            h.Type = t + 1; h.X = e->X; h.Y = e->Y; h.Start = e->Start; h.Cur = e->Start; h.Button = -1;
            return;
        }
        if (t == LR || t == MR || t == RR) {
            long want = t - 1;
            for (std::list<CEv>::iterator it = held.begin(); it != held.end(); ++it)
                if (it->Type == want) { held.erase(it); return; }
            return;
        }
        if (t >= XP0 && t <= XP0 + 4) {
            held.push_back(CEv());
            CEv& h = held.back();
            h.Type = XH0 + (t - XP0); h.X = e->X; h.Y = e->Y; h.Start = e->Start; h.Cur = e->Start; h.Button = t - XP0;
            return;
        }
        if (t >= XR0 && t <= XR0 + 4) {
            long want = t - 5;
            for (std::list<CEv>::iterator it = held.begin(); it != held.end(); ++it)
                if (it->Type == want) { held.erase(it); return; }
        }
    }
};
static CEv ev(long t) { CEv e; e.Type = t; e.X = 1.0f; e.Y = 2.0f; e.Start = 3.0; e.Cur = 4.0; e.Button = 0; return e; }
int main()
{
    int fails = 0;
    CMouseModel m;
    CEv a = ev(LP), b = ev(RP), c = ev(XP0 + 2), d = ev(LP);
    m.Process(&a); m.Process(&b); m.Process(&c); m.Process(&d);
    if (m.held.size() != 4 || m.held.front().Type != LH || m.held.front().Cur != 3.0) ++fails;
    CEv r = ev(LR); m.Process(&r);                                 // removes the FIRST left-held event only
    if (m.held.size() != 3 || m.held.front().Type != RH || m.held.back().Type != LH) ++fails;
    CEv x = ev(XR0 + 2); m.Process(&x);
    if (m.held.size() != 2) ++fails;
    CEv none = ev(MR); m.Process(&none);                           // no middle held: nothing happens
    if (m.held.size() != 2) ++fails;
    if (fails) { printf("00a66fd0_TEST FAIL %d\n", fails); return 1; }
    printf("00a66fd0_TEST PASS\n");
    return 0;
}