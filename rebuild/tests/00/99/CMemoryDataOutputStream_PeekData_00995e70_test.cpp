// Test for 0x00995E70 CMemoryDataOutputStream::PeekData: first byte of the written data, or the array object while empty.
#include <cstdio>
template <class T> class CArray {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const T& front() const { return *_M_start; }
    const T* GetAsCArray() const { return size() > 0 ? &front() : GetZeroSizeArray(); }
private:
    const T* GetZeroSizeArray() const { return reinterpret_cast<const T*>(this); }
};
class CMemoryDataOutputStream {
public:
    CArray<char> Data;
    unsigned long CurrentPosition;
    const void* PeekData() const { return Data.GetAsCArray(); }
};
int main()
{
    char buf[8] = { 'a', 'b', 'c' };
    CMemoryDataOutputStream s;
    s.Data._M_start = s.Data._M_finish = s.Data._M_end_of_storage = 0;
    if (s.PeekData() != (const void*)&s.Data) { std::printf("00995e70_TEST FAIL\n"); return 1; }
    s.Data._M_start = buf; s.Data._M_finish = buf; s.Data._M_end_of_storage = buf + 8;
    if (s.PeekData() != (const void*)&s.Data) { std::printf("00995e70_TEST FAIL\n"); return 1; }
    s.Data._M_finish = buf + 3;
    if (s.PeekData() != (const void*)buf || *(const char*)s.PeekData() != 'a') { std::printf("00995e70_TEST FAIL\n"); return 1; }
    std::printf("00995e70_TEST PASS\n");
    return 0;
}