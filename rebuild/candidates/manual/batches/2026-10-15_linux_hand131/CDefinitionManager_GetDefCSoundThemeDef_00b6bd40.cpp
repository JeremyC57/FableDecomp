// CDefinitionManager::GetDef<CSoundThemeDef>(const unsigned long& classKey, long index, CDefPointer<const CSoundThemeDef>& out) -- fetch the def at `index` of the class through GetPDefFromGlobalIndex; on a hit reassign the out pointer (old def released, new one add-ref'd), release the temporary and return true; false for index <= 0 or a null def
// Leaf: GetDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6BD40, length 113 bytes (`ret 0Ch`).
// Callees: CDefinitionManager::GetPDefFromGlobalIndex(const unsigned long&, long) @ 0x009ADE10 (landed; CDefPointer by value
// through the hidden pointer), the def's virtual slot 1 (destroy at a zero count).
// Shape: the landed GetDef family source (0x00430096, a frame-pointer size TU) compiled for speed: the hidden result is
// homed in the dead `index` argument slot and re-read from it at every use; `out = def` is the inline CDefPointer
// assignment (self test, release + null store of the old def, store + add-ref of the new one); the temporary's destructor
// releases through the ref-count base's two inline accessors (`mov esi,[eax+4] ; ... ; dec esi ; mov edx,esi ; test edx,edx
// ; mov [eax],esi`); the def type is opaque.
struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};

struct CDefBase : CDefVirtual, CRefCounted {
    void Release()                         // inlined into CDefPointer::operator=
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()                // inlined into ~CDefPointer
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

struct CDefPointer {
    CDefBase* m_p;
    CDefPointer() { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};

struct CDefinitionManager {
    CDefPointer GetPDefFromGlobalIndex(const unsigned long& kind, long index);   // 0x009ADE10 (hidden result pointer)
    bool GetDef(const unsigned long& kind, long index, CDefPointer& out);
};

bool CDefinitionManager::GetDef(const unsigned long& kind, long index, CDefPointer& out)
{
    if (index <= 0)
        return false;
    CDefPointer def = GetPDefFromGlobalIndex(kind, index);
    if (def.m_p) {
        out = def;
        return true;
    }
    return false;
}