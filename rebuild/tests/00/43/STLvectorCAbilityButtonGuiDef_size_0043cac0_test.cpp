// Behaviour test for 0x0043CAC0: vector<CAbilityButtonGuiDef>::size() is (finish - start) in 92-byte elements
#include <cstdio>

class CAbilityButtonGuiDef {                        // test stand-in with the retail size (92 bytes)
public:
    unsigned char Bytes[92];
};

class TestVector {
public:
    CAbilityButtonGuiDef* _M_start;
    CAbilityButtonGuiDef* _M_finish;
    CAbilityButtonGuiDef* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }   // the reconstructed body
};

int main()
{
    static CAbilityButtonGuiDef defs[12];
    TestVector v;
    v._M_start = defs; v._M_finish = defs; v._M_end_of_storage = defs + 12;
    if (v.size() != 0) { std::printf("0043cac0_TEST FAIL\n"); return 1; }
    v._M_finish = defs + 7;
    if (v.size() != 7) { std::printf("0043cac0_TEST FAIL\n"); return 1; }
    v._M_finish = defs + 12;
    if (v.size() != 12 || sizeof(CAbilityButtonGuiDef) != 92) { std::printf("0043cac0_TEST FAIL\n"); return 1; }
    std::printf("0043cac0_TEST PASS\n");
    return 0;
}