// CCreatureAction_LearnExpression::CCreatureAction_LearnExpression(CThingPlayerCreature*, CThing*) -- the player plays a random loaded "STANDARD_CAST" animation (speeds -1.0/0.0, silent sound pair) to learn an expression from a thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00843470, length 155 bytes, `ret 8`. Vtable 0x0127461C.
// Identity: the stored vtable 0x0127461C is CCreatureAction_LearnExpression (rtti-vtables.tsv); the catalog label
//   "CCreatureAction_GiveItemToThing" is propagated. Ego_r.pdb: PPlayerCreature CIntelligentPointer<CThingPlayerCreature>
//   at +0xb4 and PLearnFromThing CIntelligentPointer<CThing> at +0xbc (PDB CCreatureActionBase 0xb4 bytes, retail
//   0xa8), so +0xa8/+0xb0 here.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetRandomLoadedAnim (__fastcall: owner in ecx,
//          name in edx) @ 0x006924B0, CCreatureActionBase::CCreatureActionBase(owner, float, float, CAnimationEntry*,
//          const CSoundPair&, long, long) @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0,
//          CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90
//          (vtables CIntelligentPointer<CThingPlayerCreature> 0x01231C4C, CIntelligentPointer<CThing> 0x01238C6C).
// Shape: the Drink shape (0x00843510) with the player kept in ebx for the first pointer member: edi is then free and
//   holds the zero for the two long pushes, the CSoundPair constructor and the 0.0f speed (`push edi`).
struct CThingCreatureBase { };
struct CThingPlayerCreature : CThingCreatureBase { };
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
struct CIntelligentPointer : CBaseIntelligentPointer {   // one vtable per instantiation
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation,
                        const CSoundPair& sound, long c, long d);                 // 0x00693B30
};

struct CCreatureAction_LearnExpression : CCreatureActionBase {   // vtable 0x0127461C
    CIntelligentPointer<CThingPlayerCreature> PPlayerCreature;  // +0xa8 (PDB +0xb4), vtable 0x01231C4C
    CIntelligentPointer<CThing> PLearnFromThing;                // +0xb0 (PDB +0xbc), vtable 0x01238C6C
    virtual void v00();
    CCreatureAction_LearnExpression(CThingPlayerCreature* player, CThing* learnFrom);
};

CCreatureAction_LearnExpression::CCreatureAction_LearnExpression(CThingPlayerCreature* player, CThing* learnFrom)
    : CCreatureActionBase(player, -1.0f, 0.0f, GetRandomLoadedAnim(player, "STANDARD_CAST"), CSoundPair(), 0, 0)
    , PPlayerCreature(player)
    , PLearnFromThing(learnFrom)
{
}
