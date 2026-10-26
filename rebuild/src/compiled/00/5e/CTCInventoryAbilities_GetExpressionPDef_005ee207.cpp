// CTCInventoryAbilities::GetExpressionPDef(long) -- looks up the definition with the given global index and returns it as a counted CDefPointer<const CExpressionDef>
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE207, length 68 bytes.
// Callees: CTCBase::GetDefinitions() @ 0x006869D0 (`mov ecx,[ecx+4] ; jmp`: the parent thing's definition
//          manager), CDefinitionManager::GetPDefFromGlobalIndex(long) @ 0x009AD9E0 (CDefPointer<CDefClassBase>
//          returned through the hidden pointer, which VC homes in the dead `index` slot [ebp+0Ch]);
//          the def's virtual slot 1 (destroy at a zero count).
// Pragmas: frame-pointer size TU (`optimize("y", off)` + `("s", on)`).
// Shape: the base-typed temporary is converted into the typed result (a copy between two different
//        pointer types is never elided): result = temp, add-ref when non-null, then the temporary's
//        destructor releases through the ref-count base's inline accessors (`dec [eax] ; cmp [eax],0`,
//        the def kept in ecx for the virtual call); the converting constructor, the accessors and the
//        destructor are `__forceinline`
//        because a size TU calls them out of line (the converting constructor alone: 62v68). The model drops
//        the `const` of the pointee (the counted base is mutated). The arguments of GetPDefFromGlobalIndex are pushed
//        before the GetDefinitions() call that yields its `this`.

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                       // slot 1 (+0x04)
};

struct CDefRefCounted {                           // second base: the count sits at +4 behind the vptr
    long m_count;
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};

class CDefClassBase : public CDefVirtual, public CDefRefCounted { public:
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CExpressionDef : public CDefClassBase { public:
};

template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() { }
    template <class U> __forceinline CDefPointer(const CDefPointer<U>& other) : m_p((T*)other.m_p)
    {
        if (m_p)
            ++m_p->m_count;
    }
    __forceinline ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
};

class CDefinitionManager { public:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009AD9E0
};

struct CTCInventoryAbilities_Retail {
    CDefinitionManager* GetDefinitions();                           // 0x006869D0 (CTCBase)
    CDefPointer<CExpressionDef> GetExpressionPDef(long index);
};

CDefPointer<CExpressionDef> CTCInventoryAbilities_Retail::GetExpressionPDef(long index)
{
    return CDefPointer<CExpressionDef>(GetDefinitions()->GetPDefFromGlobalIndex(index));
}