// NTCScriptedControl::CActionPerformExpression::CActionPerformExpression(CThing* target, const CCharString& expression,
// long extraData1, long extraData2) -- constructor: the common action members zeroed, own vtable, an empty target
// CScriptThing, not performed, the expression and the two extra data words; a live target is adopted.
// Leaf: ??0CActionPerformExpression
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00905360, length 182 bytes (+ int3 padding), `ret 10h`.
// Callees: CBase::CBase @ 0x0099A2F0 (action root), the CScriptThing root ctor @ 0x0099A2D0, CCharString copy ctor
// @ 0x0099EC30, CThing::GetScriptThing @ 0x004C7E50 (CScriptThing by hidden pointer), CScriptThing::operator=
// @ 0x008AB980, the temporary's inline destructor (vtable 0x01238C8C, counted pimpl release: destroy + operator delete
// @ 0x00BFE9BC, CBase::~CBase @ 0x0099A2E0). Vtable 0x0128D8EC = NTCScriptedControl::CActionPerformExpression.
// Ego_r.pdb CActionPerformExpression (0x28): PTargetThing +0x0c, Performed +0x18, Expression +0x1c, ExtraData1 +0x20,
// ExtraData2 +0x24.
// Shape: the landed CActionFireProjectileWeapon (0x00716790) / CM_HiddenBooty::Init (0x00E93080) models: novtable
// CAction intermediate with ebx-zero stores, inline CScriptThing default ctor and destructor; the dead-bit test on the
// raw target pointer is inline.
void __cdecl operator delete(void* p);             // 0x00BFE9BC

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);          // 0x0099EC30
};

struct CBase {                                      // the action root
    virtual void v00();
    CBase();                                        // 0x0099A2F0
};

struct CScriptBase {                                // CScriptThing's root (manifest: CBase::CBase / ~CBase)
    virtual void v00();
    CScriptBase();                                  // 0x0099A2D0
    ~CScriptBase();                                 // 0x0099A2E0
};

struct SCountBlock {
    long  m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
};
struct CScriptThingImp;
struct CScriptThingPImp {                           // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    SCountBlock*     Info;
    CScriptThingPImp() : Data(0), Info(0) { }
    __forceinline ~CScriptThingPImp()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThing : CScriptBase {                 // 12 bytes, vtable 0x01238C8C
    CScriptThingPImp PImp;
    virtual void v00();
    CScriptThing() { }
    __forceinline ~CScriptThing() { }
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
};

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    CScriptThing GetScriptThing();                  // 0x004C7E50
};

struct __declspec(novtable) CAction : CBase {       // NTCScriptedControl::CActionBase: abstract, its own vtable store is dead
    void*   PTCScript;                              // +0x04 CTCScriptedControl*
    bool    Activated;                              // +0x08
    bool    Terminated;                             // +0x09
    CAction() : CBase(), PTCScript(0), Activated(false), Terminated(false) { }
};

struct CActionPerformExpression : CAction {         // vtable 0x0128D8EC
    CScriptThing PTargetThing;                      // +0x0c
    bool         Performed;                         // +0x18
    CCharString  Expression;                        // +0x1c
    long         ExtraData1;                        // +0x20
    long         ExtraData2;                        // +0x24
    virtual void v00();
    CActionPerformExpression(CThing* target, const CCharString& expression, long extraData1, long extraData2);
};

CActionPerformExpression::CActionPerformExpression(CThing* target, const CCharString& expression, long extraData1,
                                                   long extraData2)
    : CAction()
    , PTargetThing()
    , Performed(false)
    , Expression(expression)
    , ExtraData1(extraData1)
    , ExtraData2(extraData2)
{
    if (target != 0 && !(target->m_flags91 & 1))
        PTargetThing = target->GetScriptThing();
}