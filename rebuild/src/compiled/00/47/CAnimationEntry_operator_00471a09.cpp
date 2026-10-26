// CAnimationEntry::operator=(const CAnimationEntry&) -- memberwise copy: three plain dwords, then the intrusive counted pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471A09, length 70 bytes.
// Callees: virtual slot 1 (+0x04) of the old component map (Destroy) when its count reaches zero.
// Layout: PDB CAnimationEntry (0x10): BankIndex, CDefString AnimName, CDefString GroupName,
// CIVCountedPointer<CAnimComponentMap> Components at +0x0c.
// Shape: the implicit memberwise assignment written out (so the symbol is emitted out of line); the counted
// pointer's operator= is `if (p != o.p) { Release(); p = o.p; if (p) ++p->count; }` with Release inlined
// (`dec [ecx+4] ; jne ; call [eax+4]`, then the null store). `and dword ptr [esi+0Ch],0` for the null store
// marks a size-optimised translation unit: #pragma optimize("s", on).
#pragma optimize("s", on)
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
    CAnimationEntry& operator=(const CAnimationEntry& other);
};

CAnimationEntry& CAnimationEntry::operator=(const CAnimationEntry& other)
{
    BankIndex = other.BankIndex;
    AnimName = other.AnimName;
    GroupName = other.GroupName;
    Components = other.Components;
    return *this;
}