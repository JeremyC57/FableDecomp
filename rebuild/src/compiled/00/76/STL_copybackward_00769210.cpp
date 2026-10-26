// std::__copy_backward(CDefPointer<const CThingBaseDef>* first, CDefPointer<const CThingBaseDef>* last, CDefPointer<const CThingBaseDef>* result, const random_access_iterator_tag&, int*) -- copy [first, last) backwards onto the range ending at result, counted-def assignment inlined
// Leaf: ??$__copy_backward@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769210, 88 bytes (+int3 pad to 96), `ret 0Ch`.
// Callers: 0x0076986C and 0x0076B9C9 (vector<CDefPointer<const CThingBaseDef> > insert/erase paths).
// Shape: STLport `__copy_backward(first, last, result, random_access_iterator_tag, _Distance*)` from an /Gr
//   algorithm TU (__fastcall: first in ecx, last in edx; result, the tag reference and the null distance
//   pointer on the stack). The count n = last - first is kept in ebx and counted down; both pointers step
//   back one element before the assignment. The element assignment is the inlined CDefPointer<T>::operator=
//   (old def released through its slot-1 virtual at count zero and nulled inside the non-null branch, new
//   def adopted and addref'd); the pointee is read into a local, as the landed 0x007691A0 fill. An empty
//   range returns the untouched result (`mov eax,[esp+8]`).
// Layout: def pointee = vtable +0, reference count +4 (CDefBase's counted base, as the landed 0x0083BD10).
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy() const;                 // slot 1: destroy at count zero
    mutable unsigned long m_count;                // +0x04 (mutable: CDefPointer<const T> still counts)
};

class CThingBaseDef : public CDefPointeeBase {
};

template <class T> class CDefPointer {
public:
    T* m_p;                                       // +0x00
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

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template CDefPointer<const CThingBaseDef>* __fastcall __copy_backward<CDefPointer<const CThingBaseDef>*, CDefPointer<const CThingBaseDef>*, int>(
    CDefPointer<const CThingBaseDef>*, CDefPointer<const CThingBaseDef>*, CDefPointer<const CThingBaseDef>*,
    const random_access_iterator_tag&, int*);

}