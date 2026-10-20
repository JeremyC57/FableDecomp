// std::vector<SMusicTrack>::_M_insert_overflow(SMusicTrack* position, const SMusicTrack& x, const __false_type&, unsigned long fillLen, bool atEnd) -- STLport growth path of the music manager's 24-byte track vector (a CMusicEntry plus its name string): allocate old + max(old, fillLen) elements, copy the head, construct the fill (one inline copy or uninitialized_fill_n), copy the tail unless inserting at the end, destroy + free the old block and adopt the new one
// Grade: DIFFER(291v284) -- frame/register allocation: old size and length get separate words (retail shares one push-ecx word and homes the new start in the dead fillLen slot); fillLen in edi / new finish in ebx (retail ebx / edi)
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBC130, 284 bytes, `ret 14h`. Catalog label "CMusicEntry::LoadFromScript" is
//   propagated: the body is the vector's _M_insert_overflow, called from the push_back sites 0x00CB85CC / 0x00CBC7FE.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, STLport `__fastcall` __uninitialized_copy(first, last, result, const
//   __false_type&) 0x00CB9B00 and __uninitialized_fill_n(first, n, x, const __false_type&) 0x00CB9B50 (/Gr algorithm TU),
//   CCharString copy ctor 0x0099EC30 / dtor 0x0099EAE0.
// Layout: element 0x18 bytes: CMusicEntry (Ego_r.pdb: ID, Filename (CDefString), Gain, CacheToHD; 16 bytes) at +0, the
//   name CCharString at +0x10, four uncopied bytes at +0x14 (copy ctor copies the entry and the name only).
// Shape: STLport 4.x source: `(max)` through the const-ref template (old size homed in the `push ecx` word, the length
//   reuses it); the new start is homed in the dead fillLen slot; each `__false_type()` tag temporary is passed by address.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefString { long Index; };
struct CMusicEntry {
    unsigned long ID;                                   // +0x00
    CDefString    Filename;                             // +0x04
    float         Gain;                                 // +0x08
    bool          CacheToHD;                            // +0x0c
};
struct SMusicTrack {
    CMusicEntry   Entry;                                // +0x00
    CCharString   Name;                                 // +0x10
    unsigned long Spare;                                // +0x14 (not copied)
    SMusicTrack(const SMusicTrack& other) : Entry(other.Entry), Name(other.Name) { }
};

struct __false_type { __false_type() { } };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

SMusicTrack* __fastcall __uninitialized_copy(SMusicTrack* first, SMusicTrack* last, SMusicTrack* result, const __false_type&);   // 0x00CB9B00
SMusicTrack* __fastcall __uninitialized_fill_n(SMusicTrack* first, unsigned long n, const SMusicTrack& x, const __false_type&);  // 0x00CB9B50

inline void _Destroy(SMusicTrack* first, SMusicTrack* last)
{
    for (; first != last; ++first)
        first->~SMusicTrack();
}

template <class T> struct _STLP_alloc_proxy {
    T* _M_data;
    T* allocate(unsigned long n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned long) { if (p) free(p); }
};

struct CMusicTrackVector {
    SMusicTrack* _M_start;
    SMusicTrack* _M_finish;
    _STLP_alloc_proxy<SMusicTrack> _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    void _M_set(SMusicTrack* s, SMusicTrack* f, SMusicTrack* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage._M_data = e;
    }
    void _M_insert_overflow(SMusicTrack* position, const SMusicTrack& x, const __false_type&, unsigned long fillLen, bool atEnd);
};

void CMusicTrackVector::_M_insert_overflow(SMusicTrack* position, const SMusicTrack& x, const __false_type&, unsigned long fillLen, bool atEnd)
{
    const unsigned long oldSize = size();
    const unsigned long len = oldSize + (max)(oldSize, fillLen);
    SMusicTrack* newStart = _M_end_of_storage.allocate(len);
    SMusicTrack* newFinish = newStart;
    {
        newFinish = __uninitialized_copy(_M_start, position, newStart, __false_type());
        if (fillLen == 1) {
            new (newFinish) SMusicTrack(x);
            ++newFinish;
        } else
            newFinish = __uninitialized_fill_n(newFinish, fillLen, x, __false_type());
        if (!atEnd)
            newFinish = __uninitialized_copy(position, _M_finish, newFinish, __false_type());
    }
    _M_clear();
    _M_set(newStart, newFinish, newStart + len);
}
