// Behaviour test for 00cbb1b0: __destroy_aux over pair<unsigned __int64, CCountedPointer<T> > -- each counted pointer released; the last reference runs the delete function and frees the block
#include <cstdio>
static int g_deleted = 0;
struct Info { long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteObj(void*) { ++g_deleted; }
struct CP { void* Data; Info* Info_; ~CP() { if (Info_) { if (--Info_->RefCount == 0) { Info_->DeleteFunc(Info_->Data); delete Info_; } } Data = 0; Info_ = 0; } };
struct Entry { unsigned __int64 first; CP second; };
static void DestroyAux(Entry* first, Entry* last) { for (; first != last; ++first) first->~Entry(); }
int main()
{
    Info* shared = new Info; shared->RefCount = 2; shared->DeleteFunc = DeleteObj; shared->Data = 0;
    Info* solo = new Info; solo->RefCount = 1; solo->DeleteFunc = DeleteObj; solo->Data = 0;
    unsigned char raw[3 * sizeof(Entry)]; Entry* e = (Entry*)raw;
    e[0].first = 1; e[0].second.Data = 0; e[0].second.Info_ = shared;
    e[1].first = 2; e[1].second.Data = 0; e[1].second.Info_ = solo;
    e[2].first = 3; e[2].second.Data = 0; e[2].second.Info_ = 0;
    DestroyAux(e, e + 2);
    int ok = sizeof(Entry) == 16 && g_deleted == 1 && shared->RefCount == 1 && e[0].second.Info_ == 0 && e[1].second.Info_ == 0 && e[2].first == 3;
    DestroyAux(e + 2, e + 2);
    delete shared;
    std::printf(ok ? "00cbb1b0_TEST PASS\n" : "00cbb1b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
