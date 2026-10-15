#include <cstdio>
struct CBaseObject { int id; };
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct SKeyedThing {
    unsigned long       m_key;
    CIntelligentPointer m_thing;
    SKeyedThing() : m_key(0) { }
    SKeyedThing(const unsigned long& key, const CIntelligentPointer& thing) : m_key(key), m_thing(thing) { }
};
struct CKeyedThingVector {
    SKeyedThing m_items[4]; int m_count;
    void PushBack(const SKeyedThing& r) { m_items[m_count].m_key = r.m_key; m_items[m_count].m_thing.SetPItem(r.m_thing.GetPItem()); ++m_count; }
};
struct CVectorMap_Things {
    CKeyedThingVector m_records;
    unsigned char Compare; bool Dirty;
    void Add(const unsigned long& key, const CIntelligentPointer& thing)
    {
        {
            SKeyedThing record(key, thing);
            m_records.PushBack(record);
        }
        Dirty = true;
    }
};
int main()
{
    CBaseObject a = { 1 }, b = { 2 };
    CVectorMap_Things q; q.m_records.m_count = 0; q.Dirty = false;
    CIntelligentPointer pa; pa.SetPItem(&a); CIntelligentPointer pb; pb.SetPItem(&b);
    q.Add(10, pa); q.Add(20, pb);
    if (q.m_records.m_count != 2 || q.m_records.m_items[0].m_key != 10 || q.m_records.m_items[1].m_thing.GetPItem() != &b || !q.Dirty) { std::printf("006a3da0_TEST FAIL\n"); return 1; }
    std::printf("006a3da0_TEST PASS\n");
    return 0;
}