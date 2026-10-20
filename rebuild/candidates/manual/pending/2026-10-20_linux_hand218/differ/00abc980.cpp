// C3DMeshFileSmoothingGroupsChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the face smoothing-group list: a count, then that many unsigned longs
// Grade: DIFFER(ours v retail) -- frame: ours opens sub esp,8 (loop read value and push_back temp in separate frame words, tag in the arg slot), retail push ecx (read value and __true_type tag share the dead in slot); this kept in edi with lea ebx,[edi+10h] vs retail add ebx,10h
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABC980, 289 bytes (virtual, `ret 0Ch`).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inlined stream Read's slow path), memmove import
// 0x00BFEAE6 (the inlined clear()'s trivial copy), vector<unsigned long>::reserve 0x009B4030, the reallocating insert
// _M_insert_overflow 0x00464ACE (push_back when full).
// Shape: the count read through a by-value ReadULong() (lands in ebp / the dead `in` slot); FaceGroupsList.clear() as
// STLport erase(begin(), end()) over `__copy_trivial` (`n ? memmove(...) + n : result`, VC tests `last != first` on one
// register: `cmp eax,eax`); reserve(count); a counted loop (`dec ebp ; jne`) reading each value into a named local and
// push_back'ing it (null-tested placement store, or the overflow insert with a `__true_type` tag and fill length 1).
// Layout: Ego_r.pdb FaceGroupsList +0x10 (CArray<unsigned long>, retail 12-byte {start, finish, end_of_storage}).
// Leaf: ReadFromFile@C3DMeshFileSmoothingGroupsChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC
    long m_10;
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
    unsigned int n = (const char*)last - (const char*)first;
    return n ? (void*)((char*)memmove(result, first, n) + n) : result;
}

template <class T> struct CArray {                 // retail STLport vector layout
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

struct C3DMeshFileSmoothingGroupsChunk {
    virtual void v0();
    unsigned char _base_body[0x0c];        // CChunkedFileChunk: ChunkType[5] + pad + Parent
    CArray<unsigned long> FaceGroupsList;  // +0x10
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileSmoothingGroupsChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long count = in.ReadULong();
    CArray<unsigned long>& list = FaceGroupsList;
    list.clear();
    list.reserve(count);
    for (unsigned long i = 0; i < count; ++i)
        list.push_back(in.ReadULong());
}
