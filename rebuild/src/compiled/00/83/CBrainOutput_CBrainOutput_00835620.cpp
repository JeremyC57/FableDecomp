// CBrainOutput::CBrainOutput() -- navigation input and action pointer zeroed, look-at pointer built, then types/flags cleared and the action reset
// Leaf: ??0CBrainOutput@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835620, length 89 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (LookAtThing +0x40, then
// the CIntelligentPointer vtable 0x01237F1C is stored by the inlined derived constructor), the
// action's destroy function through its count block (`call [eax+4]`), operator delete @ 0x00BFE9BC.
// Layout from Ego_r.pdb (CBrainOutput, 72 bytes): Types +0, LookFlags +4, NavigationInput +8
// (its inline constructor zeroes only its last three dwords, +0x14..+0x1c), PAction +0x20
// (counted pointer, zero-constructed), MoveToPos, LookAtPos, LookAtThing +0x40.
// Shape: the member constructors run in declaration order from one zero register; Types and
// LookFlags are cleared in the BODY (their stores follow the LookAtThing call), and the body then
// resets PAction to null (`Reset(0)` inlined with the constant: release the block, store the
// null count first and the null object second -- the Reset order, unlike the destructor's).
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F1C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCreatureActionBase;

struct SCountBlock {                            // the shared count block of a CCountedPointer
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);   // +0x04
    CCreatureActionBase* m_pObject;             // +0x08
};

struct CCountedPointer_CCreatureActionBase {    // {object, count block}
    CCreatureActionBase* m_pObject;             // +0x00
    SCountBlock*         m_pCount;              // +0x04
    CCountedPointer_CCreatureActionBase() : m_pObject(0), m_pCount(0) { }
    __forceinline void Reset(CCreatureActionBase* object)   // forced: VC7.1 would call it out of line
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = object;
    }
};

class C3DVector { public: float X, Y, Z; };

class CNavigationInput { public:                       // 24 bytes: the first three dwords are left to the caller
    long m_00;
    long m_04;
    long m_08;
    long m_0c;
    long MovementType;
    long m_14;
    CNavigationInput() : m_0c(0), MovementType(0), m_14(0) { }
};

class CBrainOutput { public:
    unsigned long                     Types;            // +0x00
    long                              LookFlags;        // +0x04
    CNavigationInput                  NavigationInput;  // +0x08
    CCountedPointer_CCreatureActionBase PAction;        // +0x20
    C3DVector                         MoveToPos;        // +0x28
    C3DVector                         LookAtPos;        // +0x34
    CIntelligentPointer<const CThing>               LookAtThing;      // +0x40
    CBrainOutput();
};

CBrainOutput::CBrainOutput()
    : NavigationInput(), PAction(), LookAtThing()
{
    Types = 0;
    LookFlags = 0;
    PAction.Reset(0);
}