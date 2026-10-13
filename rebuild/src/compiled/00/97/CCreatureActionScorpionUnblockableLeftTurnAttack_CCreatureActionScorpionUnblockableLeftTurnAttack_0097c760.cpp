// CCreatureAction_ScorpionUnblockableLeftTurnAttack::CCreatureAction_ScorpionUnblockableLeftTurnAttack(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, thing, 150, GetSoundPair(owner, SWORD_SWING)), then the name SCORPION_STRIKE_LEFT_TURN through the setter @ 0x008574D0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097C760, oracle length 124 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  sub esp,0Ch ; push esi ; push edi ; push -1 ; mov esi,ecx ; push offset "SWORD_SWING" ; lea ecx,[esp+10h] ; call CCharString::CCharString ; mov edi,[owner] ; lea eax,[esp+8] ; push eax ; mov edx,edi ; lea ecx,[esp+10h] ; call GetSoundPair ; mov ecx,[thing] ; push eax ; push 0x96 ; push ecx ; push edi ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase ; lea ecx,[esp+8] ; call CCharString::~CCharString ; push -1 ; push offset lit2 ; lea ecx,[esp+20h] ; mov dword ptr [esi],offset vftable ; call CCharString::CCharString ; lea edx,[esp+18h] ; push edx ; mov ecx,esi ; call setter ; lea ecx,[esp+18h] ; call CCharString::~CCharString ; pop edi ; mov eax,esi ; pop esi ; add esp,0Ch ; ret 8
// Two-argument combat-action constructor whose sound pair comes from the __fastcall
// GetSoundPair @ 0x008561A0: the name literal converts implicitly to a CCharString temporary
// (re-addressed with `lea`, GOTCHAS), the 8-byte CSoundPair result lands in a frame temporary
// through the hidden pointer in ecx and is handed by reference to the base constructor
// @ 0x00857F30 with the action code 150 (0x96); the string temporary dies after the base
// constructor, then this class's vtable (0x01298A04, rtti-vtables.tsv) is stored and a scoped CCharString passed to the setter @ 0x008574D0 (its manifest label is propagated).
// One of 10 bodies of this shape.
struct CThingCreatureBase;
struct CThing;

struct CCharString {                     // one pointer; the default length makes the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
};

struct CSoundPair {                       // a user constructor makes the 8-byte result return through the hidden pointer (a POD would ride in edx:eax)
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

struct CCombatActionBase {
    virtual void v00();
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_ScorpionUnblockableLeftTurnAttack : CCombatActionBase {
    virtual void v00();
    CCreatureAction_ScorpionUnblockableLeftTurnAttack(CThingCreatureBase* owner, CThing* thing);
};

CCreatureAction_ScorpionUnblockableLeftTurnAttack::CCreatureAction_ScorpionUnblockableLeftTurnAttack(CThingCreatureBase* owner, CThing* thing)
    : CCombatActionBase(owner, thing, 150, GetSoundPair(owner, "SWORD_SWING"))
{
    {
        CCharString name("SCORPION_STRIKE_LEFT_TURN", -1);      // scoped named local (a bare temporary would reuse eax, GOTCHAS)
        SetName(name);                     // 0x008574D0
    }
}