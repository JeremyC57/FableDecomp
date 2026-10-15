// CCreatureAction_DragonFireBeamInto::CCreatureAction_DragonFireBeamInto(CThingCreatureBase*, CThing*, float) -- combat action on a target with the SWORD_SWING sound pair (action code 250); the beam timer comes from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009535B0, length 97 bytes, `ret 0Ch` (three arguments).
// Identity: the stored vtable 0x01293E54 is CCreatureAction_DragonFireBeamInto (rtti-vtables.tsv); the catalog label
//   "CCreatureAction_CastStrengthMultiStrike" is propagated. Ego_r.pdb gives the class one float, Timer (PDB +0x12c,
//   retail +0x114: the retail CCombatActionBase is 0x114 bytes, the PDB one 0x12c). No engine header exists for it.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair (__fastcall) @ 0x008561A0,
//          CCombatActionBase::CCombatActionBase(owner, target, actionCode, const CSoundPair&) @ 0x00857F30,
//          CCharString::~CCharString @ 0x0099EAE0.
// Shape: the "SWORD_SWING" literal converts implicitly to a CCharString temporary (re-addressed with `lea`), the
//   CSoundPair (user constructor -> returned through the hidden pointer in ecx) lands in a frame temporary that is
//   passed by reference to the base constructor; the string dies after the base call, then the vtable store and the
//   Timer initialiser (a float copied through edx from the third argument slot).
struct CThingCreatureBase;
struct CThing;

struct CCharString {                        // one pointer; the defaulted length makes a literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CSoundPair {                         // user constructor: the 8-byte result comes back through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

struct CCreatureAction_DragonFireBeamInto : CCombatActionBase {   // vtable 0x01293E54
    float Timer;                            // +0x114 (PDB +0x12c)
    virtual void v00();
    CCreatureAction_DragonFireBeamInto(CThingCreatureBase* owner, CThing* target, float timer);
};

CCreatureAction_DragonFireBeamInto::CCreatureAction_DragonFireBeamInto(CThingCreatureBase* owner, CThing* target, float timer)
    : CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING"))
    , Timer(timer)
{
}