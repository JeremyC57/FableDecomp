// Behaviour test for 0x009F5690 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
#include <cstring>
struct Event { float StartTime, CurrentTime; int Key; };
struct File { const unsigned char* P; void Read(void* d, int n) { std::memcpy(d, P, n); P += n; } };
static double g_time = 10.0;
struct Package {
    long TimeStamp; std::vector<Event> Events;
    Package(File& f) {
        long count; f.Read(&count, 4); f.Read(&TimeStamp, 4);
        for (long i = 0; i < count; ++i) { Event e; f.Read(&e, sizeof e); e.CurrentTime = (float)g_time; double age = e.StartTime; e.StartTime = (float)(g_time - age); Events.push_back(e); }
    }
};
int main() {
    unsigned char buf[64]; long n = 2, ts = 77; Event a = { 1.0f, 0, 5 }, b = { 4.0f, 0, 6 };
    std::memcpy(buf, &n, 4); std::memcpy(buf + 4, &ts, 4); std::memcpy(buf + 8, &a, sizeof a); std::memcpy(buf + 8 + sizeof a, &b, sizeof b);
    File f = { buf }; Package p(f);
    bool ok = p.TimeStamp == 77 && p.Events.size() == 2 && p.Events[0].StartTime == 9.0f && p.Events[1].StartTime == 6.0f && p.Events[1].Key == 6 && p.Events[0].CurrentTime == 10.0f;
    std::printf(ok ? "009f5690_TEST PASS\n" : "009f5690_TEST FAIL\n");
    return ok ? 0 : 1;
}