// Behaviour test for std::vector<CMatrix3x4>::reserve(size_type n) @ 0x00AAFD90
#include <cstdio>
#include <cstdlib>
#include <new>

class CMatrix3x4 { public: float m_row[3][4]; };

struct CMatrixVector {
    CMatrix3x4* _M_start;
    CMatrix3x4* _M_finish;
    CMatrix3x4* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    static CMatrix3x4* allocate(unsigned int n) { return n != 0 ? (CMatrix3x4*)std::malloc(n * sizeof(CMatrix3x4)) : 0; }
    CMatrix3x4* allocate_and_copy(unsigned int n, const CMatrix3x4* first, const CMatrix3x4* last)
    {
        CMatrix3x4* result = allocate(n);
        CMatrix3x4* cur = result;
        for ( ; first != last; ++first, ++cur) new (cur) CMatrix3x4(*first);
        return result;
    }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old_size = size();
            CMatrix3x4* tmp;
            if (_M_start) {
                tmp = allocate_and_copy(n, _M_start, _M_finish);
                std::free(_M_start);
            }
            else
                tmp = allocate(n);
            _M_start = tmp;
            _M_finish = tmp + old_size;
            _M_end_of_storage = tmp + n;
        }
    }
};

int main()
{
    CMatrixVector v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0;
    v.reserve(2);
    int ok = v.capacity() == 2 && v.size() == 0 && v._M_start != 0;
    v._M_start[0].m_row[1][2] = 5.0f;
    v._M_start[1].m_row[2][3] = 6.0f;
    v._M_finish = v._M_start + 2;
    v.reserve(1);                                   // no-op
    if (v.capacity() != 2) ok = 0;
    v.reserve(10);
    if (v.capacity() != 10 || v.size() != 2) ok = 0;
    if (v._M_start[0].m_row[1][2] != 5.0f || v._M_start[1].m_row[2][3] != 6.0f) ok = 0;
    std::free(v._M_start);
    if (!ok) { std::printf("00aafd90_TEST FAIL\n"); return 1; }
    std::printf("00aafd90_TEST PASS\n");
    return 0;
}