// Behaviour test for 00686090 CPointerVector::ClearElements (null every element, keep the size)
#include <cstdio>
struct CPointerVector {
    void** _M_start; void** _M_finish; void** _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void*& operator[](unsigned int n) { return *(_M_start + n); }
    void ClearElements();
};
void CPointerVector::ClearElements()
{
    for (unsigned int i = 0; i < size(); ++i)
        (*this)[i] = 0;
}
int main()
{
    int x;
    void* items[4] = { &x, &x, &x, &x };
    CPointerVector v; v._M_start = items; v._M_finish = items + 3; v._M_end_of_storage = items + 4;
    v.ClearElements();
    if (items[0] || items[1] || items[2] || items[3] != &x || v.size() != 3) { std::printf("00686090_TEST FAIL\n"); return 1; }
    v._M_finish = v._M_start; v.ClearElements();
    std::printf("00686090_TEST PASS\n");
    return 0;
}
