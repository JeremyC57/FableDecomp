// _global::CopyBackward_PairLongPointerHolder_00bed9a0(PairLongPointerHolder*, PairLongPointerHolder*, PairLongPointerHolder*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 16-byte (key, polymorphic counted-pointer holder) pairs, returns the moved-back result
// Leaf: CopyBackward_PairLongPointerHolder_00bed9a0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BED9A0, length 133 bytes.
// Callees: operator delete @ 0x00BFE9BC (the count block's destroy function is called through its slot +4).
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`); the signed count is computed once and
// counted down in the dead `result` argument slot (`dec dword ptr [esp+18h]`), both pointers step back one element
// per iteration and the moved-back result is returned.
// Element: 16 bytes (`sar 4`). The copy moves the word at +0 and the counted pointer at +8 but never the word at +4:
// a vtable pointer, so the second member is a polymorphic holder {vptr, CCountedPointer} whose implicit operator=
// leaves the vptr alone (GOTCHAS: an element copy that skips the word at +4). The manifest label
// pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > would copy +4: propagated.
// The element's implicit operator= runs the inlined CCountedPointer::operator= (the source's count block and object
// are read first, kept in ebx and the `push ecx` frame word across the release; when the blocks differ the old block
// is released -- destroy function at +4 with the object at +8 in ecx, then operator delete -- and both words plus a
// new reference are adopted). Same model as the landed 0x0084F130 / 0x00CBAE30.
struct SCountBlock {
    long m_references;                       // +0
    void (__fastcall* m_pfnDestroy)(void*);  // +4
    void* m_pObject;                         // +8
};

struct CCountedPointer {                     // CCountedPointer<T>, 8 bytes
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

struct CCountedPointerHolder {               // polymorphic 12-byte holder
    virtual void v00();                      // +0x00 vtable (not copied by the implicit operator=)
    CCountedPointer m_pObject;               // +0x04
};

struct PairLongPointerHolder {               // std::pair<long, CCountedPointerHolder>, 16 bytes
    long                  first;             // +0x00
    CCountedPointerHolder second;            // +0x04
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

PairLongPointerHolder* __fastcall CopyBackward_PairLongPointerHolder_00bed9a0(PairLongPointerHolder* first, PairLongPointerHolder* last, PairLongPointerHolder* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}