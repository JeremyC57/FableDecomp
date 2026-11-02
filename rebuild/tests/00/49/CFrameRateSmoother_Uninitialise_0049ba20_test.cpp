// Behaviour test for 0x0049ba20 CFrameRateSmoother::Uninitialise(): a valid smoother empties its Times array (finish = start,
// storage kept) and the base Uninitialise clears Valid; an invalid one leaves Times alone.
#include <cstdio>
#include <cstring>

class CInitBaseClass {
public:
    CInitBaseClass() : Valid(false) { }
    bool IsValid() const { return Valid; }
    void Uninitialise() { if (Valid) Valid = false; }
    bool Valid;
};
struct FloatArray {                                         // STLport vector<float>: start / finish / end_of_storage
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    void clear()                                            // erase(begin(), end()): __copy_trivial(finish, finish, start)
    {
        float* first = _M_start;
        float* last = _M_finish;
        float* i = (last == _M_finish) ? first
                 : (float*)((char*)std::memmove(first, last, (char*)_M_finish - (char*)last) + ((char*)_M_finish - (char*)last));
        _M_finish = i;
    }
};
class CFrameRateSmoother : public CInitBaseClass {
public:
    void Uninitialise()
    {
        if (IsValid())
            Times.clear();
        CInitBaseClass::Uninitialise();
    }
    FloatArray Times;
};

int main()
{
    float storage[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    CFrameRateSmoother s;
    s.Times._M_start = storage; s.Times._M_finish = storage + 5; s.Times._M_end_of_storage = storage + 8;
    s.Valid = true;
    s.Uninitialise();
    if (s.Valid || s.Times._M_finish != storage || s.Times._M_start != storage || s.Times._M_end_of_storage != storage + 8) {
        std::printf("0049ba20_TEST FAIL (valid)\n");
        return 1;
    }
    s.Times._M_finish = storage + 3;
    s.Uninitialise();                                        // not valid: Times kept
    if (s.Valid || s.Times._M_finish != storage + 3) {
        std::printf("0049ba20_TEST FAIL (invalid)\n");
        return 1;
    }
    std::printf("0049ba20_TEST PASS\n");
    return 0;
}