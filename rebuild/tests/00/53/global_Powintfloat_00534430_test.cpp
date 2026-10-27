// Behaviour test for 00534430 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
template<class _Ty> _Ty _Pow_int(_Ty _X, int _Y) {
    unsigned int _N; if (_Y >= 0) _N = (unsigned int)_Y; else _N = (unsigned int)(-_Y);
    for (_Ty _Z = _Ty(1); ; _X *= _X) { if ((_N & 1) != 0) _Z *= _X; if ((_N >>= 1) == 0) return (_Y < 0 ? _Ty(1) / _Z : _Z); } }
int main() {
    if (_Pow_int(2.0f, 10) != 1024.0f) { std::printf("00534430_TEST FAIL\n"); return 1; }
    if (_Pow_int(3.0f, 0) != 1.0f) { std::printf("00534430_TEST FAIL\n"); return 1; }
    if (_Pow_int(2.0f, -2) != 0.25f) { std::printf("00534430_TEST FAIL\n"); return 1; }
    if (_Pow_int(-1.5f, 3) != -3.375f) { std::printf("00534430_TEST FAIL\n"); return 1; }
    { std::printf("00534430_TEST PASS\n"); return 0; }
}