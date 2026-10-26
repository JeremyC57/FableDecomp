#include <cstdio>
#include <cstdlib>
#include <cstring>

// Host model of CLoadingMemoryFailureHandler::RegisterBank: take the handler's lock, push the bank pointer onto the
// Banks vector (grow through the POD insert-overflow when full), release the lock on the way out.
class CBankFileAsync { public: int Id; };
struct SCriticalSection { int Depth; int Entered; };
struct CCriticalSectionLock {
    SCriticalSection* m_pSection;
    CCriticalSectionLock(SCriticalSection* s, bool) : m_pSection(s) { ++s->Depth; ++s->Entered; }
    ~CCriticalSectionLock() { --m_pSection->Depth; }
};
struct CBankVector {
    CBankFileAsync** Start; CBankFileAsync** Finish; CBankFileAsync** End;
    void insert_overflow(CBankFileAsync** pos, CBankFileAsync* const& x, unsigned int fill, bool atend)
    {
        unsigned int old = (unsigned int)(Finish - Start);
        unsigned int len = old + (old < fill ? fill : old);
        CBankFileAsync** ns = len ? (CBankFileAsync**)std::malloc(len * sizeof(CBankFileAsync*)) : 0;
        CBankFileAsync** nf = ns;
        if (pos != Start) { std::memmove(nf, Start, (pos - Start) * sizeof(CBankFileAsync*)); nf += pos - Start; }
        for (unsigned int i = 0; i < fill; ++i) *nf++ = x;
        if (!atend && Finish != pos) { std::memmove(nf, pos, (Finish - pos) * sizeof(CBankFileAsync*)); nf += Finish - pos; }
        if (Start) std::free(Start);
        Start = ns; Finish = nf; End = ns + len;
    }
    void push_back(CBankFileAsync* const& x)
    {
        if (Finish != End) { if (Finish) *Finish = x; ++Finish; }
        else insert_overflow(Finish, x, 1, true);
    }
};
class CLoadingMemoryFailureHandler { public:
    CBankVector Banks;
    SCriticalSection CriticalSection;
    void RegisterBank(CBankFileAsync* bank)
    {
        CCriticalSectionLock lock(&CriticalSection, false);
        if (CriticalSection.Depth != 1) bank = 0;                         // lock held while pushing
        Banks.push_back(bank);
    }
};

int main()
{
    CBankFileAsync banks[10];
    CLoadingMemoryFailureHandler h;
    h.Banks.Start = h.Banks.Finish = h.Banks.End = 0;
    h.CriticalSection.Depth = 0; h.CriticalSection.Entered = 0;
    for (int i = 0; i < 10; ++i) { banks[i].Id = i; h.RegisterBank(&banks[i]); }
    bool ok = h.Banks.Finish - h.Banks.Start == 10 && h.CriticalSection.Depth == 0 && h.CriticalSection.Entered == 10;
    for (int i = 0; ok && i < 10; ++i) if (h.Banks.Start[i] != &banks[i]) ok = false;
    if (h.Banks.End - h.Banks.Start != 16) ok = false;                     // 1, 2, 4, 8, 16: doubling growth
    std::free(h.Banks.Start);
    if (!ok) { std::printf("009d6420_TEST FAIL\n"); return 1; }
    std::printf("009d6420_TEST PASS\n");
    return 0;
}