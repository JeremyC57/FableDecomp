// Behaviour test for 0x0075EE70: vector<CTCCameraPointScriptedSpline::CKeyCamera>::size() is (finish - start) in 56-byte elements
#include <cstdio>

class CKeyCamera {                        // test stand-in with the retail size (56 bytes)
public:
    unsigned char Bytes[56];
};

class TestVector {
public:
    CKeyCamera* _M_start;
    CKeyCamera* _M_finish;
    CKeyCamera* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }   // the reconstructed body
};

int main()
{
    static CKeyCamera defs[12];
    TestVector v;
    v._M_start = defs; v._M_finish = defs; v._M_end_of_storage = defs + 12;
    if (v.size() != 0) { std::printf("0075ee70_TEST FAIL\n"); return 1; }
    v._M_finish = defs + 7;
    if (v.size() != 7) { std::printf("0075ee70_TEST FAIL\n"); return 1; }
    v._M_finish = defs + 12;
    if (v.size() != 12 || sizeof(CKeyCamera) != 56) { std::printf("0075ee70_TEST FAIL\n"); return 1; }
    std::printf("0075ee70_TEST PASS\n");
    return 0;
}