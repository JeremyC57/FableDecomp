// Behaviour test for 0x00AE8C30 NParticleEngine::CParticleEmitterDatabase::CreateEmitterTemplate(long&)
#include <cstdio>
#include <vector>
#include <algorithm>

static int g_liveCount = 0;
struct Emitter { Emitter() { ++g_liveCount; } ~Emitter() { --g_liveCount; } };
struct Info { unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteEmitter(void* p) { delete (Emitter*)p; }
struct Counted {
    Emitter* Data; Info* Info_;
    explicit Counted(Emitter* e) : Data(e) { Info_ = 0; if (e) { Info_ = new Info; Info_->RefCount = 1; Info_->DeleteFunc = DeleteEmitter; Info_->Data = e; } }
    Counted(const Counted& o) : Data(o.Data), Info_(o.Info_) { if (Info_) ++Info_->RefCount; }
    Counted& operator=(const Counted& o) { if (o.Info_) ++o.Info_->RefCount; Release(); Data = o.Data; Info_ = o.Info_; return *this; }
    ~Counted() { Release(); }
    void Release() { if (Info_ && --Info_->RefCount == 0) { Info_->DeleteFunc(Info_->Data); delete Info_; } Info_ = 0; }
};
struct Entry { long Key; Counted Value; Entry(long k, const Counted& v) : Key(k), Value(v) { } };
static bool KeyLess(const Entry& a, const Entry& b) { return a.Key < b.Key; }
struct Database {
    std::vector<Entry> Emitters; bool Dirty; long CurrentHandle;
    Database() : Dirty(false), CurrentHandle(0) { }
    Emitter* CreateEmitterTemplate(long& handle)                        // the row's body
    {
        Emitter* emitter = new Emitter;
        Counted pointer(emitter);
        Emitters.push_back(Entry(CurrentHandle, pointer));                 // CVectorMap::Insert
        Dirty = true;
        if (Dirty) { std::sort(Emitters.begin(), Emitters.end(), KeyLess); Dirty = false; }   // CVectorMap::Update
        handle = CurrentHandle;
        ++CurrentHandle;
        return emitter;
    }
};

int main()
{
    int ok = 1;
    {
        Database db;
        db.CurrentHandle = 7;
        long h1 = -1, h2 = -1;
        Emitter* e1 = db.CreateEmitterTemplate(h1);
        Emitter* e2 = db.CreateEmitterTemplate(h2);
        ok &= h1 == 7 && h2 == 8 && db.CurrentHandle == 9 && e1 != e2 && !db.Dirty;
        ok &= db.Emitters.size() == 2 && db.Emitters[0].Value.Data == e1 && db.Emitters[1].Value.Data == e2;
        ok &= db.Emitters[0].Value.Info_->RefCount == 1 && g_liveCount == 2;   // the map holds the only reference
    }
    ok &= g_liveCount == 0;
    if (ok) std::printf("00ae8c30_TEST PASS\n"); else std::printf("00ae8c30_TEST FAIL\n");
    return ok ? 0 : 1;
}