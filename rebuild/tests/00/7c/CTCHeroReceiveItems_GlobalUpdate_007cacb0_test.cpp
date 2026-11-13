// Behaviour test for CTCHeroReceiveItems::GlobalUpdate (retail 0x007CACB0): a copy of the model's control flow over plain data --
// the ForceClear path (give the current item, pop it, empty-list stop with the container's fishing spot disabled and the background
// primitive cleared), the purge-and-take-next path, and the display path (area growth with GFLimit / GFGetInterpolatedValue, the
// bottom-right anchor to the render target, the full-size timer and the copy of the current orientation).
#include <cstdio>
#include <list>
#include <utility>
struct Box { float TLX, TLY, BRX, BRY; };
struct Item { bool alive; int id; };
static float Interp(float o, float n, float t) { return (n - o) * t + o; }
static void Limit(float& v, float lo, float hi) { if (v < lo) v = lo; else if (v > hi) v = hi; }
struct Model {
    std::list<std::pair<Item, bool> > Items;
    Item Current; bool HasCurrent; bool Persist;
    bool Receiving, Displaying, ForceClear, SendMessage;
    bool ContainerAlive, FishingSpot, SpotEnabled;
    float Amount; long FullSizeTimer; Box Initial, Old, Cur;
    int RHOld, RHCur;                       // stand-ins for the two orientations (copy order only)
    int ItemHandle, BackgroundHandle, Messages, Given, Sounds, Rendered;
    long FPS; float SecsFull, SecsStay, TLX, TLY, BRX, BRY; long Width, Height;
    Model() : HasCurrent(false), Persist(false), Receiving(false), Displaying(false), ForceClear(false), SendMessage(false),
              ContainerAlive(false), FishingSpot(false), SpotEnabled(false), Amount(0.0f), FullSizeTimer(0), RHOld(0), RHCur(0),
              ItemHandle(0), BackgroundHandle(0), Messages(0), Given(0), Sounds(0), Rendered(0), FPS(0), SecsFull(0.0f),
              SecsStay(0.0f), TLX(0.0f), TLY(0.0f), BRX(0.0f), BRY(0.0f), Width(0), Height(0) {
        Current.alive = false; Current.id = 0;
        Initial.TLX = Initial.TLY = Initial.BRX = Initial.BRY = 0.0f; Old = Initial; Cur = Initial;
    }
    void GlobalUpdate() {
        if (!Receiving) return;
        if (ForceClear) {
            ForceClear = false; Amount = 0.0f; Cur = Initial; Old = Initial;
            ItemHandle = 0; Displaying = false;
            if (HasCurrent && Current.alive) { ++Given; Items.pop_front(); HasCurrent = false; }
            if (Items.empty()) {
                Displaying = false;
                if (SendMessage && ContainerAlive) ++Messages;
                if (ContainerAlive && FishingSpot) SpotEnabled = false;
                BackgroundHandle = 0;
                return;
            }
        }
        if (!(HasCurrent && Current.alive)) {
            for (std::list<std::pair<Item, bool> >::iterator it = Items.begin(); it != Items.end(); ) {
                if (it->first.alive) ++it; else it = Items.erase(it);
            }
            if (Items.empty()) { Receiving = false; Displaying = false; return; }
            Current = Items.front().first; HasCurrent = true; Persist = Items.front().second; ++Sounds;
        }
        if (Displaying) {
            Old = Cur;
            if (Amount < 1.0f) {
                Amount += 1.0f / (FPS * SecsFull);
                Limit(Amount, 0.0f, 1.0f);
                Cur.TLX = Interp(Initial.TLX, TLX, Amount);
                Cur.TLY = Interp(Initial.TLY, TLY, Amount);
                Cur.BRX = Interp(Initial.BRX, (float)Width + BRX - 640.0f, Amount);
                Cur.BRY = Interp(Initial.BRY, (float)Height + BRY - 480.0f, Amount);
                FullSizeTimer = (long)(FPS * SecsStay + 1.0f);
            }
            --FullSizeTimer;
            RHOld = RHCur; ++RHCur;
            ++Rendered;
        }
    }
};
int main()
{
    Model m;
    Item a = { true, 1 };
    m.Items.push_back(std::make_pair(a, true));
    m.Current = a; m.HasCurrent = true; m.Receiving = true; m.ForceClear = true; m.SendMessage = true;
    m.ContainerAlive = true; m.FishingSpot = true; m.SpotEnabled = true; m.ItemHandle = 5; m.BackgroundHandle = 6;
    m.Initial.TLX = 1.0f; m.Cur.TLX = 9.0f;
    m.GlobalUpdate();
    bool ok = m.Given == 1 && m.Items.empty() && m.Messages == 1 && !m.SpotEnabled && m.BackgroundHandle == 0 && m.ItemHandle == 0
           && m.Cur.TLX == 1.0f && m.Old.TLX == 1.0f && m.Receiving && !m.Displaying && !m.ForceClear;
    Model d;
    Item dead = { false, 2 }, live = { true, 3 };
    d.Items.push_back(std::make_pair(dead, false)); d.Items.push_back(std::make_pair(live, true));
    d.Receiving = true; d.Displaying = true; d.FPS = 30; d.SecsFull = 0.5f; d.SecsStay = 2.0f;
    d.TLX = 10.0f; d.TLY = 20.0f; d.BRX = 30.0f; d.BRY = 40.0f; d.Width = 800; d.Height = 600; d.RHCur = 7;
    d.GlobalUpdate();
    ok = ok && d.Items.size() == 1 && d.HasCurrent && d.Current.id == 3 && d.Persist && d.Sounds == 1 && d.Rendered == 1
            && d.Amount > 0.066f && d.Amount < 0.067f && d.FullSizeTimer == 60 && d.RHOld == 7 && d.RHCur == 8
            && d.Cur.BRX > 12.6f && d.Cur.BRX < 12.7f;
    d.Amount = 0.99f; d.GlobalUpdate();
    ok = ok && d.Amount == 1.0f && d.Cur.TLX == 10.0f && d.Cur.BRY == 160.0f && d.Sounds == 1;
    Model e;
    e.Items.push_back(std::make_pair(dead, false)); e.Receiving = true; e.Displaying = true;
    e.GlobalUpdate();
    ok = ok && !e.Receiving && !e.Displaying && e.Items.empty() && e.Rendered == 0;
    if (!ok) { std::printf("007cacb0_TEST FAIL\n"); return 1; }
    std::printf("007cacb0_TEST PASS\n");
    return 0;
}