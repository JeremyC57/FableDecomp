// Behaviour test for CAudioObject::Open (0x00C42DD0): resets the object's members and parameter block (gain / pitch 1.0,
// volume 0), copies the system's default state callback, sets the open flag; with EAX caps it also needs (or creates)
// the EAX manager, allocates the property block and an EAX source and links them; any failure returns false.
#include <cstdio>
#include <cstring>

struct Source { void* context; };
struct Props { Source* source; void* owner; int junk; };
struct Manager { void* context; };
static Manager g_manager;
static Manager* g_pManager;
static int g_canCreateManager, g_allocFails, g_sourceFails;
static Props g_props;
static Source g_source;

struct Obj {
    unsigned long caps, flags; long p14; float gain; long volume; float pitch; void* records[6];
    void* voice; void* currentBuffer; void* queueCb; void* queueData; void* stateCb; void* stateData;
    long e4, ec, status; Props* props; Source* source;
    bool Open(unsigned long c, void* defCb, void* defData)
    {
        caps = c; currentBuffer = 0; queueCb = 0; queueData = 0;
        stateCb = defCb; stateData = defData;
        e4 = 0; ec = 0; voice = 0;
        p14 = 0; gain = 0; volume = 0; pitch = 0; std::memset(records, 0, sizeof(records));
        unsigned long requested = caps;
        volume = 0; gain = 1.0f; pitch = 1.0f; flags = 0x100000; status = 0;
        if (requested & 0x2000) {
            flags = 0x120000;
            if (g_pManager == 0) { if (!g_canCreateManager) return false; g_pManager = &g_manager; }
            props = g_allocFails ? 0 : &g_props;
            if (props == 0) return false;
            source = g_sourceFails ? 0 : &g_source;
            if (source == 0) return false;
            source->context = g_pManager->context;
            std::memset(props, 0, sizeof(*props));
            props->source = source; props->owner = this;
        }
        return true;
    }
};

int main()
{
    int ok = 1;
    int cb = 0, data = 0;
    Obj o; std::memset(&o, 0x55, sizeof(o));
    ok &= o.Open(0x80, &cb, &data);
    ok &= o.flags == 0x100000 && o.gain == 1.0f && o.pitch == 1.0f && o.volume == 0 && o.voice == 0 && o.status == 0;
    ok &= o.stateCb == &cb && o.stateData == &data && o.records[5] == 0 && o.caps == 0x80;
    g_pManager = 0; g_canCreateManager = 0;
    ok &= !o.Open(0x2000, 0, 0) && o.flags == 0x120000;
    g_canCreateManager = 1; g_allocFails = 1;
    ok &= !o.Open(0x2000, 0, 0) && g_pManager == &g_manager;
    g_allocFails = 0; g_sourceFails = 1;
    ok &= !o.Open(0x2000, 0, 0);
    g_sourceFails = 0; g_manager.context = &data;
    ok &= o.Open(0x2080, 0, 0) && o.props == &g_props && g_props.source == &g_source && g_props.owner == &o;
    ok &= g_source.context == &data && g_props.junk == 0;
    if (!ok) { std::printf("00c42dd0_TEST FAIL\n"); return 1; }
    std::printf("00c42dd0_TEST PASS\n");
    return 0;
}