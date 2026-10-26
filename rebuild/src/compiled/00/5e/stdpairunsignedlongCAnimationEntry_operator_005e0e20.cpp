// std::pair<unsigned long, CAnimationEntry>::operator=(const pair&) -- the CVectorMap element copy: key, then the entry's three dwords and its counted pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0E20, length 77 bytes (+ int3 padding).
// Callees: virtual slot 1 (+0x04) of the old component map (Destroy) when its count reaches zero.
// Layout: the element of CAnimationSet::Anims (CVectorMap<unsigned long, CAnimationEntry>): first at +0x00, then
// the PDB CAnimationEntry (BankIndex +0x04, AnimName +0x08, GroupName +0x0c, Components +0x10) -- 0x14 bytes.
// The manifest's "CAnimationEntry::operator=" label is the 0x10-byte body @ 0x00471A09; this one copies a
// fourth dword before the counted pointer at +0x10, so it is the pair's assignment.
// Shape: memberwise with the CIVCountedPointer operator= inlined (`if (p != o.p) { Release(); p = o.p;
// if (p) ++p->count; }`); plain `mov [esi+10h],0` null store (not a size TU).
class CIVCountedPointeeBase { public:
    virtual void v0();
    virtual void Destroy();                     // slot 1 (+0x04): called when the intrusive count hits zero
    long m_refCount;                            // +0x04
};

// Intrusive counted pointer (4 bytes): the pointee carries the count at +4.
template<class T> class CIVCountedPointer { public:
    T* m_pObject;
    __forceinline void Release()
    {
        T* p = m_pObject;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_pObject = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            Release();
            T* p = other.m_pObject;
            m_pObject = p;
            if (p)
                ++p->m_refCount;
        }
        return *this;
    }
};

class CDefString { public: long TablePos; };
class CAnimComponentMap : public CIVCountedPointeeBase { public: };

class CAnimationEntry { public:                        // PDB layout, 0x10 bytes
    long BankIndex;                             // +0x00
    CDefString AnimName;                        // +0x04
    CDefString GroupName;                       // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;    // +0x0c
};

namespace std {
template<class A, class B> struct pair {
    A first;
    B second;
    pair& operator=(const pair& other);
};
template<class A, class B> pair<A, B>& pair<A, B>::operator=(const pair& other)
{
    first = other.first;
    second = other.second;
    return *this;
}
template struct pair<unsigned long, CAnimationEntry>;
}