// std::for_each(std::list<CAnimSet>::iterator first, std::list<CAnimSet>::iterator last, CMultiplyAnimSetAlphaBy f) -- scales the alpha of every anim set by a factor, clamped to [0, 1], keeping the previous alpha
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x007100A0, 106 bytes, `ret 0Ch`. Caller: CAnimateThing::SetMainAnim 0x0070D716.
// Constants: 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// SGI/STLport `for_each` over a VC6-era Dinkumware list (node {next, prev, value}); the iterator is a one-pointer class with a user copy ctor, so `__fastcall` passes it on the stack and only the hidden functor pointer is in ecx.
// Shape: `alpha * factor` loads the factor first (`fld [f] ; fmul [alpha]`); the clamp is `if (<0) =0; else if (>1) =1;`; the
// inline SetAlpha reads the old alpha as a dword before the fstp of the new one.
template <class T> struct SListNode {             // VC6-era Dinkumware list node
    SListNode* next;                              // +0x00
    SListNode* prev;                              // +0x04
    T value;                                      // +0x08
};
template <class T> class CListIterator {          // list<T>::iterator: one node pointer with a user copy ctor
public:
    SListNode<T>* m_node;
    CListIterator(SListNode<T>* node) : m_node(node) {}
    CListIterator(const CListIterator& o) : m_node(o.m_node) {}
    T& operator*() const { return m_node->value; }
    CListIterator& operator++() { m_node = m_node->next; return *this; }
    bool operator!=(const CListIterator& o) const { return m_node != o.m_node; }
};
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CAnimSet {
public:
    unsigned char _pad[0x2c];
    float m_alpha;                                // +0x2C
    float m_previousAlpha;                        // +0x30
    void SetAlpha(float alpha)
    {
        m_previousAlpha = m_alpha;
        m_alpha = alpha;
    }
};
class CMultiplyAnimSetAlphaBy {
public:
    float m_factor;                               // +0x00
    CMultiplyAnimSetAlphaBy(float factor) : m_factor(factor) {}
    void operator()(CAnimSet& set) const
    {
        float alpha = set.m_alpha * m_factor;
        if (alpha < 0.0f)
            alpha = 0.0f;
        else if (alpha > 1.0f)
            alpha = 1.0f;
        set.SetAlpha(alpha);
    }
};
template CMultiplyAnimSetAlphaBy __fastcall std::for_each(CListIterator<CAnimSet>, CListIterator<CAnimSet>, CMultiplyAnimSetAlphaBy);