// CCreatureAction_GiveMoneyToThing::CCreatureAction_GiveMoneyToThing(CThingCreatureBase*, CThing*, long) -- creature action playing a random loaded "GIVE_MONEY" animation (speeds -1.0/0.75, silent sound pair) that hands an amount of money to a receiver
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ED450, length 140 bytes, `ret 0Ch`. Vtable 0x0127139C.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetRandomLoadedAnim (__fastcall: owner in ecx,
//          name in edx) @ 0x006924B0, CCreatureActionBase::CCreatureActionBase(owner, float, float, CAnimationEntry*,
//          const CSoundPair&, long, long) @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0,
//          CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90
//          (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Shape: the implicit literal temporary is built first and re-addressed with `lea`; with a non-zero second speed one
//   eax zero serves the two long pushes and the CSoundPair constructor; after the string dies: vtable store,
//   PReceivingThing (base ctor, vtable, SetPItem(receiver)) and the Amount initialiser from the third argument slot.
// Layout: Ego_r.pdb puts PReceivingThing/Amount at +0xb4/+0xbc (PDB CCreatureActionBase 0xb4 bytes, retail 0xa8):
//   +0xa8/+0xb0 here; no engine header exists for this class.
struct CThingCreatureBase;
struct CThing;
struct CAnimationEntry;
struct CBaseObject;

struct CCharString {                        // one pointer; the defaulted length makes a literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CAnimationEntry* __fastcall GetRandomLoadedAnim(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CBaseIntelligentPointer {            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation,
                        const CSoundPair& sound, long c, long d);                 // 0x00693B30
};

struct CCreatureAction_GiveMoneyToThing : CCreatureActionBase {   // vtable 0x0127139C
    CIntelligentPointer<CThing> PReceivingThing;   // +0xa8 (PDB +0xb4)
    long Amount;                                   // +0xb0 (PDB +0xbc)
    virtual void v00();
    CCreatureAction_GiveMoneyToThing(CThingCreatureBase* owner, CThing* receiver, long amount);
};

CCreatureAction_GiveMoneyToThing::CCreatureAction_GiveMoneyToThing(CThingCreatureBase* owner, CThing* receiver, long amount)
    : CCreatureActionBase(owner, -1.0f, 0.75f, GetRandomLoadedAnim(owner, "GIVE_MONEY"), CSoundPair(), 0, 0)
    , PReceivingThing(receiver)
    , Amount(amount)
{
}
