// CCreatureAction_Drink::CCreatureAction_Drink(CThingCreatureBase*, CThing*) -- creature action playing a random loaded "SCRIPT_HERO_DRINK" animation (speeds -1.0/0.0, silent sound pair) holding the thing to drink and an empty previously-carried pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00843510, length 154 bytes, `ret 8`. Vtable 0x0127473C.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetRandomLoadedAnim (__fastcall: owner in ecx,
//          name in edx) @ 0x006924B0, CCreatureActionBase::CCreatureActionBase(owner, float, float, CAnimationEntry*,
//          const CSoundPair&, long, long) @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0,
//          CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90
//          (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Shape: the literal converts implicitly to a CCharString temporary built first and re-addressed with `lea` for the
//   lookup; with the 0.0f speed among the arguments the zeros stay immediates (as 0x00916590); the temporary dies
//   after the base call; then the vtable store and the two pointer members, each addressed once in edi: PThingToDrink
//   (base ctor, vtable, SetPItem(drink)) and the default PThingPreviouslyCarrying (base ctor, vtable).
// Layout: Ego_r.pdb puts the pointers at +0xb4/+0xbc (PDB CCreatureActionBase 0xb4 bytes, retail 0xa8): +0xa8/+0xb0 here.
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
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation,
                        const CSoundPair& sound, long c, long d);                 // 0x00693B30
};

struct CCreatureAction_Drink : CCreatureActionBase {   // vtable 0x0127473C
    CIntelligentPointer<CThing> PThingToDrink;              // +0xa8 (PDB +0xb4)
    CIntelligentPointer<CThing> PThingPreviouslyCarrying;   // +0xb0 (PDB +0xbc)
    virtual void v00();
    CCreatureAction_Drink(CThingCreatureBase* owner, CThing* drink);
};

CCreatureAction_Drink::CCreatureAction_Drink(CThingCreatureBase* owner, CThing* drink)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetRandomLoadedAnim(owner, "SCRIPT_HERO_DRINK"), CSoundPair(), 0, 0)
    , PThingToDrink(drink)
    , PThingPreviouslyCarrying()
{
}
