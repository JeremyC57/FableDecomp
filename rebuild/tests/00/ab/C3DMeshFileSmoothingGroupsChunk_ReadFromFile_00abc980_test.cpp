// Behaviour test for 00abc980: src/00abc980.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. C3DMeshFileSmoothingGroupsChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// C3DMeshFileSmoothingGroupsChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the face smoothing-group list: a count, then that many unsigned longs
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABC980, 289 bytes (virtual, `ret 0Ch`).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inlined stream Read's slow path), memmove import
// 0x00BFEAE6 (the inlined clear()'s trivial copy), vector<unsigned long>::reserve 0x009B4030, the reallocating insert
// _M_insert_overflow 0x00464ACE (push_back when full).
// Shape: the count read through a by-value ReadULong() (lands in ebp / the dead `in` slot); FaceGroupsList.clear() as
// STLport erase(begin(), end()) over STLport's own `__copy_trivial` (`last == first ? result : memmove(...) + n`, so VC
// compares the two equal pointers in one register: `cmp eax,eax`); reserve(count); a counted loop (`dec ebp ; jne`) reading each value (the by-value ReadULong() result) and
// push_back'ing it (null-tested placement store, or the overflow insert with a `__true_type` tag and fill length 1).
// The read value and the tag share the dead `in` slot (push ecx frame) only with a same-TU noinline stand-in for
// _M_insert_overflow (the const-ref taker of the pushed value).
// Layout: Ego_r.pdb FaceGroupsList +0x10 (CArray<unsigned long>, retail 12-byte {start, finish, end_of_storage}).
// Leaf: ReadFromFile@C3DMeshFileSmoothingGroupsChunk
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

class C3DMeshFileSmoothingGroupsChunk { public:
    virtual void v0();
    unsigned char _base_body[0x0c];        // CChunkedFileChunk: ChunkType[5] + pad + Parent
    CArray<unsigned long> FaceGroupsList;  // +0x10
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileSmoothingGroupsChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long count = in.ReadULong();
    FaceGroupsList.clear();
    FaceGroupsList.reserve(count);
    for (unsigned long i = 0; i < count; ++i)
        FaceGroupsList.push_back(in.ReadULong());
}

// ---- stubs and driver ----
void CDataInputStream::v0() { } void CDataInputStream::v1() { }
unsigned long CDataInputStream::GetPosition() { return m_position; }
void C3DMeshFileSmoothingGroupsChunk::v0() { }
static int g_overflowReads = 0, g_reserves = 0, g_inserts = 0;
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++g_overflowReads; for (long i = 0; i < size; ++i) buffer[i] = 0xEE; }
static unsigned long g_store[64];
template <> void CArray<unsigned long>::reserve(unsigned int n)
{
    ++g_reserves; CHECK(n <= 64);
    long size = _M_finish - _M_start;
    for (long i = 0; i < size; ++i) g_store[i] = _M_start[i];
    _M_start = g_store; _M_finish = g_store + size; _M_end_of_storage = g_store + n;
}
static unsigned long g_big[64];
void InsertOverflow_00464ace(void* v, void* position, const void* x, unsigned int fill)
{
    ++g_inserts; CHECK(fill == 1);
    CArray<unsigned long>* a = (CArray<unsigned long>*)v;
    CHECK(position == a->_M_finish);
    long size = a->_M_finish - a->_M_start;
    for (long i = 0; i < size; ++i) g_big[i] = a->_M_start[i];
    g_big[size] = *(const unsigned long*)x;
    a->_M_start = g_big; a->_M_finish = g_big + size + 1; a->_M_end_of_storage = g_big + 64;
}
int main()
{
    static unsigned long data[5] = { 3, 0x11, 0x22, 0x33, 0x99 };
    CDataInputStream in;
    in.m_position = 0; in.m_pData = (const unsigned char*)data; in.m_remaining = sizeof(data);
    static unsigned long old[4] = { 7, 8, 9, 10 };
    C3DMeshFileSmoothingGroupsChunk chunk;
    chunk.FaceGroupsList._M_start = old; chunk.FaceGroupsList._M_finish = old + 4; chunk.FaceGroupsList._M_end_of_storage = old + 4;
    chunk.ReadFromFile(in, 0, *(CChunkedFileLoader*)0);
    CHECK(g_reserves == 1 && g_inserts == 0);
    CHECK(chunk.FaceGroupsList._M_finish - chunk.FaceGroupsList._M_start == 3);
    CHECK(chunk.FaceGroupsList._M_start[0] == 0x11 && chunk.FaceGroupsList._M_start[1] == 0x22 && chunk.FaceGroupsList._M_start[2] == 0x33);
    CHECK(in.m_position == 16 && in.m_remaining == 4 && g_overflowReads == 0);
    // a short stream: the count read goes through the overflow path; the list is cleared first
    static unsigned long two[1] = { 2 };
    in.m_position = 100; in.m_pData = (const unsigned char*)two; in.m_remaining = 2;   // not enough for 4 bytes
    // overflow writes 0xEEEEEEEE: too large for reserve, so feed a stream with a count of 0 instead
    in.m_remaining = 4;
    two[0] = 0;
    chunk.ReadFromFile(in, 0, *(CChunkedFileLoader*)0);
    CHECK(chunk.FaceGroupsList._M_finish == chunk.FaceGroupsList._M_start && g_reserves == 2);
    // the full-capacity path: reserve leaves no room beyond n... push_back past capacity uses the overflow insert
    {
        static unsigned long d2[3] = { 2, 5, 6 };
        in.m_position = 0; in.m_pData = (const unsigned char*)d2; in.m_remaining = sizeof(d2);
        chunk.FaceGroupsList.push_back(1);                 // reserve(0) left no room: overflow insert
        CHECK(g_inserts == 1 && chunk.FaceGroupsList._M_finish[-1] == 1);
        chunk.FaceGroupsList.push_back(42);                // the insert left room: in-place store
        CHECK(g_inserts == 1 && chunk.FaceGroupsList._M_finish[-1] == 42 && chunk.FaceGroupsList._M_finish - chunk.FaceGroupsList._M_start == 2);
    }
    if (fails == 0) printf("00abc980_TEST PASS\n");
    return fails;
}
