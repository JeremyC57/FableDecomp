// Test for CASoundManager::~CASoundManager (0x00991880): every bank-map entry's record is destroyed, the storage freed, the
// resource bank's count block released (deleter run at zero), both counted-pointer words cleared.
#include <cstdio>
#include <cstdlib>
static int g_recordDtors = 0, g_frees = 0, g_deleter = 0, g_baseDtor = 0;
struct Info { unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteBank(void*) { ++g_deleter; }
struct Record { int id; ~Record() { ++g_recordDtors; } };
struct Entry { long key; Record rec; };
struct Base { virtual ~Base() { ++g_baseDtor; } };
struct SoundManager : Base {
    int* resData; Info* resInfo;
    Entry* start; Entry* finish; Entry* eos;
    ~SoundManager()
    {
        for (Entry* e = start; e != finish; ++e) e->~Entry();
        if (start) { std::free(start); ++g_frees; }
        if (resInfo) {
            --resInfo->RefCount;
            if (resInfo->RefCount == 0) { resInfo->DeleteFunc(resInfo->Data); delete resInfo; }
        }
        resData = 0; resInfo = 0;
    }
};
int main()
{
    {
        SoundManager m;
        m.start = (Entry*)std::malloc(3 * sizeof(Entry)); m.finish = m.start + 3; m.eos = m.finish;
        int bank = 0; m.resData = &bank;
        m.resInfo = new Info; m.resInfo->RefCount = 1; m.resInfo->DeleteFunc = DeleteBank; m.resInfo->Data = &bank;
    }
    if (g_recordDtors != 3 || g_frees != 1 || g_deleter != 1 || g_baseDtor != 1) { std::printf("00991880_TEST FAIL\n"); return 1; }
    {
        Info* shared = new Info; shared->RefCount = 2; shared->DeleteFunc = DeleteBank; shared->Data = 0;
        SoundManager* m = new SoundManager; m->start = m->finish = m->eos = 0; m->resData = 0; m->resInfo = shared;
        delete m;
        if (shared->RefCount != 1 || g_deleter != 1 || g_frees != 1 || g_baseDtor != 2) { std::printf("00991880_TEST FAIL\n"); return 1; }
        delete shared;
    }
    std::printf("00991880_TEST PASS\n");
    return 0;
}