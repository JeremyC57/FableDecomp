// CCreatureAction_PerformExpressionExtended::CCreatureAction_PerformExpressionExtended(const CCreatureAction_PerformExpressionExtended&) -- copy constructor: base copy, add-ref'd expression def, the target pointer, then the expression settings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084BE30, length 146 bytes (+ int3 padding). Vtable 0x012744FC.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: `ret 4`; explicit memberwise copy in declaration order. PExpressionDef is a CDefPointer whose inline copy constructor copies
// the pointer and bumps the def's count at +4 (`inc dword ptr [eax+4]`) when non-null; PPerformToThing's inline copy constructor is the
// base ctor, its vtable (0x01238C6C), then SetPItem(other.GetPItem()). Members = PDB names (PDB +0xb4.. sit 0xc high).
class CThing;
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CExpressionDef { public:
    void* __vftable;
    long  m_refCount;                       // +4 CDefPointee count
};
struct CExpressionDefPointer {              // CDefPointer<CExpressionDef const>
    CExpressionDef* m_pDef;
    CExpressionDefPointer(const CExpressionDefPointer& other) : m_pDef(other.m_pDef)
    {
        if (m_pDef)
            ++m_pDef->m_refCount;
    }
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
};
class CCreatureAction_PerformExpressionExtended : public CCreatureActionBase { public:   // vtable 0x012744FC
    CExpressionDefPointer PExpressionDef;   // +0xa8
    CIntelligentPointer<CThing>   PPerformToThing;  // +0xac
    long                  ExpressionItemDefIndex;   // +0xb4
    bool                  EmoteIconAdded;   // +0xb8
    long                  FrameCount;       // +0xbc
    bool                  ReplacedAction;   // +0xc0
    unsigned long         SoundEffectID;    // +0xc4
    virtual void v00();
    CCreatureAction_PerformExpressionExtended(const CCreatureAction_PerformExpressionExtended& other);
};

CCreatureAction_PerformExpressionExtended::CCreatureAction_PerformExpressionExtended(const CCreatureAction_PerformExpressionExtended& other)
    : CCreatureActionBase(other)
    , PExpressionDef(other.PExpressionDef)
    , PPerformToThing(other.PPerformToThing)
    , ExpressionItemDefIndex(other.ExpressionItemDefIndex)
    , EmoteIconAdded(other.EmoteIconAdded)
    , FrameCount(other.FrameCount)
    , ReplacedAction(other.ReplacedAction)
    , SoundEffectID(other.SoundEffectID)
{
}