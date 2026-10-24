// CVectorMap<unsigned long,CSoundPair,CKeyPairCompareLess<unsigned long,CSoundPair> >::Insert(unsigned long key, const CSoundPair& value) -- appends {key, value} to the sorted sound-pair map's vector, marks it dirty, then re-sorts at once and clears the flag
// Leaf: ?Insert@?$CVectorMap@
// Retail: Fable.exe @ 0x0081B220, 116 bytes (+ int3 padding), `ret 8`: __fastcall (this -> esi), the key BY VALUE in
//   the first stack slot (`mov ecx,[esp+18h]` reads the slot itself), the value reference in the second.
// No callers (an unreferenced COMDAT). FableWin.pdb names the member `Insert(const unsigned long&, const CSoundPair&)`
//   (the Xbox build); the PC build reads the key straight from its slot, so its key is a value parameter here.
// Callees: vector<pair<unsigned long,CSoundPair> >::_M_insert_overflow(pos, const T&, const __true_type&, size_type,
//   bool) 0x0081CA90 (same-TU stand-in below) and the SGI sort(first, last, CKeyPairCompareLess) 0x0081E0F0 (a /Gr
//   algorithm TU: first in ecx, last in edx, the 1-byte comparator zero-extended and pushed).
// Label: catalogued `std_vector_push_back_SoundPair` (descriptive). Ego_r.pdb CSoundPair: long FirstSound, LastSound.
// Layout (CVectorMap): the STLport vector +0x00, the empty Compare +0x0c, bool Dirty +0x0d.
// Shape: the inline push_back of the pair temporary (placement-new null test, `add [esi+4],0Ch`; the overflow's empty
//   __true_type temporary homed on the dead key slot -- GOTCHA 1276, which needs the same-TU auto_inline(off)/noinline
//   overflow body that never touches its tag), `Dirty = true`, then the inline Sort (sort + `Dirty = false`).
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
inline void* operator new(unsigned int, void* where) throw() { return where; }

class CSoundPair {
public:
    long FirstSound;                                         // +0x0
    long LastSound;                                          // +0x4
};

namespace std {
struct __true_type { __true_type() { } };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) { }   // STLport's explicit pair copy constructor
};

template <class T> class vector {
public:
    typedef T value_type;
    T* _M_start;                                             // +0x00
    T* _M_finish;                                            // +0x04
    T* _M_end_of_storage;                                    // +0x08
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend);
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};

// Same-TU stand-in for the out-of-line STLport growth path 0x0081CA90 (it reads x only through its reference and never
// touches the tag, so the caller homes the tag temporary on a dead argument slot as retail does).
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&,
                                                        unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);
    T* new_start = (T*)malloc(len * sizeof(T));
    T* new_finish = new_start;
    for (T* p = _M_start; p != position; ++p, ++new_finish)
        new (new_finish) T(*p);
    for (unsigned int n = fill_len; n > 0; --n, ++new_finish)
        new (new_finish) T(x);
    if (!atend)
        for (T* q = position; q != _M_finish; ++q, ++new_finish)
            new (new_finish) T(*q);
    if (_M_start)
        free(_M_start);
    _M_start = new_start;
    _M_finish = new_finish;
    _M_end_of_storage = new_start + len;
}
#pragma auto_inline()

template <class RandomAccessIter, class Compare>
void __fastcall sort(RandomAccessIter first, RandomAccessIter last, Compare comp);   // 0x0081E0F0
}

template <class Key, class Value>
class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

template <class Key, class Value, class Compare>
class CVectorMap {
public:
    std::vector<std::pair<Key, Value> > Data;                // +0x00
    Compare                             CompareFunc;         // +0x0c (empty)
    bool                                Dirty;               // +0x0d

    void Sort()
    {
        std::sort(Data.begin(), Data.end(), CompareFunc);
        Dirty = false;
    }
    void Insert(Key key, const Value& value);
};

template <class Key, class Value, class Compare>
void CVectorMap<Key, Value, Compare>::Insert(Key key, const Value& value)
{
    Data.push_back(std::pair<Key, Value>(key, value));
    Dirty = true;
    Sort();
}

template class CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<unsigned long, CSoundPair> >;