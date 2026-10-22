#include <cstdio>
#include <cstdlib>
// Behaviour test for CCombatActionBase::SetupCombatStrikeEvents @ 0x00859A70: every strike-start event of the animation
// whose damage-dealing thing is alive and has a weapon trail becomes a strike event (start frame clamped to >= 1); each
// strike event then takes the nearest end frame (at least start + 1), both frames shift by the anim delay (end + 1) and
// the action records the last end frame; strike events without an end event are erased from the list.
struct AnimEvent { int kind; int id; float time; };     // kind 1 = strike start, 2 = strike end, 0 = other
struct Thing { bool dead; bool hasTrail; };
struct StrikeEvent { int id; long StartFrame; long EndFrame; Thing* weapon; };
struct Node { Node* next; Node* prev; StrikeEvent* data; };

static Thing g_weapon = { false, true };
static Thing g_deadWeapon = { true, true };
static Thing g_noTrail = { false, false };

static bool Parse(const AnimEvent& e, bool& isStart, bool& isWeapon)
{
    isWeapon = e.id != 9;
    if (e.kind == 0)
        return false;
    isStart = e.kind == 1;
    return true;
}
static Thing* DamageThing(const AnimEvent& e)
{
    if (e.id == 7) return &g_deadWeapon;
    if (e.id == 8) return &g_noTrail;
    if (e.id == 6) return 0;
    return &g_weapon;
}

struct Finder {
    StrikeEvent* Event;
    long NearestFrame;
    bool Found;
    Finder(StrikeEvent* e) : Event(e), NearestFrame(2147483647L), Found(false) { }
    void operator()(const AnimEvent& e)
    {
        long frame = (long)(e.time * 30.0f);
        if (e.kind == 2 && e.id == Event->id && frame >= Event->StartFrame && frame < NearestFrame) {
            NearestFrame = frame;
            Found = true;
        }
    }
};
template <class It, class F> F ForEach(It first, It last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

struct Action {
    Node head;                                        // list sentinel
    long LastStrikeEventEndTime;
    long delay;
    bool Initialised;
    int created, erased;
    void PushBack(StrikeEvent* e)
    {
        Node* n = new Node; n->data = e;
        n->next = &head; n->prev = head.prev; head.prev->next = n; head.prev = n;
        ++created;
    }
    Node* Erase(Node* n)
    {
        Node* next = n->next; Node* prev = n->prev;
        prev->next = next; next->prev = prev;
        delete n->data; delete n; ++erased;
        return next;
    }
    void Setup(const AnimEvent* first, const AnimEvent* last)
    {
        if (!Initialised)
            return;
        float framesPerSecond = 30.0f;
        float timeScale = 1.0f;
        for (const AnimEvent* it = first; it != last; ++it) {
            long startFrame = (long)(it->time * framesPerSecond);
            bool isStart = false, isWeapon = false;
            if (Parse(*it, isStart, isWeapon) && isStart) {
                if (startFrame == 0)
                    startFrame = 1;
                Thing* weapon = DamageThing(*it);
                if (weapon && !weapon->dead && weapon->hasTrail) {
                    StrikeEvent* e = new StrikeEvent;
                    e->id = it->id; e->StartFrame = startFrame; e->EndFrame = 0; e->weapon = weapon;
                    PushBack(e);
                }
            }
        }
        long lastEndTime = 0;
        for (Node* node = head.next; node != &head; ) {
            StrikeEvent* e = node->data;
            Finder finder = ForEach(first, last, Finder(e));
            if (finder.Found) {
                e->EndFrame = finder.NearestFrame;
                if (e->EndFrame <= e->StartFrame)
                    e->EndFrame = e->StartFrame + 1;
                e->StartFrame = (long)(e->StartFrame * timeScale);
                e->EndFrame = (long)(e->EndFrame * timeScale);
                e->StartFrame += delay;
                e->EndFrame += delay + 1;
                if (e->EndFrame > lastEndTime)
                    lastEndTime = e->EndFrame;
                node = node->next;
            } else {
                node = Erase(node);
            }
        }
        LastStrikeEventEndTime = lastEndTime;
    }
};

static int Fail(int n) { std::printf("00859a70_TEST FAIL %d\n", n); return 1; }

int main()
{
    // frames at 30 fps: 0.0 -> 0 (clamped to 1), 0.5 -> 15, 1.0 -> 30
    static const AnimEvent events[] = {
        { 1, 1, 0.0f },   // start of strike 1 at frame 0 -> 1
        { 0, 0, 0.2f },   // unrelated event
        { 1, 2, 0.5f },   // start of strike 2 at 15
        { 2, 2, 0.5f },   // end of strike 2 at 15 (== start -> start + 1)
        { 1, 3, 0.5f },   // start of strike 3: no end event -> erased
        { 1, 7, 0.5f },   // dead weapon -> not created
        { 1, 8, 0.5f },   // no weapon trail -> not created
        { 1, 6, 0.5f },   // no damage thing -> not created
        { 2, 1, 1.0f },   // end of strike 1 at 30
        { 2, 1, 0.8f },   // nearer end of strike 1 at 24
    };
    Action a;
    a.head.next = a.head.prev = &a.head;
    a.LastStrikeEventEndTime = -5; a.delay = 4; a.Initialised = true; a.created = a.erased = 0;
    a.Setup(events, events + sizeof(events) / sizeof(events[0]));
    if (a.created != 3 || a.erased != 1) return Fail(1);
    Node* n = a.head.next;
    if (n == &a.head || n->data->id != 1 || n->data->StartFrame != 1 + 4 || n->data->EndFrame != 24 + 4 + 1) return Fail(2);
    n = n->next;
    if (n == &a.head || n->data->id != 2 || n->data->StartFrame != 15 + 4 || n->data->EndFrame != 16 + 4 + 1) return Fail(3);
    if (n->next != &a.head) return Fail(4);
    if (a.LastStrikeEventEndTime != 29) return Fail(5);
    // not initialised: nothing happens
    Action b;
    b.head.next = b.head.prev = &b.head;
    b.LastStrikeEventEndTime = -5; b.delay = 0; b.Initialised = false; b.created = b.erased = 0;
    b.Setup(events, events + 3);
    if (b.created != 0 || b.LastStrikeEventEndTime != -5 || b.head.next != &b.head) return Fail(6);
    std::printf("00859a70_TEST PASS\n");
    return 0;
}