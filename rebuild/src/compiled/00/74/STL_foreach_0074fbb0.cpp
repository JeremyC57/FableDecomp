// _STL::for_each(CThingObject** first, CThingObject** last, CAddObjectsNotSolidTo f) -- make every listed object not solid to each object in the range (through its CTCSolidToThing); returns the functor
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- GetTC reads the map's Last into a `last` local shared by a __forceinline lower_bound(first, last, key) helper and the end test, so VC7.1 loads Last before First and keeps it in edi as retail does
// Leaf: ??$for_each@PAPAVCThingObject@@VCAddObjectsNotSolidTo@@@_STL@@YI?AVCAddObjectsNotSolidTo@@PAPAVCThingObject@@0V1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074FBB0, 185 bytes, `ret 8` (link-time __fastcall: the hidden result
//   pointer in ecx, first in edx, then last and the 4-byte functor on the stack).
// Callee: CTCSolidToThing::AddThingNotSolidTo(CThing*) @ 0x00770FA0 (per listed object).
// Caller: 0x0074E827.
// Label: Ego_r.pdb names the Dinkumware twin `std::_For_each<CThingObject**, CAddObjectsNotSolidTo>`; this is STLport's
//   `for_each` (functor returned through the hidden pointer: CAddObjectsNotSolidTo has a user constructor, Ego_r.pdb
//   `??0CAddObjectsNotSolidTo@@QAE@ABV?$vector@PAVCThingObject@@...@@@Z`, ObjectList +0).
// Shape: `for (; first != last; ++first) f(*first); return f;` with `operator()(CThingObject* object)` inlined: the
//   object's CTCSolidToThing (interface type 77 = 0x4D: present bit 13 of the +0x28 word, then the interface map's
//   lower_bound len/half walk inline over (First, last) with `last` read first and shared with the end test,
//   `it == last || 77 < it->first` -> last) gets AddThingNotSolidTo for every entry of
//   *ObjectList (end cached); on the no-TC path the TC register is left as is (the lookup has no else value).
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