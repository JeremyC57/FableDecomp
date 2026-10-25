// NThingMap::CThingMapManager::GetAllActiveThingMapsInArea(const C2DBoxI& area, _STL::vector<CThingMap*>& maps) const -- collect every active thing map whose area overlaps the given box
// Leaf: ?GetAllActiveThingMapsInArea@
// Grade: RELOCATION_MATCH (no /Oa) -- same-TU noinline stand-ins: a read-only C2DBoxI::Overlapping (VC7.1 then keeps info->PThingMap in edi across the call, as retail; this was the /Oa effect) and vector::resize(n, x) (the null temporary's address no longer escapes), with the real _M_insert_overflow body. Earlier: /Oa 133v132 with ebx/ebp swapped, plain 127v132.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006384B0, 132 bytes (+ int3 padding), `ret 8`.
// Identity: Ego_r.pdb `?GetAllActiveThingMapsInArea@CThingMapManager@NThingMap@@QBEXABVC2DBoxI@@AAV?$vector@PAVCThingMap@
// NThingMap@@...@Z`; the body walks ActiveThingMaps (+0x14/+0x18) and tests each map's area. The catalog label
// "CThingMapManager_GetOverlappingThings" is descriptive.
// Callees: vector<CThingMap*>::resize(size_type, const value_type&) @ 0x00639730 (out of line; catalogued as the propagated
// vector<unsigned int>::resize), C2DBoxI::Overlapping(const C2DBoxI&) const @ 0x004FAB40 (out of line, `ret 4`), the vector's
// STLport _M_insert_overflow(pos, x, const __true_type&, size_type, bool) @ 0x00639170 (`ret 14h`).
// Layout (retail; engine/CThingMapManager.h carries the Ego_r.pdb CArray offsets, whose containers are 4 bytes wider, so it
// does not fit -- the landed GetThingMapAt 0x00637A80 / AddGameMapWithArea 0x006381A0 models): ActiveThingMaps
// {first, last} at +0x14/+0x18, each entry a CThingMapInfo whose first member is the CThingMap; the map's area at +0x20.
// Shape: `maps.resize(0)` is STLport's inline `resize(n) { resize(n, T()); }` (the null pointer temporary in a frame word);
// `this` is parked in a frame word because esi carries the entry; the box is re-read from its argument slot per pass and
// the end pointer re-read from `this`. push_back is inlined: the in-place copy behind the placement-new null test and
// `add [finish],4`, else _M_insert_overflow with x bound straight to the entry's PThingMap member and the empty
// __true_type tag homed in the dead `maps` argument slot (needs the real _M_insert_overflow body out of line in the TU).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    bool Overlapping(const C2DBoxI& other) const;      // 0x004FAB40
};

namespace _STL {

class __true_type { public: __true_type() {} };

template <class T> class allocator { public: allocator() {} };

template <class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08

    typedef const T* const_iterator;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }

    void resize(unsigned int new_size, const T& x);      // 0x00639730
    void resize(unsigned int new_size) { resize(new_size, T()); }

    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x00639170

    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

// The real STLport _M_insert_overflow body (0x00639170 for T = CThingMap*), out of line in the TU.
#pragma auto_inline(off)
template <class T, class Alloc>
__declspec(noinline) void vector<T, Alloc>::_M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int n, bool atend)
{
    unsigned int oldSize = _M_finish - _M_start;
    unsigned int len = oldSize + (oldSize < n ? n : oldSize);
    T* ns = len ? (T*)malloc(len * sizeof(T)) : 0;
    T* nf = ns;
    T* p;
    for (p = _M_start; p != pos; ++p) { *nf++ = *p; }
    for (; n != 0; --n) { *nf++ = x; }
    if (!atend)
        for (p = pos; p != _M_finish; ++p) { *nf++ = *p; }
    if (_M_start)
        free(_M_start);
    _M_start = ns;
    _M_finish = nf;
    _M_end_of_storage = ns + len;
}
// Same-TU stand-in for vector::resize(n, x) 0x00639730 (truncates or appends copies; never keeps `x`).
template <class T, class Alloc>
__declspec(noinline) void vector<T, Alloc>::resize(unsigned int new_size, const T& x)
{
    if (new_size < (unsigned int)(_M_finish - _M_start))
        _M_finish = _M_start + new_size;
    else
        while ((unsigned int)(_M_finish - _M_start) < new_size)
            push_back(x);
}
#pragma auto_inline(on)

}

#pragma auto_inline(off)
// Same-TU stand-in for C2DBoxI::Overlapping 0x004FAB40 (read-only).
__declspec(noinline) bool C2DBoxI::Overlapping(const C2DBoxI& other) const
{
    return TLX <= other.BRX && other.TLX <= BRX && TLY <= other.BRY && other.TLY <= BRY;
}
#pragma auto_inline(on)

namespace NThingMap {

class CThingMap {
public:
    unsigned char _pad00[0x20];
    C2DBoxI       Area;                         // +0x20
};

class CThingMapManager {
public:
    class CThingMapInfo {
    public:
        CThingMap* PThingMap;                   // +0x00
        long       RefCount;                    // +0x04
    };
    void*           __vftable;                  // +0x00
    void*           PWorld;                     // +0x04
    unsigned char   ThingMaps[12];              // +0x08 (retail 12-byte container)
    _STL::vector<CThingMapInfo*> ActiveThingMaps;   // +0x14
    void GetAllActiveThingMapsInArea(const C2DBoxI& area, _STL::vector<CThingMap*>& maps) const;
};

void CThingMapManager::GetAllActiveThingMapsInArea(const C2DBoxI& area, _STL::vector<CThingMap*>& maps) const
{
    maps.resize(0);
    for (_STL::vector<CThingMapInfo*>::const_iterator it = ActiveThingMaps.begin(); it != ActiveThingMaps.end(); ++it) {
        CThingMapInfo* info = *it;
        if (info->PThingMap->Area.Overlapping(area))
            maps.push_back(info->PThingMap);
    }
}

}