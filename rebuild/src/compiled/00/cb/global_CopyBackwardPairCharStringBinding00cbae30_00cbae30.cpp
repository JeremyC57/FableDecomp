// _global::CopyBackward_PairCharStringBinding_00cbae30(pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >*, pair<..>*, pair<..>*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over 12-byte (name, counted script binding) pairs
// Leaf: CopyBackward_PairCharStringBinding_00cbae30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBAE30, length 143 bytes.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); count = (last - first)
// / 12, held in the dead argument slot; both pointers step back one element; the moved-back result is returned.
// Element: std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > (the manifest label; 12 bytes = the
// one-word CCharString + the 8-byte {object, count block} counted pointer). The pair's implicit operator= calls the
// out-of-line CCharString::operator= @ 0x0099EFB0, then the inlined CCountedPointer::operator= (the source's fields
// read first; when the count blocks differ release the old block -- destroy function at +4 with the object, then
// operator delete @ 0x00BFE9BC -- adopt both words and add a reference), modelled as the landed 0x0084F130.
struct CCharString {
    void* m_pData;                           // +0x00
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct SCountBlock {
    long m_references;                       // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4
    void* m_pObject;                         // +8
};

struct CCountedPointer {                     // CCountedPointer<CEntityScriptBindingBase>, 8 bytes
    void*        m_pObject;                  // +0
    SCountBlock* m_pCount;                   // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;
        void* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

struct PairCharStringBinding {               // std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >
    CCharString     first;                   // +0x00
    CCountedPointer second;                  // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairCharStringBinding* __fastcall CopyBackward_PairCharStringBinding_00cbae30(PairCharStringBinding* first, PairCharStringBinding* last, PairCharStringBinding* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}