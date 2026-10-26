// CCreatureAction_SitDown::CCreatureAction_SitDown(CThingCreatureBase*, CAnimationEntry*, CCountedPointer<CModeSitBase>) -- base CCreatureActionBase(owner, -1.0, 0.0, animation, zeroed CSoundPair, 0, 1), own vtable, the sitting mode kept as a counted pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AB9D0, length 123 bytes (+ int3 padding).
// Callees: CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*, const CSoundPair&, long, long) @ 0x00693B30,
// the count block's destroy function (`mov ecx,[edi+8] ; call [edi+4]`), operator delete @ 0x00BFE9BC. Own vtable 0x012781FC.
// Shape: `ret 10h` = owner, animation and the two words of the BY-VALUE counted pointer, which the constructor destroys itself: the member's
// inline copy constructor (add-ref) and the parameter's inline destructor (release) share one null test and are folded into
// `inc ecx ; mov eax,ecx ; dec eax` with both count stores kept (GOTCHAS 24d); the parameter's dead field clears are dropped.
// Layout: PMode (CCountedPointer<CModeSitBase>) at +0xa8/+0xac; the PDB header (CCreatureAction_SitDown.h) sits 0x0c high, modelled locally.
class CThingCreatureBase;
class CAnimationEntry;
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CModeSitBase;
struct SCountBlock {                        // the shared count block of a CCountedPointer
    long m_references;                      // +0x00
    void (__fastcall* m_pfnDestroy)(CModeSitBase*);   // +0x04
    CModeSitBase* m_pObject;                // +0x08
};
struct CCountedPointer_CModeSitBase {       // {object, count block}
    CModeSitBase* m_pObject;                // +0x00
    SCountBlock*  m_pCount;                 // +0x04
    CCountedPointer_CModeSitBase(const CCountedPointer_CModeSitBase& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer_CModeSitBase()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};
class CCreatureAction_SitDown : public CCreatureActionBase { public:          // vtable 0x012781FC; retail sizeof 0xb0
    CCountedPointer_CModeSitBase PMode;     // +0xa8
    virtual void v00();
    CCreatureAction_SitDown(CThingCreatureBase* owner, CAnimationEntry* animation, CCountedPointer_CModeSitBase mode);
};

CCreatureAction_SitDown::CCreatureAction_SitDown(CThingCreatureBase* owner, CAnimationEntry* animation, CCountedPointer_CModeSitBase mode)
    : CCreatureActionBase(owner, -1.0f, 0.0f, animation, CSoundPair(), 0, 1)
    , PMode(mode)
{
}