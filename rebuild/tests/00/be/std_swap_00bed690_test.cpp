// Behaviour test for 0x00BED690 std::swap<pair<long, CCompositeSurfaceInfo>> (counted-pointer reference counts survive the swap)
#include <cstdio>

static int g_deleted = 0;
struct Info { unsigned int RefCount; };
struct CP {
    void* Data; Info* Info_;
    CP() : Data(0), Info_(0) { }
    CP(const CP& o) : Data(o.Data), Info_(o.Info_) { if (Info_) ++Info_->RefCount; }
    ~CP() { release(); Data = 0; Info_ = 0; }
    void release() { if (Info_ && --Info_->RefCount == 0) ++g_deleted; }
    CP& operator=(const CP& o) { if (Info_ != o.Info_) { release(); Data = o.Data; Info_ = o.Info_; if (Info_) ++Info_->RefCount; } return *this; }
};
struct Entry { long first; CP second; };

static void swap(Entry& a, Entry& b) { Entry tmp = a; a = b; b = tmp; }

int main()
{
    int da = 1, db = 2;
    Info ia = { 0 }, ib = { 0 };
    {
        Entry a, b;
        a.first = 5; a.second.Data = &da; a.second.Info_ = &ia; ia.RefCount = 1;
        b.first = 9; b.second.Data = &db; b.second.Info_ = &ib; ib.RefCount = 1;
        swap(a, b);
        if (a.first != 9 || b.first != 5 || a.second.Data != &db || b.second.Data != &da
            || ia.RefCount != 1 || ib.RefCount != 1 || g_deleted != 0) { std::printf("00bed690_TEST FAIL\n"); return 1; }
    }
    if (g_deleted != 2) { std::printf("00bed690_TEST FAIL\n"); return 1; }
    std::printf("00bed690_TEST PASS\n");
    return 0;
}