// Behaviour test for 00aa5bd0 (CVectorPairOwner_00aa5bd0::ClearBoth): both vectors end up empty (finish = start) with their storage kept.
#include <cstdio>
#include <cstring>

struct SKeyFrame {
    long m_time;
    long m_value;
    SKeyFrame& operator=(const SKeyFrame& other) { m_time = other.m_time; m_value = other.m_value; return *this; }
};

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)std::memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

template <class T> inline T* CopyAssign(T* first, T* last, T* result)
{
    for (long n = (long)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

struct CPointerVector {
    void** _M_start;
    void** _M_finish;
    void** _M_end_of_storage;
    void clear() { _M_finish = (void**)CopyTrivial(_M_finish, _M_finish, _M_start); }
};

struct CKeyFrameVector {
    SKeyFrame* _M_start;
    SKeyFrame* _M_finish;
    SKeyFrame* _M_end_of_storage;
    void clear() { _M_finish = CopyAssign(_M_finish, _M_finish, _M_start); }
};

struct CVectorPairOwner {
    CPointerVector  m_pointers;
    CKeyFrameVector m_records;
    void ClearBoth()
    {
        m_pointers.clear();
        m_records.clear();
    }
};

int main()
{
    static void* ptrs[4];
    static SKeyFrame frames[4];
    CVectorPairOwner o;
    o.m_pointers._M_start = ptrs; o.m_pointers._M_finish = ptrs + 3; o.m_pointers._M_end_of_storage = ptrs + 4;
    o.m_records._M_start = frames; o.m_records._M_finish = frames + 2; o.m_records._M_end_of_storage = frames + 4;
    o.ClearBoth();
    if (o.m_pointers._M_finish != ptrs || o.m_records._M_finish != frames || o.m_pointers._M_end_of_storage != ptrs + 4
        || o.m_records._M_end_of_storage != frames + 4) { std::printf("00aa5bd0_TEST FAIL\n"); return 1; }
    std::printf("00aa5bd0_TEST PASS\n");
    return 0;
}