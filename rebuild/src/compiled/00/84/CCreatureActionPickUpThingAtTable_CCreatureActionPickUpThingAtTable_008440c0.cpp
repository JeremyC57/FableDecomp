// CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(CThingCreatureBase*, CThing*, long, long) -- base CCreatureActionBase(owner, -1.0, 0.5, the owner's table animation for the kind, zeroed CSoundPair, 0, 0), the thing pointer, the last argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008440C0, length 115 bytes (+ int3 padding). Vtable 0x012753E4.
// Callees: the table-animation lookup @ 0x006924B0 (`__fastcall`: ecx owner, edx kind), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*,
// float, float, CAnimationEntry*, const CSoundPair&, long, long) @ 0x00693B30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the landed PutDownThingAtTable constructor (0x00842A20) with the thing as the SECOND argument and the kind third; after the vtable
// store the CIntelligentPointer<CThing> member at +0xa8 is built from the thing (inline converting constructor: base ctor, its vtable
// 0x01238C6C, SetPItem), then the word at +0xb0.
struct CThingCreatureBase;
struct CThing;
struct CAnimationEntry;
struct CCharString;
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};
CAnimationEntry* __fastcall GetTableAnimation(CThingCreatureBase* owner, long kind);   // 0x006924B0

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CCreatureAction_PickUpThingAtTable : CCreatureActionBase {   // vtable 0x012753E4
    CIntelligentPointer PThing;             // +0xa8 the thing picked up
    long m_tablePoint;                      // +0xb0 (descriptive: the fourth argument)
    virtual void v00();
    CCreatureAction_PickUpThingAtTable(CThingCreatureBase* owner, CThing* thing, long kind, long tablePoint);
};

CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(CThingCreatureBase* owner, CThing* thing, long kind, long tablePoint)
    : CCreatureActionBase(owner, -1.0f, 0.5f, GetTableAnimation(owner, kind), CSoundPair(), 0, 0)
    , PThing(thing)
    , m_tablePoint(tablePoint)
{
}