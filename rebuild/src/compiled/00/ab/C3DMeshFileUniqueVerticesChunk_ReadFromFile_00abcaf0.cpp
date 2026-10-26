// C3DMeshFileUniqueVerticesChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the unique-vertex index list: a count, then that many unsigned longs
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABCAF0, 289 bytes (virtual, `ret 0Ch`); byte twin of 0x00ABC980 (the smoothing-groups chunk).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inlined stream Read's slow path), memmove import
// 0x00BFEAE6 (the inlined clear()'s trivial copy), vector<unsigned long>::reserve 0x009B4030, the reallocating insert
// _M_insert_overflow 0x00464ACE (push_back when full).
// Shape: the count read through a by-value ReadULong() (lands in ebp / the dead `in` slot); UniqueVerticesList.clear() as
// STLport erase(begin(), end()) over STLport's own `__copy_trivial` (`last == first ? result : memmove(...) + n`, so VC
// compares the two equal pointers in one register: `cmp eax,eax`); reserve(count); a counted loop (`dec ebp ; jne`) reading each value (the by-value ReadULong() result) and
// push_back'ing it (null-tested placement store, or the overflow insert with a `__true_type` tag and fill length 1).
// The read value and the tag share the dead `in` slot (push ecx frame) only with a same-TU noinline stand-in for
// _M_insert_overflow (the const-ref taker of the pushed value).
// Layout: Ego_r.pdb UniqueVerticesList +0x10 (CArray<unsigned long>, retail 12-byte {start, finish, end_of_storage}).
// Leaf: ReadFromFile@C3DMeshFileUniqueVerticesChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6
#pragma intrinsic(memcpy)

class CChunkedFileLoader;
class CDataInputStream { public:
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
};

struct __true_type { __true_type() {} };

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}

template <class T> class CArray { public:                 // retail STLport vector layout
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void reserve(unsigned int n);                  // 0x009B4030
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fillLength, bool atEnd);   // 0x00464ACE
    T* erase(T* first, T* last)
    {
        T* i = (T*)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(_M_start, _M_finish); }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
// Same-TU stand-in for _M_insert_overflow 0x00464ACE (not the retail body).
void InsertOverflow_00464ace(void* v, void* position, const void* x, unsigned int fill);
#pragma auto_inline(off)
template <class T> __declspec(noinline) void CArray<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fillLength, bool atEnd)
{
    InsertOverflow_00464ace(this, position, &x, fillLength);
}
#pragma auto_inline(on)

class C3DMeshFileUniqueVerticesChunk { public:
    virtual void v0();
    unsigned char _base_body[0x0c];        // CChunkedFileChunk: ChunkType[5] + pad + Parent
    CArray<unsigned long> UniqueVerticesList;  // +0x10
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileUniqueVerticesChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long count = in.ReadULong();
    UniqueVerticesList.clear();
    UniqueVerticesList.reserve(count);
    for (unsigned long i = 0; i < count; ++i)
        UniqueVerticesList.push_back(in.ReadULong());
}