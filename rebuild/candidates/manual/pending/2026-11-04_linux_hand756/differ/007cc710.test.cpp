// Behaviour test for CTCMultiArrow::ReleaseShot (0x007CC710): a model of the row -- after a whole volley the newest ammo effect
// is removed (and killed); with no effect showing the pending count goes down instead; the last shot cleans up.
#include <stdio.h>
#include <vector>
struct Arrow { std::vector<int> Effects; int Pending, ShotsLeft, VolleySize, ShotsInVolley, Killed; bool Cleaned; };
static void ReleaseShot(Arrow& a)
{
    if (++a.ShotsInVolley == a.VolleySize) {
        a.ShotsInVolley = 0;
        if (!a.Effects.empty()) { a.Effects.pop_back(); ++a.Killed; }
        else if (a.Pending > 0) --a.Pending;
    }
    if (--a.ShotsLeft == 0) a.Cleaned = true;
}
int main()
{
    Arrow a; a.Effects.push_back(1); a.Effects.push_back(2); a.Pending = 1; a.ShotsLeft = 6; a.VolleySize = 2; a.ShotsInVolley = 0;
    a.Killed = 0; a.Cleaned = false;
    for (int i = 0; i < 6; ++i) ReleaseShot(a);
    int ok = a.Effects.empty() && a.Killed == 2 && a.Pending == 0 && a.Cleaned;
    printf(ok ? "007cc710_TEST PASS\n" : "007cc710_TEST FAIL\n");
    return ok ? 0 : 1;
}
