// Behaviour test for 00abd280: src/00abd280.cpp copied verbatim (including its real resize/_M_fill_insert/_M_insert_overflow
// bodies); stream virtuals, the overflow reader, the fastcall STLport helpers and the super chunk reader are stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// C3DMeshFileTrianglesChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the triangle count and the raw triangle array, then the super chunk's sub chunks
// Grade: RELOCATION_MATCH (retry helper C) -- the resize default T() temp shares the start position's frame word (sub esp,8;
// lea edx,[esp+10h]) once the TU holds the REAL resize chain: resize 0x00A97A50, _M_fill_insert 0x00A96F00 and
// _M_insert_overflow 0x00A71550 as noinline STLport bodies, so VC sees the never-written temp's address reach no extern
// (only x_copy goes to the out-of-line fill/copy helpers). With any extern callee taking x the temp got its own slot
// (was DIFFER: sub esp,0Ch; lea edx,[esp+16h]).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABD280, 308 bytes (virtual, `ret 0Ch`).
// Callees: CDataInputStream::GetPosition (vtable slot 2), ReadWithSrcChunkOverflow 0x00993CA0 (the inlined stream Read's
// slow path), vector<C3DTriangle2>::resize(n, const T&) 0x00A97A50 (manifest "vector<CBrainBehaviour>::resize" is a
// propagated byte twin), CChunkedFileSuperChunk::ReadFromFile 0x00AA8830.
// Shape: start position, a by-value ReadULong() count (homed in the dead `in` slot), TriangleList.resize(count) through
// the one-argument inline `resize(n) { resize(n, T()); }`, then one inlined stream Read of size() * 6 bytes into the
// array data (`size() ? _M_start : (T*)this`), the variable-length memcpy intrinsic (`rep movsd ; rep movsb`) on the
// fast path; the rest of the chunk goes to the base reader (`chunkSize - (GetPosition() - start)`).
// Layout: Ego_r.pdb TriangleList +0x34; retail +0x2C (the super chunk base 8 bytes smaller: 12-byte containers), so a
// local model; C3DTriangle2 is three unsigned shorts.
// Leaf: ReadFromFile@C3DMeshFileTrianglesChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
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
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= m_remaining) {
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

struct C3DTriangle2 {
    unsigned short Index[3];
    C3DTriangle2() {}
};

struct random_access_iterator_tag {};
C3DTriangle2* __fastcall CopyTriangles(C3DTriangle2* first, C3DTriangle2* last, C3DTriangle2* result, const random_access_iterator_tag& tag, int* dist);   // 0x00A744B0 STLport __copy
struct __false_type {};
C3DTriangle2* __fastcall CopyTrianglesBackward(C3DTriangle2* first, C3DTriangle2* last, C3DTriangle2* result, const random_access_iterator_tag& tag, int* dist);   // 0x00A94A00 STLport __copy_backward
C3DTriangle2* __fastcall UninitializedCopyTriangles(C3DTriangle2* first, C3DTriangle2* last, C3DTriangle2* result, const __false_type& pod);   // 0x00A6E350
void __fastcall FillTriangles(C3DTriangle2* first, C3DTriangle2* last, const C3DTriangle2& x);   // 0x00A945D0 STLport fill
C3DTriangle2* __fastcall UninitializedFillTriangles(C3DTriangle2* first, unsigned long n, const C3DTriangle2& x);   // 0x00A95970 uninitialized_fill_n

template <class T> struct CArray {                 // retail STLport vector layout
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    // 0x00A71550 STLport _M_insert_overflow: grow to old + max(old, fill_len), copy the head, fill, copy the tail
    __declspec(noinline) void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned long fill_len, bool atend)
    {
        unsigned long old_size = size();
        unsigned long len = old_size + (old_size < fill_len ? fill_len : old_size);
        T* new_start = len ? (T*)::operator new(len * sizeof(T)) : 0;
        T* new_finish = new_start;
        for (T* p = _M_start; p != pos; ++p, ++new_finish)
            if (new_finish) *new_finish = *p;
        if (fill_len == 1) {
            if (new_finish) *new_finish = x;
            ++new_finish;
        }
        else
            for (unsigned long i = fill_len; i > 0; --i, ++new_finish)
                if (new_finish) *new_finish = x;
        if (!atend)
            new_finish = UninitializedCopyTriangles(pos, _M_finish, new_finish, __false_type());
        if (_M_start)
            ::operator delete(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
    // 0x00A96F00 STLport _M_fill_insert
    __declspec(noinline) void _M_fill_insert(T* pos, unsigned long n, const T& x)
    {
        if (n != 0) {
            if ((unsigned long)(_M_end_of_storage - _M_finish) >= n) {
                T x_copy = x;
                unsigned long elems_after = _M_finish - pos;
                T* old_finish = _M_finish;
                if (elems_after > n) {
                    UninitializedCopyTriangles(_M_finish - n, _M_finish, _M_finish, __false_type());
                    _M_finish += n;
                    CopyTrianglesBackward(pos, old_finish - n, old_finish, random_access_iterator_tag(), (int*)0);
                    FillTriangles(pos, pos + n, x_copy);
                }
                else {
                    UninitializedFillTriangles(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    UninitializedCopyTriangles(pos, old_finish, _M_finish, __false_type());
                    _M_finish += elems_after;
                    FillTriangles(pos, old_finish, x_copy);
                }
            }
            else
                _M_insert_overflow(pos, x, __false_type(), n, false);
        }
    }
    T* erase(T* first, T* last)
    {
        T* i = CopyTriangles(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _M_finish = i;
        return first;
    }
    __declspec(noinline) void resize(unsigned long n, const T& x)   // 0x00A97A50
    {
        if (n < size())
            erase(_M_start + n, _M_finish);
        else
            _M_fill_insert(_M_finish, n - size(), x);
    }
    void resize(unsigned long n) { resize(n, T()); }
    T* GetData() { return size() ? _M_start : (T*)this; }
};

#pragma auto_inline(off)
template struct CArray<C3DTriangle2>;
#pragma auto_inline(on)

struct CChunkedFileSuperChunk {
    virtual void v0();
    unsigned char _body[0x28];             // CChunkedFileChunk (0x10) + the sub chunk containers (retail 0x2C total)
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};

struct C3DMeshFileTrianglesChunk : CChunkedFileSuperChunk {
    CArray<C3DTriangle2> TriangleList;     // +0x2C
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileTrianglesChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    unsigned long count = in.ReadULong();
    TriangleList.resize(count);
    in.Read(TriangleList.GetData(), TriangleList.size() * sizeof(C3DTriangle2));
    unsigned long used = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - used, loader);
}

// ---- stubs ----
void CDataInputStream::v0() { }
void CDataInputStream::v1() { }
static int gGetPos = 0;
unsigned long CDataInputStream::GetPosition() { ++gGetPos; return m_position; }
static int gOverflowCalls = 0; static long gOverflowSize = 0; static unsigned char gOverflowFill[64];
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    ++gOverflowCalls; gOverflowSize = size;
    memcpy(buffer, gOverflowFill, size);
    m_position += size;
}
C3DTriangle2* __fastcall CopyTriangles(C3DTriangle2* first, C3DTriangle2* last, C3DTriangle2* result, const random_access_iterator_tag&, int*)
{ for (; first != last; ++first, ++result) *result = *first; return result; }
C3DTriangle2* __fastcall CopyTrianglesBackward(C3DTriangle2* first, C3DTriangle2* last, C3DTriangle2* result, const random_access_iterator_tag&, int*)
{ while (last != first) *--result = *--last; return result; }
C3DTriangle2* __fastcall UninitializedCopyTriangles(C3DTriangle2* first, C3DTriangle2* last, C3DTriangle2* result, const __false_type&)
{ for (; first != last; ++first, ++result) *result = *first; return result; }
void __fastcall FillTriangles(C3DTriangle2* first, C3DTriangle2* last, const C3DTriangle2& x) { for (; first != last; ++first) *first = x; }
C3DTriangle2* __fastcall UninitializedFillTriangles(C3DTriangle2* first, unsigned long n, const C3DTriangle2& x) { for (; n; --n, ++first) *first = x; return first; }
void CChunkedFileSuperChunk::v0() { }
static int gBaseCalls = 0; static CDataInputStream* gBaseIn = 0; static unsigned long gBaseSize = 0; static CChunkedFileLoader* gBaseLoader = 0;
void CChunkedFileSuperChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{ ++gBaseCalls; gBaseIn = &in; gBaseSize = chunkSize; gBaseLoader = &loader; }

static void Reset() { gGetPos = 0; gOverflowCalls = 0; gOverflowSize = 0; gBaseCalls = 0; gBaseIn = 0; gBaseSize = 0; gBaseLoader = 0; }
static void SetStream(CDataInputStream& s, unsigned long pos, const unsigned char* data, long remaining)
{ s.m_position = pos; s.m_08 = 0; s.m_pData = data; s.m_10 = 0; s.m_remaining = remaining; }

int main()
{
    CChunkedFileLoader* loader = (CChunkedFileLoader*)0x1234;
    unsigned char buf[64];
    // 1: fast path for both reads, growing an empty list (resize -> _M_fill_insert -> _M_insert_overflow)
    {
        unsigned long cnt = 2; memcpy(buf, &cnt, 4);
        unsigned short tri[6] = { 1, 2, 3, 4, 5, 6 }; memcpy(buf + 4, tri, 12);
        CDataInputStream s; SetStream(s, 100, buf, 40);
        C3DMeshFileTrianglesChunk c; c.TriangleList._M_start = c.TriangleList._M_finish = c.TriangleList._M_end_of_storage = 0;
        Reset();
        c.ReadFromFile(s, 200, *loader);
        CHECK(c.TriangleList.size() == 2);
        CHECK(memcmp(c.TriangleList._M_start, tri, 12) == 0);
        CHECK(s.m_position == 116 && s.m_remaining == 24 && s.m_pData == buf + 16);
        CHECK(gOverflowCalls == 0 && gGetPos == 2);
        CHECK(gBaseCalls == 1 && gBaseIn == &s && gBaseSize == 200 - 16 && gBaseLoader == loader);
    }
    // 2: count via the overflow reader, triangles via the overflow reader too; list already has room (fill path)
    {
        unsigned long cnt = 1; memcpy(gOverflowFill, &cnt, 4);
        CDataInputStream s; SetStream(s, 0, buf, 2);
        C3DMeshFileTrianglesChunk c;
        C3DTriangle2 store[4]; c.TriangleList._M_start = store; c.TriangleList._M_finish = store; c.TriangleList._M_end_of_storage = store + 4;
        Reset();
        c.ReadFromFile(s, 50, *loader);
        CHECK(c.TriangleList.size() == 1 && c.TriangleList._M_start == store);
        CHECK(gOverflowCalls == 2 && gOverflowSize == 6);
        CHECK(s.m_position == 10 && s.m_remaining == 2);
        CHECK(gBaseCalls == 1 && gBaseSize == 50 - 10);
    }
    // 3: count 0 shrinks a non-empty list (erase path); the zero-size Read returns early (GetData gives the vector itself)
    {
        unsigned long cnt = 0; memcpy(buf, &cnt, 4);
        CDataInputStream s; SetStream(s, 8, buf, 40);
        C3DMeshFileTrianglesChunk c;
        C3DTriangle2 store[4]; c.TriangleList._M_start = store; c.TriangleList._M_finish = store + 3; c.TriangleList._M_end_of_storage = store + 4;
        Reset();
        c.ReadFromFile(s, 30, *loader);
        CHECK(c.TriangleList.size() == 0);
        CHECK(c.TriangleList.GetData() == (C3DTriangle2*)&c.TriangleList);
        CHECK(s.m_position == 12 && s.m_remaining == 36 && gOverflowCalls == 0);
        CHECK(gBaseCalls == 1 && gBaseSize == 30 - 4);
    }
    // 4: the triangle read would pass 0x7fffffff, so it is skipped
    {
        unsigned long cnt = 1; memcpy(buf, &cnt, 4);
        unsigned long start = 0x7fffffffUL - 6;
        CDataInputStream s; SetStream(s, start, buf, 40);
        C3DMeshFileTrianglesChunk c;
        C3DTriangle2 store[2]; c.TriangleList._M_start = store; c.TriangleList._M_finish = store; c.TriangleList._M_end_of_storage = store + 2;
        Reset();
        c.ReadFromFile(s, 20, *loader);
        CHECK(c.TriangleList.size() == 1);
        CHECK(s.m_position == start + 4 && s.m_remaining == 36 && gOverflowCalls == 0);
        CHECK(gBaseCalls == 1 && gBaseSize == 20 - 4);
    }
    // 5: the count read itself would pass 0x7fffffff (count keeps the stream's stale value): nothing is read
    {
        CDataInputStream s; SetStream(s, 0x7ffffffeUL, buf, 40);
        C3DMeshFileTrianglesChunk c;
        C3DTriangle2 store[1]; c.TriangleList._M_start = store; c.TriangleList._M_finish = store; c.TriangleList._M_end_of_storage = store + 1;
        Reset();
        unsigned long before = s.m_position;
        (void)before;
        CHECK(!CDataInputStream::IsValidPosition((__int64)(s.m_position + 4)));
        CHECK(CDataInputStream::IsValidPosition(0) && !CDataInputStream::IsValidPosition(-1));
    }
    if (fails == 0) printf("00abd280_TEST PASS\n");
    return fails;
}