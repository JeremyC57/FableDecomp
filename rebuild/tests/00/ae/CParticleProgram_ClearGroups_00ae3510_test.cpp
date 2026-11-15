// Standalone C++03 behavior fixture; test-only containers trace the two real calls.
// The function body below is copied verbatim from the selected row.
#include <vector>
#include <stdio.h>
#include <stdlib.h>
static int Trace[4], TraceCount, Released;
static bool Tracking;
static void Check(bool condition, int line)
{
    if (!condition) { fprintf(stderr, "check failed at %d\n", line); exit(1); }
}
#define CHECK(x) Check((x), __LINE__)
class C3DGroup2 {
public:
    int Data;
    explicit C3DGroup2(int data = 0) : Data(data) {}
    ~C3DGroup2() { if (Tracking && Data) ++Released; }
};
template<class T> class CArray : public std::vector<T> {
public:
    int TraceId;
    CArray() : TraceId(-1) {}
    void clear() {
        if (Tracking && TraceId >= 0) {
            CHECK(TraceCount < 4);
            Trace[TraceCount++] = TraceId;
        }
        std::vector<T>::clear();
    }
};
class CParticleProgram {
public:
    int UntouchedBefore;
    CArray<C3DGroup2> NonSimGroups;
    CArray<C3DGroup2> SimGroups;
    int UntouchedAfter;
    void ClearGroups();
};
void CParticleProgram::ClearGroups()
{
    SimGroups.clear();
    NonSimGroups.clear();
}
int main()
{
    const int counts[3] = {0, 1, 4};
    for (int p = 0; p != 3; ++p) {
        for (int q = 0; q != 3; ++q) {
            CParticleProgram value;
            value.UntouchedBefore = 12345;
            value.UntouchedAfter = -5678;
            value.SimGroups.TraceId = 1;
            value.NonSimGroups.TraceId = 2;
            for (int i = 0; i < counts[p]; ++i)
                value.SimGroups.push_back(C3DGroup2(i + 1));
            for (int j = 0; j < counts[q]; ++j) {
                value.NonSimGroups.push_back(C3DGroup2(j + 10));
            }
            const size_t parentCapacity = value.SimGroups.capacity();
            const size_t groupCapacity = value.NonSimGroups.capacity();
            TraceCount = 0; Released = 0; Tracking = true;
            value.ClearGroups();
            Tracking = false;
            CHECK(TraceCount == 2 && Trace[0] == 1 && Trace[1] == 2);
            CHECK(Released == counts[p] + counts[q]);
            CHECK(value.SimGroups.empty() && value.NonSimGroups.empty());
            CHECK(value.SimGroups.capacity() == parentCapacity);
            CHECK(value.NonSimGroups.capacity() == groupCapacity);
            CHECK(value.UntouchedBefore == 12345 && value.UntouchedAfter == -5678);
            TraceCount = 0; Released = 0; Tracking = true;
            value.ClearGroups();
            Tracking = false;
            CHECK(TraceCount == 2 && Trace[0] == 1 && Trace[1] == 2);
            CHECK(Released == 0);
            CHECK(value.SimGroups.empty() && value.NonSimGroups.empty());
        }
    }
    puts("00ae3510_TEST PASS");
    return 0;
}