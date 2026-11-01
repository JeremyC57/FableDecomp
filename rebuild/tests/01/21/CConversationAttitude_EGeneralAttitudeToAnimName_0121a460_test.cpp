// Behaviour test for the dynamic initializer of CConversationAttitude::GeneralAttitudeToAnimName (0x0121A460): the vector's three words are zeroed, storage for 18 strings is allocated (finish = start, end = start + 18), 18 copies of the empty string are copy-constructed in place, finish is set past the last, the temporary is destroyed, and the atexit destructor is registered.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
static char g_log[256];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
static int g_copies, g_dtors, g_atexit;
struct StringModel {
    const char* Text;
    StringModel(const char* text, long = -1) : Text(text) { }
    StringModel(const StringModel& other) : Text(other.Text) { ++g_copies; }
    ~StringModel() { ++g_dtors; }
};
struct VectorModel {
    StringModel* Start; StringModel* Finish; StringModel* EndOfStorage;
    VectorModel(unsigned int n, const StringModel& value) : Start(0), Finish(0), EndOfStorage(0)
    {
        Start = n != 0 ? (StringModel*)std::malloc(n * sizeof(StringModel)) : 0;
        Finish = Start;
        EndOfStorage = Start + n;
        StringModel* cur = Start;
        for ( ; n > 0; --n, ++cur)
            new (cur) StringModel(value);
        Finish = cur;
    }
};
static void AtExitDestructor() { ++g_atexit; }
static VectorModel* Initialise(void* storage)
{
    VectorModel* v = new (storage) VectorModel(18, "");
    AtExitDestructor();                              // stands in for the atexit registration of the _$E2 thunk
    return v;
}
int main()
{
    static double storage[4];
    VectorModel* v = Initialise(storage);
    bool ok = v->Finish - v->Start == 18 && v->EndOfStorage == v->Finish && g_copies == 18 && g_dtors == 1 && g_atexit == 1;
    for (int i = 0; i < 18 && ok; ++i) ok = std::strcmp(v->Start[i].Text, "") == 0;
    std::free(v->Start);
    if (!ok) { std::printf("0121a460_TEST FAIL\n"); return 1; }
    std::printf("0121a460_TEST PASS\n");
    return 0;
}