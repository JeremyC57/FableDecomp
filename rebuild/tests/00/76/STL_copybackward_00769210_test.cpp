// Behaviour test for 0x00769210 STLport __copy_backward over CDefPointer<const T>: overlapping
// right shift by one keeps every def's count balanced and returns the new start.
#include <cstdio>

static int gDestroyed = 0;

class CDefPointeeBase { public:
    virtual void Destroy() const { ++gDestroyed; }
    mutable unsigned int m_count;
    CDefPointeeBase() : m_count(0) { }
    virtual ~CDefPointeeBase() { }
};

template <class T> class CDefPointer {
public:
    T* m_p;
    void Release()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_count == 0)
                p->Destroy();
            m_p = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            Release();
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};

template <class _RandomAccessIter, class _BidirectionalIter>
_BidirectionalIter copy_backward_model(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result)
{
    for (int __n = (int)(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

int main()
{
    const CDefPointeeBase a, b, c, d;
    CDefPointer<const CDefPointeeBase> v[4];
    v[0].m_p = &a; a.m_count = 1;
    v[1].m_p = &b; b.m_count = 1;
    v[2].m_p = &c; c.m_count = 1;
    v[3].m_p = &d; d.m_count = 1;
    CDefPointer<const CDefPointeeBase>* r = copy_backward_model(v, v + 3, v + 4);   // shift right by one
    int ok = (r == v + 1);
    if (v[0].m_p != &a || v[1].m_p != &a || v[2].m_p != &b || v[3].m_p != &c) ok = 0;
    if (a.m_count != 2 || b.m_count != 1 || c.m_count != 1 || d.m_count != 0) ok = 0;
    if (gDestroyed != 1) ok = 0;                 // d lost its only reference
    if (copy_backward_model(v, v, v + 2) != v + 2) ok = 0;   // empty range returns result
    if (!ok) { std::printf("00769210_TEST FAIL\n"); return 1; }
    std::printf("00769210_TEST PASS\n");
    return 0;
}