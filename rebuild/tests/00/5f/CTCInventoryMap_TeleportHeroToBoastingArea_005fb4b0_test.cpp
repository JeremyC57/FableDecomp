// Test for 0x005FB4B0 CTCInventoryMap::TeleportHeroToBoastingArea: self-contained behaviour model -- with a live hero and a
// boasting-area marker creature (property 0x72), the mini map is flagged for the region load and one move-hero event (type 0x2C)
// is handed to the world carrying the marker's X, Y, Z, its facing angle, three false flag bytes and the main player number.
#include <cstdio>
#include <cstring>

struct Event {
    long type; signed char player; unsigned char data[32]; unsigned char end;
    Event(long t) : type(t), player(-1), end(0) {}
    void Add(const void* p, int n) { std::memcpy(data + end, p, n); end = (unsigned char)(end + n); }
    void AddFloat(float f) { Add(&f, 4); }
    void AddUBYTE(unsigned char b) { Add(&b, 1); }
};
struct Creature { float x, y, z, angle; unsigned long property; };
struct World { bool miniMap; int events; Event last; World() : miniMap(false), events(0), last(0) {} };
static void Teleport(bool heroAlive, const Creature* creatures, int count, long mainPlayer, World& world)
{
    if (!heroAlive) return;
    const Creature* marker = 0;
    for (int i = 0; i < count && !marker; ++i)
        if (creatures[i].property == 0x72) marker = &creatures[i];
    if (!marker) return;
    world.miniMap = true;
    Event e(0x2c);
    e.AddFloat(marker->x); e.AddFloat(marker->y); e.AddFloat(marker->z); e.AddFloat(marker->angle);
    e.AddUBYTE(false); e.AddUBYTE(false); e.AddUBYTE(false);
    e.player = (signed char)mainPlayer;
    ++world.events; world.last = e;
}

static int fail(int n) { std::printf("005fb4b0_TEST FAIL %d\n", n); return 1; }

int main()
{
    Creature c[2] = { { 1, 2, 3, 0.5f, 3 }, { 10.0f, 20.0f, 30.0f, 1.25f, 0x72 } };
    World w;
    Teleport(false, c, 2, 0, w);
    if (w.events || w.miniMap) return fail(1);
    Teleport(true, c, 1, 0, w);
    if (w.events || w.miniMap) return fail(2);
    Teleport(true, c, 2, 1, w);
    if (w.events != 1 || !w.miniMap || w.last.type != 0x2c || w.last.player != 1 || w.last.end != 19) return fail(3);
    float f[4]; std::memcpy(f, w.last.data, 16);
    if (f[0] != 10.0f || f[1] != 20.0f || f[2] != 30.0f || f[3] != 1.25f) return fail(4);
    if (w.last.data[16] || w.last.data[17] || w.last.data[18]) return fail(5);
    std::printf("005fb4b0_TEST PASS\n");
    return 0;
}