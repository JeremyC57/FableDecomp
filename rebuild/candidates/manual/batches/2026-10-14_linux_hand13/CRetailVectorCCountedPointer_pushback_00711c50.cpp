// std::vector<CCountedPointer<T>>::push_back(const CCountedPointer<T>&) -- retail VC6-era vector: when there is
// spare capacity the element is placement-copied at `last` (the counted-pointer copy constructor bumps the
// pointee's reference count) and `last` advances; otherwise the out-of-line grow-and-insert runs.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711C50, length 57 bytes.
// Callees: CRetailVector::GrowInsert 0x00711A50 (where, value, tag, count, count -- the 12-byte slot
//          temporary `lea edx,[esp+0Ch]` is the empty tag homed in the dead argument slot).
// The `test eax,eax` before the inline copy is the compiler's own placement-new null check; the pointee's
// reference count lives at +4 of the object (`inc dword ptr [edx+4]`).
struct CRefCounted {
    void* __vftable;
    long m_refCount;                                     // +4
};

struct CCountedPointer {
    CRefCounted* m_pObject;
    CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject) { if (m_pObject) ++m_pObject->m_refCount; }
};

struct SInsertTag { SInsertTag() {} };

inline void* operator new(unsigned int, void* where) { return where; }

template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    void GrowInsert(T* where, const T& value, const SInsertTag& tag, unsigned long count, unsigned long construct);   // 0x00711A50
    void push_back(const T& value);
};

template<class T> void CRetailVector<T>::push_back(const T& value)
{
    if (last != end_of_storage) {
        new (last) T(value);
        ++last;
    } else {
        GrowInsert(last, value, SInsertTag(), 1, 1);
    }
}

template struct CRetailVector<CCountedPointer>;
