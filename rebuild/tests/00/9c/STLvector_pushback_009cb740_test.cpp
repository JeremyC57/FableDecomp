#include <cstdio>
#include <cstdlib>
#include <new>

// Host model of the text bank's entry vectors: an intrusively counted pointee (count at +4 in retail), the
// CIVCountedPointer copy add-refs, the destructor releases; STLport vector push_back / _M_insert_overflow.
struct CPointee { int RefCount; };
template <class T> struct CIVCountedPointer {
    T* P;
    explicit CIVCountedPointer(T* p) : P(p) { if (P) ++P->RefCount; }
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
    ~CIVCountedPointer() { if (P) --P->RefCount; }
private:
    CIVCountedPointer& operator=(const CIVCountedPointer&);
};
struct CDataBankTextEntry { CIVCountedPointer<CPointee> PText; long NarratorIndex;
    CDataBankTextEntry(CPointee* p, long n) : PText(p), NarratorIndex(n) { } };
struct CDataBankGroupEntry { CIVCountedPointer<CPointee> PGroup;
    explicit CDataBankGroupEntry(CPointee* p) : PGroup(p) { } };

template <class T> struct vector {
    T* Start; T* Finish; T* End;
    vector() : Start(0), Finish(0), End(0) { }
    ~vector() { for (T* p = Start; p != Finish; ++p) p->~T(); if (Start) std::free(Start); }
    static T* copy_(T* first, T* last, T* out) { for (; first != last; ++first, ++out) new (out) T(*first); return out; }
    void insert_overflow(T* pos, const T& x, unsigned int fill, bool atend)
    {
        unsigned int old = (unsigned int)(Finish - Start);
        unsigned int len = old + (old < fill ? fill : old);
        T* ns = len ? (T*)std::malloc(len * sizeof(T)) : 0;
        T* nf = copy_(Start, pos, ns);
        if (fill == 1) { new (nf) T(x); ++nf; }
        else for (unsigned int i = 0; i < fill; ++i, ++nf) new (nf) T(x);
        if (!atend) nf = copy_(pos, Finish, nf);
        for (T* p = Start; p != Finish; ++p) p->~T();
        if (Start) std::free(Start);
        Start = ns; Finish = nf; End = ns + len;
    }
    void push_back(const T& x)
    {
        if (Finish != End) {
            if (Finish) new (Finish) T(x);
            ++Finish;
        }
        else
            insert_overflow(Finish, x, 1, true);
    }
};

int main()
{
    CPointee text = { 0 };
    {
        vector<CDataBankTextEntry> v;
        for (long i = 0; i < 9; ++i) { CDataBankTextEntry e(&text, i); v.push_back(e); }
        if (v.Finish - v.Start != 9) { std::printf("009cb740_TEST FAIL\n"); return 1; }
        for (long i = 0; i < 9; ++i) if (v.Start[i].NarratorIndex != i || v.Start[i].PText.P != &text) { std::printf("009cb740_TEST FAIL\n"); return 1; }
        if (text.RefCount != 9) { std::printf("009cb740_TEST FAIL\n"); return 1; }                              // one reference per stored entry
    }
    if (text.RefCount != 0) { std::printf("009cb740_TEST FAIL\n"); return 1; }
    std::printf("009cb740_TEST PASS\n");
    return 0;
}