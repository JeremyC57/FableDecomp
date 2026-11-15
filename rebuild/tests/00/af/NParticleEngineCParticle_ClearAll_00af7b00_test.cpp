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
class CEnginePrimitiveHandle {
public:
    int Data;
    explicit CEnginePrimitiveHandle(int data = 0) : Data(data) {}
    ~CEnginePrimitiveHandle() { if (Tracking && Data) ++Released; }
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
namespace NParticleEngine {
class CParticle {
public:
    int UntouchedBefore;
    CArray<CArray<long> > PrimIDList;
    CArray<CEnginePrimitiveHandle> ParentHandleList;
    int UntouchedAfter;
    void ClearAll();
};
}
void NParticleEngine::CParticle::ClearAll()
{
    ParentHandleList.clear();
    PrimIDList.clear();
}
int main()
{
    const int counts[3] = {0, 1, 4};
    for (int p = 0; p != 3; ++p) {
        for (int q = 0; q != 3; ++q) {
            NParticleEngine::CParticle value;
            value.UntouchedBefore = 12345;
            value.UntouchedAfter = -5678;
            value.ParentHandleList.TraceId = 1;
            value.PrimIDList.TraceId = 2;
            for (int i = 0; i < counts[p]; ++i)
                value.ParentHandleList.push_back(CEnginePrimitiveHandle(i + 1));
            for (int j = 0; j < counts[q]; ++j) {
                CArray<long> group;
                for (int k = 0; k <= j; ++k) group.push_back(10 + k);
                value.PrimIDList.push_back(group);
            }
            const size_t parentCapacity = value.ParentHandleList.capacity();
            const size_t groupCapacity = value.PrimIDList.capacity();
            TraceCount = 0; Released = 0; Tracking = true;
            value.ClearAll();
            Tracking = false;
            CHECK(TraceCount == 2 && Trace[0] == 1 && Trace[1] == 2);
            CHECK(Released == counts[p]);
            CHECK(value.ParentHandleList.empty() && value.PrimIDList.empty());
            CHECK(value.ParentHandleList.capacity() == parentCapacity);
            CHECK(value.PrimIDList.capacity() == groupCapacity);
            CHECK(value.UntouchedBefore == 12345 && value.UntouchedAfter == -5678);
            TraceCount = 0; Released = 0; Tracking = true;
            value.ClearAll();
            Tracking = false;
            CHECK(TraceCount == 2 && Trace[0] == 1 && Trace[1] == 2);
            CHECK(Released == 0);
            CHECK(value.ParentHandleList.empty() && value.PrimIDList.empty());
        }
    }
    puts("00af7b00_TEST PASS");
    return 0;
}