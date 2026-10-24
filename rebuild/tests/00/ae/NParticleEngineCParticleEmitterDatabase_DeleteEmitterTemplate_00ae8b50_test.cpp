// behaviour test for 0x00AE8B50 NParticleEngine::CParticleEmitterDatabase::DeleteEmitterTemplate(long handle): erase the equal range of the handle from the sorted emitter map, releasing the templates
#include <cstdio>
static int gDeleted = 0;
struct CCountBlock { long RefCount; void (*DeleteData)(void* data); void* Data; };
template <class T> struct TDeleter { static void DeleteData(void* data) { ++gDeleted; delete (T*)data; } };
template <class T> struct CCountedPointer {
    T* Data; CCountBlock* Block;
    CCountedPointer() : Data(0), Block(0) { }
    explicit CCountedPointer(T* p) : Data(p), Block(0)
    { if (p) { Block = new CCountBlock; Block->RefCount = 1; Block->DeleteData = &TDeleter<T>::DeleteData; Block->Data = p; } }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Block(o.Block) { if (Block) ++Block->RefCount; }
    void ReleaseData()
    {
        if (Block) { --Block->RefCount; if (Block->RefCount == 0) { Block->DeleteData(Block->Data); delete Block; } }
        Block = 0; Data = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        if (Block != o.Block) { ReleaseData(); Data = o.Data; Block = o.Block; if (Block) ++Block->RefCount; }
        return *this;
    }
    ~CCountedPointer() { ReleaseData(); }
    long Refs() const { return Block ? Block->RefCount : 0; }
};
struct SEmitter { int Unused; };
struct SEntry { long first; CCountedPointer<SEmitter> second; };
struct SDatabase {
    SEntry Entries[8]; int Count;
    SEntry* LowerBound(long key) { SEntry* f = Entries; long n = Count; while (n > 0) { long h = n / 2; SEntry* m = f + h; if (m->first < key) { f = m + 1; n -= h + 1; } else n = h; } return f; }
    SEntry* UpperBound(long key) { SEntry* f = Entries; long n = Count; while (n > 0) { long h = n / 2; SEntry* m = f + h; if (!(key < m->first)) { f = m + 1; n -= h + 1; } else n = h; } return f; }
    void EraseRange(SEntry* first, SEntry* last)
    {
        SEntry* end = Entries + Count; SEntry* d = first;
        for (SEntry* s = last; s != end; ++s, ++d) { d->first = s->first; d->second = s->second; }
        for (SEntry* p = d; p != end; ++p) p->second.ReleaseData();
        Count = (int)(d - Entries);
    }
    void DeleteEmitterTemplate(long handle) { SEntry* first = LowerBound(handle); if (first != Entries + Count) EraseRange(first, UpperBound(handle)); }
};
int main()
{

    {
        SDatabase db;
        db.Count = 0;
        long keys[6] = { 1, 3, 3, 4, 7, 9 };
        for (int i = 0; i < 6; ++i) { db.Entries[i].first = keys[i]; db.Entries[i].second = CCountedPointer<SEmitter>(new SEmitter); ++db.Count; }
        db.DeleteEmitterTemplate(3);
        if (db.Count != 4 || gDeleted != 2) { std::printf("00ae8b50_TEST FAIL\n"); return 1; }
        long expect[4] = { 1, 4, 7, 9 };
        for (int i = 0; i < 4; ++i) if (db.Entries[i].first != expect[i] || db.Entries[i].second.Refs() != 1) { std::printf("00ae8b50_TEST FAIL\n"); return 1; }
        db.DeleteEmitterTemplate(5);                                         // absent (lower bound not at the end)
        db.DeleteEmitterTemplate(42);                                        // past the end: nothing
        if (db.Count != 4 || gDeleted != 2) { std::printf("00ae8b50_TEST FAIL\n"); return 1; }
        db.DeleteEmitterTemplate(9);
        if (db.Count != 3 || gDeleted != 3) { std::printf("00ae8b50_TEST FAIL\n"); return 1; }
    }
    if (gDeleted != 6) { std::printf("00ae8b50_TEST FAIL\n"); return 1; }
    std::printf("00ae8b50_TEST PASS\n");
    return 0;
}