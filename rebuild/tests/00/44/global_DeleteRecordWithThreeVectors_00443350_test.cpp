// Behaviour test for 0x00443350 DeleteRecordWithThreeVectors(p): null-safe delete freeing the three vectors' storage
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
struct PodVector {
    int* start; int* finish; int* eos;
    PodVector() : start(0), finish(0), eos(0) { }
    ~PodVector() { if (start) { std::free(start); ++g_frees; } }
};
struct CRecordWithThreeVectors { int Key; PodVector First, Second, Third; };
static void DeleteRecordWithThreeVectors(CRecordWithThreeVectors* p) { delete p; }
int main()
{
    CRecordWithThreeVectors* r = new CRecordWithThreeVectors;
    r->First.start = static_cast<int*>(std::malloc(8));
    r->Third.start = static_cast<int*>(std::malloc(8));
    DeleteRecordWithThreeVectors(r);
    DeleteRecordWithThreeVectors(0);
    if (g_frees != 2) { std::printf("00443350_TEST FAIL\n"); return 1; }
    std::printf("00443350_TEST PASS\n");
    return 0;
}