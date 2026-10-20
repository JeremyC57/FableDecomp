// Behaviour test for CGameEventPackageSet::InitFromCompressedBuffer @ 0x009F1AC0: NoPackages reset, then per package
// (count byte, 32-bit time stamp, events) the reused local package is cleared, stamped, filled through
// CGameEvent::InitFromCompressedBuffer (advancing by the bytes it consumed) + AddEvent, and copied whole into the next
// slot. Slots past the new count are left alone.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

class CGameEvent {
public:
    long Type;
    signed char Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool Valid;
    bool Replacement;
    CGameEvent() : Player(-1), Valid(false) {}
    long InitFromCompressedBuffer(const unsigned char* buffer);     // 0x009F1870
};

static int g_initCalls = 0, g_addCalls = 0;
// Host model of the callee: u16 type (bit 15 = replacement), player, data length, data bytes.
long CGameEvent::InitFromCompressedBuffer(const unsigned char* buffer)
{
    ++g_initCalls;
    unsigned short t = (unsigned short)(buffer[0] | (buffer[1] << 8));
    Type = t & 0x7fff; Replacement = (t & 0x8000) != 0;
    Player = (signed char)buffer[2]; EndPos = buffer[3];
    std::memcpy(Data, buffer + 4, EndPos); Valid = true;
    return 4 + EndPos;
}

class CGameEventPackage {
public:
    long TimeStamp;
    unsigned long NoEvents;
    CGameEvent Events[40];
    CGameEventPackage() : NoEvents(0) {}
    void Clear() { NoEvents = 0; }
    void SetTimeStamp(long timeStamp) { TimeStamp = timeStamp; }
    void AddEvent(const CGameEvent* event);                         // 0x009F1650
};
void CGameEventPackage::AddEvent(const CGameEvent* event) { ++g_addCalls; Events[NoEvents++] = *event; }

class CGameEventPackageSet {
public:
    unsigned long NoPackages;
    CGameEventPackage Packages[50];
    void InitFromCompressedBuffer(const unsigned char* buffer);
};

void CGameEventPackageSet::InitFromCompressedBuffer(const unsigned char* buffer)
{
    CGameEvent event;
    CGameEventPackage package;
    NoPackages = 0;
    unsigned char noPackages = *buffer++;
    for (long i = 0; i < noPackages; ++i) {
        unsigned char noEvents = *buffer++;
        long timeStamp = *(const long*)buffer;
        buffer += 4;
        package.Clear();
        package.SetTimeStamp(timeStamp);
        for (long j = 0; j < noEvents; ++j) {
            buffer += event.InitFromCompressedBuffer(buffer);
            package.AddEvent(&event);
        }
        Packages[NoPackages] = package;
        ++NoPackages;
    }
}

static CGameEventPackageSet g_set;

int main()
{
    CHECK(sizeof(CGameEvent) == 0x28 && sizeof(CGameEventPackage) == 0x648);
    std::memset(&g_set, 0xCD, sizeof(g_set));
    g_set.NoPackages = 7; g_set.Packages[3].TimeStamp = 0x5a5a5a5a; g_set.Packages[3].NoEvents = 9;

    const unsigned char buf[] = {
        3,                                             // packages
        2, 0x44, 0x33, 0x22, 0x11,                     // package 0: 2 events, stamp 0x11223344
            0x34, 0x12, 7, 3, 0xA0, 0xA1, 0xA2,        //   type 0x1234, player 7, 3 data bytes
            0x45, 0x83, 0xFF, 0,                       //   type 0x0345 replacement, player -1, no data
        0, 0xFB, 0xFF, 0xFF, 0xFF,                     // package 1: no events, stamp -5
        1, 0x01, 0x00, 0x00, 0x00,                     // package 2: 1 event, stamp 1
            0x1A, 0x00, 2, 1, 0x2B,                    //   type 0x1a, player 2, 1 data byte
    };
    g_set.InitFromCompressedBuffer(buf);

    CHECK(g_set.NoPackages == 3 && g_initCalls == 3 && g_addCalls == 3);
    const CGameEventPackage& p0 = g_set.Packages[0];
    CHECK(p0.TimeStamp == 0x11223344 && p0.NoEvents == 2);
    CHECK(p0.Events[0].Type == 0x1234 && p0.Events[0].Player == 7 && p0.Events[0].EndPos == 3 && p0.Events[0].Data[2] == 0xA2
          && p0.Events[0].Valid && !p0.Events[0].Replacement);
    CHECK(p0.Events[1].Type == 0x345 && p0.Events[1].Player == -1 && p0.Events[1].EndPos == 0 && p0.Events[1].Replacement);
    CHECK(g_set.Packages[1].TimeStamp == -5 && g_set.Packages[1].NoEvents == 0);
    const CGameEventPackage& p2 = g_set.Packages[2];
    CHECK(p2.TimeStamp == 1 && p2.NoEvents == 1 && p2.Events[0].Type == 0x1a && p2.Events[0].Player == 2 && p2.Events[0].Data[0] == 0x2B);
    CHECK(g_set.Packages[3].TimeStamp == 0x5a5a5a5a && g_set.Packages[3].NoEvents == 9);   // untouched

    // An empty buffer resets the count only.
    const unsigned char none[] = { 0 };
    g_initCalls = g_addCalls = 0;
    g_set.InitFromCompressedBuffer(none);
    CHECK(g_set.NoPackages == 0 && g_initCalls == 0 && g_set.Packages[0].TimeStamp == 0x11223344);

    if (g_fail) { std::printf("009f1ac0_TEST FAIL\n"); return 1; }
    std::printf("009f1ac0_TEST PASS\n");
    return 0;
}