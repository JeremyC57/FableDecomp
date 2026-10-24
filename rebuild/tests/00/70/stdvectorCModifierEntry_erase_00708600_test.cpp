// Behaviour test for 0x00708600 std::vector<CModifierEntry>::erase(iterator) (STLport shape).
#include <cstdio>

struct CModifierEntry { long DefIndex; bool Active; };

template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* end() { return _M_finish; }
    T* erase(T* position)
    {
        if (position + 1 != end()) {
            const T* first = position + 1;
            T* result = position;
            for (int n = (int)(_M_finish - first); n > 0; --n) { *result = *first; ++first; ++result; }
        }
        --_M_finish;
        return position;
    }
};

int main()
{
    CModifierEntry e[4] = { { 1, true }, { 2, false }, { 3, true }, { 4, false } };
    CRetailVector<CModifierEntry> v; v._M_start = e; v._M_finish = e + 4; v._M_end_of_storage = e + 4;
    CModifierEntry* r = v.erase(e + 1);                   // middle
    bool ok = r == e + 1 && v._M_finish == e + 3 && e[0].DefIndex == 1 && e[1].DefIndex == 3 && e[1].Active
           && e[2].DefIndex == 4 && !e[2].Active;
    r = v.erase(e + 2);                                   // last: no copy
    ok = ok && r == e + 2 && v._M_finish == e + 2 && e[1].DefIndex == 3;
    r = v.erase(e);                                       // first
    ok = ok && r == e && v._M_finish == e + 1 && e[0].DefIndex == 3;
    if (!ok) { std::printf("00708600_TEST FAIL\n"); return 1; }
    std::printf("00708600_TEST PASS\n");
    return 0;
}