// CCombatSequence_BattleCharge::OnCreate() -- stamp the use frame, read the charge value from the owner's sub-definition (0 when absent), queue a CCreatureAction_CastBattleCharge with it, then the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00909070, 206 bytes (208 with padding), virtual `?OnCreate@CCombatSequence_BattleCharge@@UAEXXZ`.
// Callees: the world frame counter `mov ecx,[0x013B86A0] ; call 0x004167C8` (incremental thunk onto GFGetCurrentFrame, as in
//   the landed IsValid 0x00908DA0), CDefClassBase::GetSubDef<T>(CDefPointer<T>&) 0x008E3240 on the owner's def (+0x70),
//   operator new 0x00BFEA1A (0x130 bytes), CCreatureAction_CastBattleCharge ctor 0x008CE290 (owner, target, 0, value),
//   CCountedPointer::Reset 0x00905FD0, AddAction 0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC; the def
//   is released through its vtable slot 1 when its count (+4) drops to zero.
// Layout: owner +0x1c, target +0x20, m_frameOfLastUse +0x28 (retail, as 0x00908DA0); the sub-def's value at +0x48 (T is the
//   opaque instantiation of the landed 0x008E3240, its label names CThunderBattleDef).
// Shape: the counted-pointer dtor's null stores survive because the def pointer's release follows; the def pointer is
//   released through its register-held copy (`dec [edi+4] ; jne`). The value local is declared before the def pointer
//   (zero-store order), the owner's def is read into a local before GetSubDef, owner/target reach the action through
//   inline accessors (the OnCreate family rule), and GetSubDef<T> has a same-TU no-escape stand-in body (COMDAT
//   instance): together they give retail without the parked /Oa flag.
// Leaf: OnCreate
class CThingCreatureBase;
class CThing;

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

class CSoundPair { public:                         // the zeroed two-dword sound argument of CCombatActionBase
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCombatActionBase { public:
    virtual void v00();
    CCombatActionBase() { }
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

struct CCountedPointer {
    CCombatActionBase* m_pObject;           // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(CCombatActionBase* object) { Reset(object); return *this; }
    void Reset(CCombatActionBase* object);  // 0x00905FD0
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

class CCombatSequenceBase { public:
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_pOwner;           // +0x1c
    CThing* m_pTarget;                      // +0x20
    CThingCreatureBase* GetCreature() const { return m_pOwner; }
    CThing* GetTarget() const { return m_pTarget; }
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

struct CWorld_Retail {
    long GetCurrentFrame();                                     // 0x004167C8 -> GFGetCurrentFrame 0x0049D870
};
extern CWorld_Retail* g_pWorld;                                 // 0x013B86A0

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                     // slot 1
    long m_count;                                               // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};

class CThunderBattleDef : public CDefBase { public:
    unsigned char _pad08[0x40];
    long m_value;                                               // +0x48
};

template<class T> class CDefPointer { public:
    T* m_pDef;
    CDefPointer() : m_pDef(0) { }
    ~CDefPointer()
    {
        if (m_pDef)
            m_pDef->Release();
    }
    T* operator->() const { return m_pDef; }
};

class CDefClassBase { public:
    template<class T> bool GetSubDef(CDefPointer<T>& out);     // 0x008E3240 (CThunderBattleDef instantiation)
};

void* FindSubDef_008e3240(CDefClassBase* def);
#pragma auto_inline(off)
template<class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    T* def = (T*)FindSubDef_008e3240(this);
    if (!def)
        return false;
    out.m_pDef = def;
    return true;
}
#pragma auto_inline(on)
class CThingCreatureBase { public:
    unsigned char _pad[0x70];
    CDefClassBase* m_pDef;                                      // +0x70
};

class CCreatureAction_CastBattleCharge : public CCombatActionBase { public:
    unsigned char _body[0x12c];                                 // sizeof = 0x130
    CCreatureAction_CastBattleCharge(CThingCreatureBase* owner, CThing* thing, long mode, long value);   // 0x008CE290
};

struct CCombatSequence_BattleCharge : CCombatSequenceBase {
    unsigned char _pad24[4];                                    // +0x24
    long m_frameOfLastUse;                                      // +0x28
    virtual void OnCreate();
};

void CCombatSequence_BattleCharge::OnCreate()
{
    m_frameOfLastUse = g_pWorld->GetCurrentFrame();
    long value = 0;
    CDefPointer<CThunderBattleDef> def;
    CDefClassBase* ownerDef = GetCreature()->m_pDef;
    if (ownerDef->GetSubDef(def))
        value = def->m_value;
    CCountedPointer action;
    action = new CCreatureAction_CastBattleCharge(GetCreature(), GetTarget(), 0, value);
    AddAction(action);
    CCombatSequenceBase::OnCreate();
}