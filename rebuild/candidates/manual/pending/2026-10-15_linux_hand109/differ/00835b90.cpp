// CThingDeadCreature::PeekDefinition() const -- the creature definition of this dead creature, fetched through a temporary counted def pointer
// Grade: DIFFER(70v60) -- the def pointer is loaded into eax then copied to esi, and the release forms `&m_refCount` (`add eax,4 ; dec edx ; test edx,edx`) where retail loads straight into esi and does `dec [esi+4] ; jne`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835B90, length 60 bytes (+ int3 padding).
// Callees: the thing's definition-manager accessor @ 0x004C7990 (`[g_main]+0x20`, `this` unused),
// CDefinitionManager::GetDef<CThingCreatureDef>(long, CDefPointer&) @ 0x00513160, the def's virtual slot 1 (+4) when the
// temporary drops the last reference.
// Layout (retail, local model): CThing's definition global index (unsigned short) at +0x8c; a counted definition holds
// its count at +4 behind the vptr.
// Shape: Ego_r.pdb `?PeekDefinition@CThingDeadCreature@@IBEABVCThingCreatureDef@@XZ` returns a const reference; the
// CDefPointer temporary is zero-initialised in the `push ecx` slot, the arguments are pushed before the accessor runs
// (a chained call), the def is kept in esi across the inline release and returned.
struct CDefVirtual {                            // the definition's vptr
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
};

struct CRefCounted {                            // second base: the count sits at +4 behind the vptr
    long m_refCount;
};

struct CDefPointeeBase : CDefVirtual, CRefCounted {
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

template<class T> struct CDefPointer {          // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject)
            m_pObject->ReleaseDef();
    }
    T& operator*() const { return *m_pObject; }
};

struct CThingCreatureDef : CDefPointeeBase {
};

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingCreatureDef>& out);   // 0x00513160 (GetDef<CThingCreatureDef>)
};

struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short m_defGlobalIndex;            // +0x8c
    CDefinitionManager* GetDefinitionManager() const;       // 0x004C7990
};

struct CThingDeadCreature : CThing {
    const CThingCreatureDef& PeekDefinition() const;
};

const CThingCreatureDef& CThingDeadCreature::PeekDefinition() const
{
    CDefPointer<CThingCreatureDef> def;
    GetDefinitionManager()->GetDef(m_defGlobalIndex, def);
    return *def;
}
