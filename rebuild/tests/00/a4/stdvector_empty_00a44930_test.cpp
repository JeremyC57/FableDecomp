// Behaviour test for 0x00A44930 std::vector<CASuspendableProcess*>::empty() const
#include <cstdio>
class CASuspendableProcess;
template <class T> class TestVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
};
int main()
{
    CASuspendableProcess* storage[4] = { 0, 0, 0, 0 };
    TestVector<CASuspendableProcess*> v;
    v._M_start = storage; v._M_finish = storage; v._M_end_of_storage = storage + 4;
    if (!v.empty()) { std::printf("00a44930_TEST FAIL\n"); return 1; }
    v._M_finish = storage + 2;
    if (v.empty()) { std::printf("00a44930_TEST FAIL\n"); return 1; }
    std::printf("00a44930_TEST PASS\n");
    return 0;
}