// CCreatureAction_SummonerFlameSlice::CCreatureAction_SummonerFlameSlice(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, target, 150, zeroed CSoundPair), own vtable (0x01264214), a default CIntelligentPointer at +0x114
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CBCA0, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the CIntelligentPointer vtable 0x01238C6C).
// Shape: the target argument is loaded into edx before the frame is set up; the sound pair is zeroed by its inline constructor
// (register-zeroed stack stores); the member pointer is constructed after the vtable store, addressed once in edi. `ret 8`.
class CThingCreatureBase;
class CThing;
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C; 8 bytes
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

class CCreatureAction_SummonerFlameSlice : public CCombatActionBase { public:   // vtable 0x01264214
    CIntelligentPointer<CThing> PSummonedThing;     // +0x114 (the pointer FinishAction @ 0x008CAD10 kills)
    virtual void v00();
    CCreatureAction_SummonerFlameSlice(CThingCreatureBase* owner, CThing* target);
};

CCreatureAction_SummonerFlameSlice::CCreatureAction_SummonerFlameSlice(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , PSummonedThing()
{
}