// Behaviour test for 0x0074FBB0 _STL::for_each(CThingObject**, CThingObject**, CAddObjectsNotSolidTo): every object in
// the range has each entry of the held object list added to its interface-77 CTCSolidToThing; the functor comes back.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CTCBase;
class CThing;

class CTCSolidToThing {
public:
    void AddThingNotSolidTo(CThing* thing);                  // 0x00770FA0
};

template <class K, class V> struct CKeyPair {
    K first;
    V second;
};

class CInterfaceMap {                                        // CVectorMap<ETCInterfaceType, CTCBase*>
public:
    typedef CKeyPair<long, CTCBase*> value_type;
    value_type* First;                                       // +0x00
    value_type* Last;                                        // +0x04
    value_type* EndOfStorage;                                // +0x08
    static __forceinline const value_type* LowerBound(const value_type* first, const value_type* last, const long& key)
    {
        int count = last - first;
        while (count > 0) {
            int half = count / 2;
            const value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count -= half + 1;
            }
            else
                count = half;
        }
        return first;
    }
    const value_type* LowerBound(const long& key) const
    {
        const value_type* first = First;
        int count = Last - First;
        while (count > 0) {
            int half = count / 2;
            const value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count -= half + 1;
            }
            else
                count = half;
        }
        return first;
    }
};

class CThingObject {
public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                        // +0x20
    CInterfaceMap   m_interfaces;                            // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type) const
    {
        if (HasTC(type)) {
            const CInterfaceMap::value_type* last = m_interfaces.Last;
            const CInterfaceMap::value_type* it = CInterfaceMap::LowerBound(m_interfaces.First, last, type);
            return (T*)((it == last || type < it->first) ? last : it)->second;
        }
    }
};

namespace _STL {
template <class T> class vector {
public:
    T* _M_start;                                             // +0x00
    T* _M_finish;                                            // +0x04
    T* _M_end_of_storage;                                    // +0x08
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
};
}

class CAddObjectsNotSolidTo {
public:
    const _STL::vector<CThingObject*>* ObjectList;           // +0x00
    CAddObjectsNotSolidTo(const _STL::vector<CThingObject*>& objects) : ObjectList(&objects) { }
    __forceinline void operator()(CThingObject* object)
    {
        CTCSolidToThing* solid = object->GetTC<CTCSolidToThing>(77);
        const CThingObject* const* end = ObjectList->end();
        for (const CThingObject* const* it = ObjectList->begin(); it != end; ++it)
            solid->AddThingNotSolidTo((CThing*)*it);
    }
};

namespace _STL {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

template CAddObjectsNotSolidTo __fastcall for_each(CThingObject**, CThingObject**, CAddObjectsNotSolidTo);
}


static int g_calls = 0;
static CTCSolidToThing* g_who[16];
static CThing* g_what[16];
void CTCSolidToThing::AddThingNotSolidTo(CThing* thing) { g_who[g_calls] = this; g_what[g_calls] = thing; ++g_calls; }

static void Setup(CThingObject& o, CInterfaceMap::value_type* entries, CTCSolidToThing* solid)
{
    memset(&o, 0, sizeof(o));
    o.m_presentBits[77 >> 5] |= 1UL << (77 & 31);
    entries[0].first = 10; entries[0].second = 0;
    entries[1].first = 77; entries[1].second = (CTCBase*)solid;
    entries[2].first = 90; entries[2].second = 0;
    o.m_interfaces.First = entries;
    o.m_interfaces.Last = entries + 3;
    o.m_interfaces.EndOfStorage = entries + 3;
}

int main()
{
    static CThingObject a, b;
    static CInterfaceMap::value_type ea[3], eb[3];
    static CTCSolidToThing sa, sb;
    Setup(a, ea, &sa);
    Setup(b, eb, &sb);
    static CThingObject x, y;
    CThingObject* listed[2] = { &x, &y };
    _STL::vector<CThingObject*> objects;
    objects._M_start = listed; objects._M_finish = listed + 2; objects._M_end_of_storage = listed + 2;
    CThingObject* range[2] = { &a, &b };

    CAddObjectsNotSolidTo f = _STL::for_each(range, range + 2, CAddObjectsNotSolidTo(objects));
    int ok = 1;
    if (g_calls != 4) ok = 0;
    if (g_who[0] != &sa || g_who[1] != &sa || g_who[2] != &sb || g_who[3] != &sb) ok = 0;
    if (g_what[0] != (CThing*)&x || g_what[1] != (CThing*)&y || g_what[2] != (CThing*)&x || g_what[3] != (CThing*)&y) ok = 0;
    if (f.ObjectList != &objects) ok = 0;
    _STL::for_each(range, range, CAddObjectsNotSolidTo(objects));   // empty range
    if (g_calls != 4) ok = 0;

    printf(ok ? "0074fbb0_TEST PASS\n" : "0074fbb0_TEST FAIL\n");
    return ok ? 0 : 1;
}