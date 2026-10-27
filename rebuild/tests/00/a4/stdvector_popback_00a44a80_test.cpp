// Behaviour test for 0x00A44A80 std::vector<CASuspendableProcess*>::pop_back()
#include <cstdio>
class CASuspendableProcess;
template <class T> inline void TestDestroy(T* p) { (void)p; }
template <class T> class TestVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void pop_back() { --_M_finish; TestDestroy(_M_finish); }
};
int main()
{
    CASuspendableProcess* storage[4] = { 0, 0, 0, 0 };
    TestVector<CASuspendableProcess*> v;
    v._M_start = storage; v._M_finish = storage + 3; v._M_end_of_storage = storage + 4;
    v.pop_back();
    if (v._M_finish != storage + 2) { std::printf("00a44a80_TEST FAIL\n"); return 1; }
    v.pop_back(); v.pop_back();
    if (v._M_finish != v._M_start) { std::printf("00a44a80_TEST FAIL\n"); return 1; }
    std::printf("00a44a80_TEST PASS\n");
    return 0;
}