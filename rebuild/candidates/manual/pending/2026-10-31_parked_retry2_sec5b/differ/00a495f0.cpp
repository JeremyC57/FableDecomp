// CLipSyncSampleData::ReadFromFile(CDataInputStream& stream) -- read a lip-sync sample: the phoneme-name count and (id byte, null-terminated name) pairs into a temporary map<unsigned long, CCharString>, the frames-per-second (unsigned -> float), the frame count (resizing FrameData), then per frame a byte count of (phoneme id byte, weight byte / 255 clamped to [0,1]) phonemes, each named from the map and pushed into the frame
// Grade: DIFFER(1261v1261) 11/433 -- every instruction matches except stack-slot packing of three one-byte objects: retail puts the phoneme id / weight read buffers at [esp+12h]/[esp+13h] and the push_back `__false_type` tag in the dead stream-argument slot; here they land at [esp+11h]/[esp+12h]/[esp+13h] (6 levers inert: POD tag, named tag, explicit byte locals, map ctor default-argument temps with and without ctors, unsigned char count, array buffers)
// Leaf: ?ReadFromFile@CLipSyncSampleData@@QAEXAAVCDataInputStream@@@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A495F0, 1261 bytes, `ret 4`.
// Name: Ego_r.pdb + FableWin.pdb ?ReadFromFile@CLipSyncSampleData@@QAEXAAVCDataInputStream@@@Z (manifest row; callers
//   of the landed map operator[] 0x00A4A670 name it).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (inline Read template, as landed CParticleSystem::ReadBinary
//   0x00AF4660), the stream's virtual ReadNullTerminatedString (slot 6); map<unsigned long, CCharString>::operator[]
//   0x00A4A670, its _M_erase 0x00630D90 (shared body); vector<CLipSyncPhoneme>::operator= 0x00A4A520 / _M_insert_overflow
//   0x00A4A710, vector<CLipSyncFrameData>::_M_fill_insert 0x00A4B020, __destroy_aux 0x00A43F80 (`__fastcall`); CCharString
//   ctor/copy/dtor/operator=, malloc/free.
// Layout: Ego_r.pdb CLipSyncSampleData { CArray<CLipSyncFrameData> FrameData; float FramesPerSecond; } with retail
//   12-byte STLport vectors (FramesPerSecond +0x0C); CLipSyncPhoneme { CCharString Symbol; unsigned long ID; float Weight; }.
// Shape: reads through inline ReadULong/ReadUByte helpers (their buffers share the dead stream-argument slot, the
//   values stay in registers); the name is a named local (`CCharString name = ...; names[key] = name;`, 29 -> 11);
//   STLport map is 12 bytes (header, count, empty key_compare -- 135 -> 29); the resize is inline with STLport's layered
//   erase (__copy_ptrs -> inline __copy: the zero-length tail copy loop survives, as retail) and the default value's
//   destructor folds only because _M_fill_insert has a same-TU noinline stand-in that never writes through the value
//   (the landed resize(n) 0x00A4B1D0 idiom) and __destroy_aux has a same-TU `__fastcall` body.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CDataInputStream { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadNullTerminatedString();     // slot 6 (+0x18)
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size)
    {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    __forceinline unsigned char ReadUByte() { unsigned char value; Read(&value, 1); return value; }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, 4); return value; }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};
namespace std {
struct __false_type { __false_type() { } };
struct __true_type { __true_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { _Destroy(_M_start, _M_finish); if (_M_start) free(_M_start); }
    vector& operator=(const vector& x);
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type n) { return _M_start[n]; }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy_ptrs(__last, _M_finish, __first, __false_type());
        _Destroy(__i, _M_finish);
        _M_finish = __i;
        return __first;
    }
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) _Tp(__x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
    void _M_insert_overflow(iterator __pos, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend);
};
}
inline void* operator new(unsigned int, void* p) { return p; }
// _M_fill_insert (out of line in retail, 0x00A4B020 for vector<CLipSyncFrameData>): a same-TU noinline stand-in (not graded)
// that, like the retail body, reads the value but never writes through the reference -- VC7.1's mod-ref analysis then folds
// the default value's inline destructor (the landed resize(n) 0x00A4B1D0 idiom).
namespace std { unsigned long g_fill_value_sink; }
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_fill_insert(iterator __pos, size_type __n, const _Tp& __x)
{
    if (__n != 0)
        g_fill_value_sink = *(const unsigned long*)&__x + (unsigned long)__pos;
}
#pragma auto_inline(on)
class CLipSyncPhoneme { public:
    CCharString Symbol;                                 // +0
    unsigned long ID;                                   // +4
    float Weight;                                       // +8
    CLipSyncPhoneme() : ID(0), Weight(0.0f) { }
};
template <class T> class CArray : public std::vector<T> { };
class CLipSyncFrameData { public:
    CArray<CLipSyncPhoneme> Phonemes;                   // +0x00 (12 bytes in retail)
};
struct CNameNode {
    bool _M_color;
    CNameNode* _M_parent;
    CNameNode* _M_left;
    CNameNode* _M_right;
    unsigned long first;
    CCharString second;
};
class CNameMap { public:                                // std::map<unsigned long, CCharString> (STLport)
    CNameNode* _M_header;
    unsigned long _M_node_count;
    struct less { } _M_key_compare;
    CNameMap()
    {
        _M_header = 0;
        _M_header = (CNameNode*)malloc(sizeof(CNameNode));
        _M_node_count = 0;
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    ~CNameMap()
    {
        clear();
        if (_M_header)
            free(_M_header);
    }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    void _M_erase(CNameNode* x);                        // 0x00630D90
    CCharString& operator[](const unsigned long& k);    // 0x00A4A670
};
class CLipSyncSampleData { public:
    CArray<CLipSyncFrameData> FrameData;                // +0x00 (12 bytes in retail)
    float FramesPerSecond;                              // +0x0c
    void ReadFromFile(CDataInputStream& stream);
};
void CLipSyncSampleData::ReadFromFile(CDataInputStream& stream)
{
    CNameMap names;
    unsigned long count = stream.ReadULong();
    for (unsigned long i = 0; i < count; ++i) {
        unsigned long key = stream.ReadUByte();
        CCharString name = stream.ReadNullTerminatedString();
        names[key] = name;
    }
    FramesPerSecond = (float)stream.ReadULong();
    FrameData.resize(stream.ReadULong());
    for (unsigned long f = 0; f < FrameData.size(); ++f) {
        CLipSyncFrameData& frame = FrameData[f];
        unsigned long n = stream.ReadUByte();
        for (unsigned long k = 0; k < n; ++k) {
            CLipSyncPhoneme phoneme;
            phoneme.ID = stream.ReadUByte();
            phoneme.Weight = (float)stream.ReadUByte() * (1.0f / 255.0f);
            if (phoneme.Weight < 0.0f)
                phoneme.Weight = 0.0f;
            else if (phoneme.Weight > 1.0f)
                phoneme.Weight = 1.0f;
            phoneme.Symbol = names[phoneme.ID];
            frame.Phonemes.push_back(phoneme);
        }
    }
}
