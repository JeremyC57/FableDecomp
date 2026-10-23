// Behaviour test for 0x0080e480
#include <cstdio>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTypedObject {
    long m_type;
    virtual void v00() {}
    virtual long GetType() { return m_type; }
};
struct CTCInterface31 {
    CTypedObject* m_obj;
    CTypedObject* GetObject() { return m_obj; }
};
struct CThingFilter_IsAffectedByObstructions {
    bool operator()(CThing* thing) const
    {
        CTCInterface31* component;
        if (thing->GetInterface(0x31, &component) && component->GetObject()->GetType() == 0x5F)
            return false;
        return true;
    }
};
int main()
{
    CTypedObject obj; obj.m_type = 0x5F;
    CTCInterface31 comp; comp.m_obj = &obj;
    CInterfaceMap::SEntry e[1]; e[0].type = 0x31; e[0].component = (CTCBase*)&comp;
    CThing t; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = e; t.m_interfaces.last = e + 1;
    CThingFilter_IsAffectedByObstructions f;
    bool ok = f(&t);                                 // absent -> true
    t.m_presentBits[1] = 0x20000;
    if (f(&t)) ok = false;                           // type 0x5F -> false
    obj.m_type = 3;
    if (!f(&t)) ok = false;
    if (!ok) { std::printf("0080e480_TEST FAIL\n"); return 1; }
    std::printf("0080e480_TEST PASS\n");
    return 0;
}