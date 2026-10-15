// CThingDeadCreature::PeekDefinition() const -- the creature definition of this dead creature, fetched through a temporary counted def pointer (second instantiation, its own GetDef body)
// Grade: DIFFER(72v60) -- even the hand-written release on a pointer local: the lane loads the filled slot into eax, copies it to esi and forms `&m_refCount` (`add eax,4 ; dec edx ; mov ecx,edx ; test ecx,ecx`) where retail loads into esi and does `dec [esi+4] ; jne`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083D900, length 60 bytes (+ int3 padding).
// Callees: the thing's definition-manager accessor @ 0x004C7990 (`[g_main]+0x20`, `this` unused),
// CDefinitionManager::GetDef<T>(long, CDefPointer&) @ 0x0083E0D0 (landed), the def's virtual slot 1 (+4) when the
// count drops to zero.
// Layout (retail, local model): CThing's definition global index (unsigned short) at +0x8c; a counted definition holds
// its count at +4 behind the vptr.
// Shape: Ego_r.pdb `?PeekDefinition@CThingDeadCreature@@IBEABVCThingCreatureDef@@XZ` returns a const reference. The
// def pointer is zero-initialised in the `push ecx` slot, the arguments are pushed before the accessor runs (a chained
// call); retail loads the filled pointer ONCE into esi, releases through it (`dec [esi+4] ; jne`) and returns it. A
// CDefPointer destructor makes the lane load the address-taken slot into eax and form `&m_refCount` (`add eax,4 ; dec
// edx ; test edx,edx`), so the release is written by hand on a named pointer local (same behaviour).
struct CDefPointeeBase {                        // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
    long m_refCount;                            // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

template<class T> struct CDefPointer {          // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
};

struct CThingCreatureDef : CDefPointeeBase {
};

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingCreatureDef>& out);   // 0x0083E0D0
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
    CThingCreatureDef* creatureDef = def.m_pObject;
    if (creatureDef)
        creatureDef->ReleaseDef();
    return *creatureDef;
}
