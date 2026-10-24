// Behaviour test for 0x0086EE50: resize(n) forwards n and an empty inner vector to resize(n, value), then destroys the temporary.
#include <cstdio>
static unsigned int g_n = 0; static int g_emptyValue = 0; static int g_dtors = 0;
struct CRecordVector { void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CRecordVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { } ~CRecordVector() { ++g_dtors; } };
struct CRecordVectorVector {
    unsigned int size;
    void resize(unsigned int newSize, const CRecordVector& value) { g_n = newSize; g_emptyValue = (!value._M_start && !value._M_finish && !value._M_end_of_storage); size = newSize; }
    void resize(unsigned int newSize) { resize(newSize, CRecordVector()); }
};
int main()
{
    CRecordVectorVector v; v.size = 0;
    v.resize(7);
    if (g_n != 7 || !g_emptyValue || g_dtors != 1 || v.size != 7) { std::printf("0086ee50_TEST FAIL\n"); return 1; }
    std::printf("0086ee50_TEST PASS\n");
    return 0;
}