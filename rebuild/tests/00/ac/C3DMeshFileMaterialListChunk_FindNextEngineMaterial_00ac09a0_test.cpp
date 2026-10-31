// Test: C3DMeshFileMaterialListChunk::FindNextEngineMaterial -- converts `previous` to a base-typed counted pointer,
// asks the super chunk for the next sub-chunk of the same type and static-casts the result into a fresh counted
// pointer returned by value; reference counts must balance (the found chunk shared once more, temporaries released).
#include <cstdio>
struct Info { long RefCount; };
struct Chunk { virtual ~Chunk() {} int id; };
struct Material : Chunk { };
template <class T> struct CP {
    T* Data; Info* Info_;
    CP() : Data(0), Info_(0) {}
    CP(T* d, Info* i) : Data(d), Info_(i) { if (Info_) ++Info_->RefCount; }
    CP(const CP& o) : Data(o.Data), Info_(o.Info_) { if (Info_) ++Info_->RefCount; }
    template <class U> CP(const CP<U>& o) : Data(o.Data), Info_(o.Info_) { if (Info_) ++Info_->RefCount; }
    ~CP() { if (Info_) --Info_->RefCount; Data = 0; Info_ = 0; }
    void ShareData(T* d, Info* i) { if (Info_ != i) { if (Info_) --Info_->RefCount; Data = d; Info_ = i; if (i) ++i->RefCount; } }
    template <class U> void SetStaticCast(const CP<U>& o) { ShareData(static_cast<T*>(o.Data), o.Info_); }
};
static Material g_m[3]; static Info g_i[3];
struct Super {
    const CP<Chunk> FindNextSubChunkOfType(const CP<const Chunk>& prev) const
    {
        for (int k = 0; k < 2; ++k)
            if (prev.Data == &g_m[k]) return CP<Chunk>(&g_m[k + 1], &g_i[k + 1]);
        return CP<Chunk>();
    }
};
struct List : Super {
    CP<const Material> FindNextEngineMaterial(const CP<const Material>& previous) const
    {
        CP<const Material> chunk;
        chunk.SetStaticCast(FindNextSubChunkOfType(previous));
        return chunk;
    }
};
int main()
{
    for (int k = 0; k < 3; ++k) { g_i[k].RefCount = 1; g_m[k].id = k; }
    List list;
    bool ok = true;
    {
        CP<const Material> first(&g_m[0], &g_i[0]);
        CP<const Material> next = list.FindNextEngineMaterial(first);
        ok = ok && next.Data == &g_m[1] && g_i[1].RefCount == 2 && g_i[0].RefCount == 2;
        CP<const Material> last = list.FindNextEngineMaterial(CP<const Material>(&g_m[2], &g_i[2]));
        ok = ok && last.Data == 0 && last.Info_ == 0;
    }
    ok = ok && g_i[0].RefCount == 1 && g_i[1].RefCount == 1 && g_i[2].RefCount == 1;
    std::printf(ok ? "00ac09a0_TEST PASS\n" : "00ac09a0_TEST FAIL\n");
    return ok ? 0 : 1;
}