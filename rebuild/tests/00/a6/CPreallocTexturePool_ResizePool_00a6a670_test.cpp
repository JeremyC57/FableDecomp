#include <cstdio>
#include <cstdlib>
#include <vector>

namespace {
void check(bool condition)
{
    if (!condition)
        std::abort();
}

enum Event { Wait, Detach, Remove, ReleaseOwner, Create, InitialiseTexture, Add, Destroy };
struct Entry { Event event; unsigned long value; };
std::vector<Entry> events;

void record(Event event, unsigned long value)
{
    Entry entry = { event, value };
    events.push_back(entry);
}

struct FixtureRender {
    void WaitWhileGPUIsBusy() { record(Wait, 0); }
    void AttachTextureToStage(long stage, const void* texture)
    {
        check(texture == 0);
        record(Detach, stage);
    }
};
struct FixtureDisplay {
    FixtureRender render;
    FixtureRender& GetRenderManager() { return render; }
};
struct FixtureSystem {
    FixtureDisplay display;
    FixtureDisplay* GetDisplayManager() { return &display; }
};
FixtureSystem system;
FixtureSystem* GFGetSystemManager() { return &system; }

unsigned long next_id;
unsigned long live_objects;
struct FixtureData;
struct FixtureOwner {
    FixtureData* texture;
    bool will_release;
    void ReleaseTexture();
};
struct FixtureData {
    FixtureData() : id(++next_id), references(1), allocated(false), listed(false), initialized(false), owner(0)
    {
        ++live_objects;
        record(Create, id);
    }
    ~FixtureData()
    {
        check(!listed && references == 0 && live_objects > 0);
        --live_objects;
        record(Destroy, id);
    }
    void Initialise(unsigned long size, unsigned long index)
    {
        check(size == 64 && index == 7 && !allocated && !initialized);
        initialized = true;
        record(InitialiseTexture, id);
    }
    bool IsAllocated() const { return allocated; }
    FixtureOwner* GetOwner() { check(owner != 0); return owner; }
    unsigned long id;
    unsigned long references;
    bool allocated, listed, initialized;
    FixtureOwner* owner;
};
void FixtureOwner::ReleaseTexture()
{
    record(ReleaseOwner, texture->id);
    if (will_release)
        texture->allocated = false;
}

struct FixturePointer {
    FixturePointer() : data(0) {}
    FixturePointer(const FixturePointer& other) : data(other.data)
    {
        if (data)
            ++data->references;
    }
    ~FixturePointer() { Reset(0); }
    FixturePointer& operator=(const FixturePointer& other)
    {
        if (data != other.data) {
            if (other.data)
                ++other.data->references;
            Reset(other.data);
        }
        return *this;
    }
    void Reset(FixtureData* texture)
    {
        if (data && --data->references == 0)
            delete data;
        data = texture;
    }
    FixtureData* operator->() const { return data; }
    FixtureData& operator*() const { return *data; }
    FixtureData* data;
};

struct FixturePool {
    FixturePool() : Count(0), MaxSize(64), PoolIndex(7) {}
    ~FixturePool()
    {
        for (unsigned long i = 0; i < Count; ++i)
            Textures[i]->listed = false;
    }
    bool ResizePool(unsigned long);
    void RemoveResource(FixtureData& texture)
    {
        check(texture.listed);
        texture.listed = false;
        record(Remove, texture.id);
    }
    void AddResource(FixtureData& texture)
    {
        check(!texture.listed);
        texture.listed = true;
        record(Add, texture.id);
    }
    FixtureData& GetTextureData(unsigned long index) { return *Textures[index]; }
    unsigned long GetTexturesAllocated() const
    {
        unsigned long count = 0;
        for (unsigned long i = 0; i < Count; ++i)
            if (Textures[i]->allocated)
                ++count;
        return count;
    }
    void SetUp(unsigned long n)
    {
        Textures.resize(n);
        Count = n;
        for (unsigned long i = 0; i < n; ++i) {
            Textures[i].Reset(new FixtureData());
            Textures[i]->listed = true;
            Textures[i]->initialized = true;
        }
        events.clear();
    }
    void MarkAllocated(unsigned long index, FixtureOwner& owner, bool makes_available)
    {
        owner.texture = Textures[index].data;
        owner.will_release = makes_available;
        owner.texture->allocated = true;
        owner.texture->owner = &owner;
    }
    unsigned long Count, MaxSize;
    long PoolIndex;
    std::vector<FixturePointer> Textures;
};
}

bool FixturePool::ResizePool(unsigned long new_size)
{
    if (new_size == Count)
        return true;

    bool success = false;
    FixtureRender& render_manager = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    render_manager.WaitWhileGPUIsBusy();
    for (long c_stage = 0; c_stage < 4; ++c_stage)
        render_manager.AttachTextureToStage(c_stage, 0);

    for (unsigned long c1 = 0; c1 < Count; ++c1)
        RemoveResource(*Textures[c1]);

    unsigned long alloc_size = GetTexturesAllocated();
    long dest_index = 0;
    for (unsigned long c1 = 0; c1 < Count; ++c1) {
        FixtureData& managed_texture = GetTextureData(c1);
        if (managed_texture.IsAllocated() && new_size < alloc_size) {
            managed_texture.GetOwner()->ReleaseTexture();
            alloc_size = GetTexturesAllocated();
        }
        if (managed_texture.IsAllocated()) {
            if (static_cast<unsigned long>(dest_index) < new_size &&
                static_cast<unsigned long>(dest_index) != c1) {
                Textures[dest_index] = Textures[c1];
                Textures[c1].Reset(new FixtureData());
            }
            ++dest_index;
        }
    }

    if (static_cast<unsigned long>(dest_index) <= new_size) {
        Count = new_size;
        Textures.resize(Count);
        for (unsigned long c1 = dest_index; c1 < Count; ++c1) {
            Textures[c1].Reset(new FixtureData());
            Textures[c1]->Initialise(MaxSize, PoolIndex);
        }
        success = true;
    } else {
        Count = dest_index;
        Textures.resize(Count);
        success = true;
    }

    for (unsigned long c1 = 0; c1 < Count; ++c1)
        AddResource(*Textures[c1]);
    return success;
}

namespace {
void check_setup_prefix(const unsigned long* removed, unsigned long count)
{
    check(events.size() >= 5 + count);
    check(events[0].event == Wait);
    for (unsigned long i = 0; i < 4; ++i)
        check(events[i+1].event == Detach && events[i+1].value == i);
    for (unsigned long i = 0; i < count; ++i)
        check(events[i+5].event == Remove && events[i+5].value == removed[i]);
}

void check_registration(const FixturePool& pool)
{
    check(pool.Textures.size() == pool.Count);
    const std::size_t first = events.size() - pool.Count;
    for (unsigned long i = 0; i < pool.Count; ++i) {
        check(pool.Textures[i]->listed && pool.Textures[i]->references == 1);
        check(events[first + i].event == Add && events[first + i].value == pool.Textures[i]->id);
    }
}

unsigned count_event(Event event)
{
    unsigned count = 0;
    for (std::size_t i = 0; i != events.size(); ++i)
        if (events[i].event == event)
            ++count;
    return count;
}
}

int main()
{
    {
        FixturePool pool;
        pool.SetUp(2);
        check(pool.ResizePool(2));
        check(events.empty() && pool.Count == 2);
    }
    check(live_objects == 0);
    {
        FixturePool pool;
        FixtureOwner owner;
        pool.SetUp(2);
        pool.MarkAllocated(1, owner, false);
        const unsigned long removed[] = { pool.Textures[0]->id, pool.Textures[1]->id };
        check(pool.ResizePool(4));
        check_setup_prefix(removed, 2);
        check(pool.Count == 4 && pool.Textures[0].data == owner.texture);
        check(count_event(ReleaseOwner) == 0 && count_event(InitialiseTexture) == 3);
        check_registration(pool);
    }
    check(live_objects == 0);
    {
        FixturePool pool;
        FixtureOwner owner1, owner2;
        pool.SetUp(3);
        pool.MarkAllocated(0, owner1, true);
        pool.MarkAllocated(2, owner2, false);
        const unsigned long removed[] = { pool.Textures[0]->id, pool.Textures[1]->id, pool.Textures[2]->id };
        check(pool.ResizePool(1));
        check_setup_prefix(removed, 3);
        check(events[8].event == ReleaseOwner && events[8].value == removed[0]);
        check(pool.Count == 1 && pool.Textures[0].data == owner2.texture);
        check(count_event(ReleaseOwner) == 1 && count_event(InitialiseTexture) == 0);
        check_registration(pool);
    }
    check(live_objects == 0);
    {
        FixturePool pool;
        FixtureOwner owner1, owner2;
        pool.SetUp(3);
        pool.MarkAllocated(0, owner1, false);
        pool.MarkAllocated(1, owner2, false);
        const unsigned long removed[] = { pool.Textures[0]->id, pool.Textures[1]->id, pool.Textures[2]->id };
        check(pool.ResizePool(1));
        check_setup_prefix(removed, 3);
        check(pool.Count == 2 && pool.Textures[0].data == owner1.texture && pool.Textures[1].data == owner2.texture);
        check(count_event(ReleaseOwner) == 2 && count_event(InitialiseTexture) == 0);
        check_registration(pool);
    }
    check(live_objects == 0);
    {
        FixturePool pool;
        FixtureOwner owner;
        pool.SetUp(2);
        pool.MarkAllocated(1, owner, true);
        const unsigned long removed[] = { pool.Textures[0]->id, pool.Textures[1]->id };
        check(pool.ResizePool(0));
        check_setup_prefix(removed, 2);
        check(pool.Count == 0 && pool.Textures.empty() && live_objects == 0);
        check(count_event(ReleaseOwner) == 1 && count_event(Add) == 0);
    }
    {
        FixturePool pool;
        pool.SetUp(0);
        check(pool.ResizePool(3));
        check_setup_prefix(0, 0);
        check(pool.Count == 3 && count_event(InitialiseTexture) == 3);
        check_registration(pool);
    }
    check(live_objects == 0);
    std::puts("00a6a670_TEST PASS");
    return 0;
}