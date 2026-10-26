// CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(CThingCreatureBase*, CThing*, long, long) -- base CCreatureActionBase(owner, -1.0, 0.5, the owner's table animation for the kind, zeroed CSoundPair, 0, 0), the thing pointer, the last argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008440C0, length 115 bytes (+ int3 padding). Vtable 0x012753E4.
// Callees: the table-animation lookup @ 0x006924B0 (`__fastcall`: ecx owner, edx kind), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*,
// float, float, CAnimationEntry*, const CSoundPair&, long, long) @ 0x00693B30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the landed PutDownThingAtTable constructor (0x00842A20) with the thing as the SECOND argument and the kind third; after the vtable
// store the CIntelligentPointer<CThing> member at +0xa8 is built from the thing (inline converting constructor: base ctor, its vtable
// 0x01238C6C, SetPItem), then the word at +0xb0.
class CThingCreatureBase;
class CThing;
class CAnimationEntry;
class CCharString;
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};
CAnimationEntry* __fastcall GetTableAnimation(CThingCreatureBase* owner, long kind);   // 0x006924B0

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
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCreatureAction_PickUpThingAtTable : public CCreatureActionBase { public:   // vtable 0x012753E4
    CIntelligentPointer<CThing> PThing;             // +0xa8 the thing picked up
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