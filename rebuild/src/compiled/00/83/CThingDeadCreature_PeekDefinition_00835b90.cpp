// CThingDeadCreature::PeekDefinition() const -- the creature definition of this dead creature, fetched through a temporary counted def pointer
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

class CDefPointeeBase : public CDefVirtual, public CRefCounted { public:
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

template<class T> class CDefPointer { public:          // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject)
            m_pObject->ReleaseDef();
    }
    T& operator*() const { return *m_pObject; }
};

class CThingCreatureDef : public CDefPointeeBase { public:
};

class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingCreatureDef>& out);   // 0x00513160 (GetDef<CThingCreatureDef>)
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CThingCreatureDef>& out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short m_defGlobalIndex;            // +0x8c
    CDefinitionManager* GetDefinitionManager() const;       // 0x004C7990
};

class CThingDeadCreature : public CThing { public:
    const CThingCreatureDef& PeekDefinition() const;
};

const CThingCreatureDef& CThingDeadCreature::PeekDefinition() const
{
    CDefPointer<CThingCreatureDef> def;
    GetDefinitionManager()->GetDef(m_defGlobalIndex, def);
    return *def;
}