// GFSerialiseMapBinaryIn(CDataInputStream& stream, map<long, CComboMultiplierData>& out) -- binary persist read of the hero's combo-multiplier map: clear it, read the count, then per entry read the key and the combo data and insert the pair
// Leaf: ??$GFSerialiseMapBinaryIn@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DCFA0, 532 bytes (+ int3 padding to 544), `ret` (__fastcall: the
//   stream in ecx, the map in edx).
// Identity: the catalog's "CHeroExperienceDef_ReadTreeFromStream" is a descriptive auto label. Reader twin of the writer
//   0x006DC350 (this chunk's row) for CHeroExperienceDef::ComboMultiplierMap, map<long, CComboMultiplierData>; the value
//   traits 0x006DBE00 (CPersistTraits<CComboMultiplierData>::TransferBinaryIn, landed) is the per-entry reader.
// Callees: malloc 0x00BFEA0E (the DefaultVal / pair / value_type inner-map headers), _Rb_tree<long, ..>::_M_erase
//   0x004E6E0C (out.clear()), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the count's slow path),
//   CPersistTraits<long>::TransferBinaryIn 0x0040FE60, CPersistTraits<CComboMultiplierData>::TransferBinaryIn 0x006DBE00,
//   the inner map's _Rb_tree copy constructor 0x004F97B3 (the value_type copy), map::insert(const value_type&)
//   0x006DCDD0, _Rb_tree<EDamageAttribute, ..>::_M_erase 0x00450903 and free 0x00BFEA14 (the three inner-map destructors).
// Shape: the map reader template (twin of 0x006DBA50): the value traits carries a DefaultVal CComboMultiplierData, so it
//   is a frame local built first and destroyed last; inline clear(); the count through the inlined memory-stream read
//   (fast path into ebp); the loop value-initialises the pair (key 0, the inner map built), reads the key through a
//   long-traits temporary (its 4-byte DefaultVal) homed on the dead count slot, the value through the traits local, then inserts a value_type
//   copy (the implicit CComboMultiplierData copy: out-of-line map copy + two float moves) and destroys both pairs.
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

class CDataInputStream { friend struct CInputStreamMethods;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
struct CInputStreamMethods : CMemoryDataInputStream {
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T> struct less { };
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
struct _Rb_tree_iterator { _Rb_tree_node_base* _M_node; };
template <class V> struct _Rb_tree_base {
    typedef _Rb_tree_node<V> _Node;
    _Rb_tree_node_base* _M_header;                          // +0x00
    _Rb_tree_base() : _M_header(0) { _M_header = (_Node*)malloc(sizeof(_Node)); }
    ~_Rb_tree_base() { if (_M_header != 0) free(_M_header); }
};
template <class K, class V, class C = less<K> > class map : public _Rb_tree_base<pair<const K, V> > {
public:
    typedef pair<const K, V> value_type;
    unsigned long       _M_node_count;                      // +0x04
    C                   _M_key_compare;                     // +0x08
    map() : _M_node_count(0)
    {
        this->_M_header->_M_color = false;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
    map(const map& x);                                      // 0x004F97B3 (map<EDamageAttribute, float>)
    ~map() { clear(); }
    void _M_erase(_Rb_tree_node_base* x);                   // 0x004E6E0C (outer) / 0x00450903 (inner)
    pair<_Rb_tree_iterator, bool> insert(const value_type& value);   // 0x006DCDD0
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(this->_M_header->_M_parent);
            this->_M_header->_M_left = this->_M_header;
            this->_M_header->_M_parent = 0;
            this->_M_header->_M_right = this->_M_header;
            _M_node_count = 0;
        }
    }
};
}

class CComboMultiplierData {                                // Ego_r.pdb, 0x14 bytes with the 12-byte STLport map
public:
    std::map<EDamageAttribute, float> ComboMultiplier;      // +0x00
    float ExperienceMultiplierNumSecondsForAttenuation;     // +0x0C
    float ExperienceMultiplierParamDecayDeltaPerSecond;     // +0x10
};

template <class T> struct CPersistTraits {                 // Ego_r.pdb: CPersistTraits<long> = {long DefaultVal}
    T DefaultVal;                                           // +0x00
    void TransferBinaryIn(CInputStreamMethods& stream, T* out) const;   // 0x0040FE60 (long), 0x006DBE00 (combo data)
};

// Same-TU stand-in (auto_inline off, noinline) for the out-of-line long traits 0x0040FE60: it reads only the stream and
// writes only through `out`, which lets VC7.1 home the 4-byte traits temporary on the dead count slot.
#pragma auto_inline(off)
template <> __declspec(noinline) void CPersistTraits<long>::TransferBinaryIn(CInputStreamMethods& stream, long* out) const
{
    *out = stream.position;
}
#pragma auto_inline(on)

__forceinline void ReadCount(CInputStreamMethods& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CInputStreamMethods& stream, std::map<K, V, C>& out)
{
    CPersistTraits<V> valueTraits;
    out.clear();
    long count;
    ReadCount(stream, &count);
    for (; count > 0; --count) {
        std::pair<K, V> value;
        CPersistTraits<K>().TransferBinaryIn(stream, &value.first);
        valueTraits.TransferBinaryIn(stream, &value.second);
        out.insert(typename std::map<K, V, C>::value_type(value.first, value.second));
    }
}

template void __fastcall GFSerialiseMapBinaryIn<CComboMultiplierData, long, std::less<long> >(
    CInputStreamMethods& stream, std::map<long, CComboMultiplierData, std::less<long> >& out);