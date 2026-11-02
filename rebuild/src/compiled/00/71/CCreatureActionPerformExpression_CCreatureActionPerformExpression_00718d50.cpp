// CCreatureAction_PerformExpression::~CCreatureAction_PerformExpression() -- implicit destructor: PPerformToThing, then the PExpressionDef counted def handle, then ~CCreatureActionBase by tail jump
// Leaf: ??1CCreatureAction_PerformExpression@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00718D50, 52 bytes, thiscall, tail `jmp ~CCreatureActionBase`.
// Identity (proven): callers are the scalar deleting destructor 0x0084B600 (slot 0 of vtable 0x012743DC, RTTI
//   .?AVCCreatureAction_PerformExpression@@) and 0x007EF3FD / 0x007EF465, which destroy a by-value action built by
//   CCreatureAction_PerformExpression's constructor 0x008430B0 (stores 0x012743DC, then PExpressionDef +0xA8 with AddRef,
//   PPerformToThing +0xAC with the CIntelligentPointer<CThing> vtable 0x01238C6C). FableWin.pdb spells
//   ??1CCreatureAction_PerformExpression@@UAE@XZ (donor 0x01DCDA70, after its ??_E); Ego_r.pdb CCreatureAction_PerformExpression
//   : CCreatureActionBase has PExpressionDef (CDefPointer<const CExpressionDef>) / PPerformToThing (CIntelligentPointer<CThing>)
//   at +0xB4 / +0xB8, i.e. retail +0xA8 / +0xAC (CCreatureActionBase is 0xA8 in retail, 12-byte STLport containers). No
//   0x012743DC store: the destructor is the implicit one. Byte twin of ~CCreatureAction_PerformExpressionExtended 0x0084ADC0
//   (a sibling class with the same two leading members); the catalog label (that sibling's name) is propagated.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, CCreatureActionBase::~CCreatureActionBase 0x00693EF0;
//   the def release is CIVCountedPointeeBase's inline Release plus the virtual Delete (slot 1).
// Shape: members in reverse order: the implicit ~CIntelligentPointer<CThing> (the out-of-line base dtor), engine/CDefPointer.h's
//   inline ~CDefPointer (`dec [ecx+4]; jne; call [vptr+4]; Object = 0` inside the null test), then the base dtor. DestroyOne (a
//   destruction by name, ungraded; chunk 656/657 pattern) makes VC7.1 emit the out-of-line implicit `??1`.
// Layout: CCreatureActionBase modelled locally at its retail size 0xA8 (as the landed ~CCreatureAction_ShowItemToThing 0x007ED750):
//   engine/CCreatureActionBase.h has the retail size but flattens the vptr into a data member, so it cannot be the polymorphic
//   base whose out-of-line virtual destructor 0x00693EF0 this body calls (header debt). engine/CCreatureAction_PerformExpression.h
//   is the flattened generated layout (no base, members as opaque bytes): header debt.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CBaseObject;
class CBaseObjectPointer;
class CThing;
class CThingCreatureBase;
class CExpressionDef;

class CBaseIntelligentPointer {                             // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: vtable 0x01238C6C
public:
    CIntelligentPointer() { }
};

class CCreatureActionBase {                                 // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
private:
    unsigned char _body[0xa8 - 4];
};

class CCreatureAction_PerformExpression : public CCreatureActionBase {   // vtable 0x012743DC, retail 0xC8 bytes
public:
    CCreatureAction_PerformExpression(CThingCreatureBase& creature, CThing* performToThing,
        const CDefPointer<const CExpressionDef>& expressionDef, long, long, long, long);   // 0x008430B0
    CDefPointer<const CExpressionDef> PExpressionDef;       // +0xA8
    CIntelligentPointer<CThing>       PPerformToThing;      // +0xAC
    long                              ExpressionItemDefIndex;   // +0xB4
    bool                              EmoteIconAdded;       // +0xB8
    bool                              ReplacedAction;       // +0xB9
    bool                              Extended;             // +0xBA
    bool                              ButtonHeld;           // +0xBB
    unsigned long                     SoundEffectID;        // +0xBC
    long                              ExtraData1;           // +0xC0
    long                              ExtraData2;           // +0xC4
};

void DestroyOne(CCreatureAction_PerformExpression* action)
{
    action->CCreatureAction_PerformExpression::~CCreatureAction_PerformExpression();
}