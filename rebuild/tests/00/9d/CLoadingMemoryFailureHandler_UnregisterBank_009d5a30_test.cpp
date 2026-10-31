// Behaviour test for CLoadingMemoryFailureHandler::UnregisterBank (0x009D5A30): lock, find the bank, erase it (tail moved down).
#include <cstdio>
#include <cstring>
static int g_locks = 0, g_unlocks = 0;
struct CCriticalSectionLock {
    CCriticalSectionLock(int&, bool) { ++g_locks; }
    ~CCriticalSectionLock() { ++g_unlocks; }
};
class CBankFileAsync { public: int id; };
template <class T> T* Find4(T* first, T* last, const T& val)      // STLport __find semantics
{
    long trip = (last - first) >> 2;
    for (; trip > 0; --trip) {
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
    }
    switch (last - first) {
    case 3: if (*first == val) return first; ++first;
    case 2: if (*first == val) return first; ++first;
    case 1: if (*first == val) return first; ++first;
    default: return last;
    }
}
struct CArrayBanks {
    CBankFileAsync** _M_start; CBankFileAsync** _M_finish; CBankFileAsync** _M_end;
    CBankFileAsync** begin() { return _M_start; }
    CBankFileAsync** end() { return _M_finish; }
    CBankFileAsync** Find(CBankFileAsync* const& b) { return Find4(begin(), end(), b); }
    CBankFileAsync** erase(CBankFileAsync** pos)
    {
        if (pos + 1 != end())
            std::memmove(pos, pos + 1, (char*)_M_finish - (char*)(pos + 1));
        --_M_finish;
        return pos;
    }
};
class CLoadingMemoryFailureHandler { public:
    CArrayBanks Banks;
    int CriticalSection;
    void UnregisterBank(CBankFileAsync* bank)
    {
        CCriticalSectionLock lock(CriticalSection, false);
        CBankFileAsync** it = Banks.Find(bank);
        if (it != Banks.end())
            Banks.erase(it);
    }
};
int main()
{
    CBankFileAsync b[7];
    CBankFileAsync* store[8];
    for (int i = 0; i < 7; ++i) { b[i].id = i; store[i] = &b[i]; }
    CLoadingMemoryFailureHandler h;
    h.Banks._M_start = store; h.Banks._M_finish = store + 7; h.Banks._M_end = store + 8;
    h.UnregisterBank(&b[5]);                         // middle-tail element (remainder switch path)
    h.UnregisterBank(&b[0]);                         // first element (unrolled loop path)
    h.UnregisterBank(&b[6]);                         // last element: no memmove
    CBankFileAsync other; h.UnregisterBank(&other);  // absent: unchanged
    int expect[4] = { 1, 2, 3, 4 };
    if (h.Banks._M_finish - h.Banks._M_start != 4 || g_locks != 4 || g_unlocks != 4) { std::printf("009d5a30_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (store[i]->id != expect[i]) { std::printf("009d5a30_TEST FAIL\n"); return 1; }
    std::printf("009d5a30_TEST PASS\n");
    return 0;
}