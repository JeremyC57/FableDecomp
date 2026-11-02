// CCreatureAction_PerformExpressionExtended::~CCreatureAction_PerformExpressionExtended() -- implicit destructor: PPerformToThing, then the PExpressionDef counted def handle, then ~CCreatureActionBase by tail jump
// Leaf: ??1CCreatureAction_PerformExpressionExtended@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084ADC0, 52 bytes, thiscall, tail `jmp ~CCreatureActionBase`.
// Identity (proven): callers 0x00846EF0 / 0x00847722 destroy by-value actions built by CCreatureAction_PerformExpressionExtended's
//   constructor 0x00843310 (stores vtable 0x012744FC, RTTI .?AVCCreatureAction_PerformExpressionExtended@@; PExpressionDef +0xA8,
//   PPerformToThing +0xAC, ExpressionItemDefIndex +0xB4, EmoteIconAdded +0xB8, FrameCount +0xBC); the class's ??_G 0x0084B620
//   (slot 0 of 0x012744FC) inlines this same sequence. Ego_r.pdb (donor 0x00525EB0) and FableWin.pdb (0x01DD02A0) spell
//   ??1CCreatureAction_PerformExpressionExtended@@UAE@XZ; Ego_r lays the class out as CCreatureActionBase + PExpressionDef +0xB4,
//   PPerformToThing +0xB8, ExpressionItemDefIndex, EmoteIconAdded, FrameCount, ReplacedAction, SoundEffectID (retail -0xC).
//   No 0x012744FC store: the implicit destructor. Byte twin of ~CCreatureAction_PerformExpression 0x00718D50.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, CCreatureActionBase::~CCreatureActionBase 0x00693EF0;
//   the def release is CIVCountedPointeeBase's inline Release plus the virtual Delete (slot 1).
// Shape: as 0x00718D50: ~CIntelligentPointer<CThing>, engine/CDefPointer.h's inline ~CDefPointer, the base dtor. DestroyOne (ungraded
//   emission helper, chunk 656/657 pattern) makes VC7.1 emit the out-of-line implicit `??1`.
// Layout: CCreatureActionBase modelled locally at its retail size 0xA8 (as the landed 0x007ED750): engine/CCreatureActionBase.h
//   has the retail size but flattens the vptr into a data member, so it cannot be the polymorphic base whose out-of-line
//   virtual destructor 0x00693EF0 this body calls (header debt). The generated
//   engine/CCreatureAction_PerformExpressionExtended.h is flattened (no base): header debt.
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

class CCreatureAction_PerformExpressionExtended : public CCreatureActionBase {   // vtable 0x012744FC, retail 0xC8 bytes
public:
    CCreatureAction_PerformExpressionExtended(CThingCreatureBase& creature, CThing* performToThing,
        const CDefPointer<const CExpressionDef>& expressionDef, long, bool, unsigned long, long, long);   // 0x00843310
    CDefPointer<const CExpressionDef> PExpressionDef;       // +0xA8
    CIntelligentPointer<CThing>       PPerformToThing;      // +0xAC
    long                              ExpressionItemDefIndex;   // +0xB4
    bool                              EmoteIconAdded;       // +0xB8
    long                              FrameCount;           // +0xBC
    bool                              ReplacedAction;       // +0xC0
    unsigned long                     SoundEffectID;        // +0xC4
};

void DestroyOne(CCreatureAction_PerformExpressionExtended* action)
{
    action->CCreatureAction_PerformExpressionExtended::~CCreatureAction_PerformExpressionExtended();
}