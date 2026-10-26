// Test for 0x006C5630 CTCHeroOpinionDeedLog::PeekPDeedEffectsDef: the def at the deed type's table index is returned
// with one extra reference; a missing def gives a null pointer.
#include <stdio.h>
static int g_destroyed = 0; static long g_lastIndex = -1;
class CDefBase {                                       // counted def object: vptr, reference count at +4
public:
    virtual void v00() { }
    virtual void Destroy() const { ++g_destroyed; }
    mutable long RefCount;                             // +0x04
};
class COpinionDeedEffectsDef : public CDefBase { };

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
};

class CDefinitionManager {
public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x006C76B0 (T = COpinionDeedEffectsDef)
};
class CGameDefinitionManager : public CDefinitionManager { };

class CTCBase {
public:
    virtual ~CTCBase() { }
protected:
    void* ParentThing;                                 // +0x04
    bool  ToBeRemoved;                                 // +0x08
    bool  IsInGlobalUpdateTCs;                         // +0x09
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    friend int main();
};

enum EOpinionDeedType { };

class CTCHeroOpinionDeedLog : public CTCBase {
public:
    unsigned char  _pad0c[0x2E - 0x0C];                // OpinionDeedMap .. GuardsIgnoreCrimes
    unsigned short DeedEffectsDefIndices[92];          // +0x2E
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType type) const;
};

CDefPointer<const COpinionDeedEffectsDef> CTCHeroOpinionDeedLog::PeekPDeedEffectsDef(EOpinionDeedType type) const
{
    CDefPointer<const COpinionDeedEffectsDef> def;
    GetDefinitionManager().GetDef(DeedEffectsDefIndices[type], &def);
    return def;
}

static COpinionDeedEffectsDef g_defs[3];
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    g_lastIndex = index;
    if (index <= 0 || index > 2) return false;
    out->Object = &g_defs[index];
    ++g_defs[index].RefCount;
    return true;
}
static CGameDefinitionManager g_manager;
const CGameDefinitionManager& CTCBase::GetDefinitionManager() const { return g_manager; }
int main()
{
    CTCHeroOpinionDeedLog log;
    bool ok = (char*)&log.DeedEffectsDefIndices[0] - (char*)&log == 0x2E;
    log.DeedEffectsDefIndices[5] = 2; log.DeedEffectsDefIndices[6] = 0;
    g_defs[2].RefCount = 1;
    {
        CDefPointer<const COpinionDeedEffectsDef> d = log.PeekPDeedEffectsDef((EOpinionDeedType)5);
        ok = ok && g_lastIndex == 2 && d.Object == &g_defs[2] && g_defs[2].RefCount == 2;
    }
    ok = ok && g_defs[2].RefCount == 1 && g_destroyed == 0;
    CDefPointer<const COpinionDeedEffectsDef> e = log.PeekPDeedEffectsDef((EOpinionDeedType)6);
    ok = ok && g_lastIndex == 0 && e.Object == 0;
    if (!ok) { printf("006c5630_TEST FAIL\n"); return 1; }
    printf("006c5630_TEST PASS\n");
    return 0;
}
