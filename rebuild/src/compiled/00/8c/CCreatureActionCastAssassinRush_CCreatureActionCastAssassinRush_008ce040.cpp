// CCreatureAction_CastAssassinRush::CCreatureAction_CastAssassinRush(CThingCreatureBase*, CThing*, bool, long, CCountedPointer<CCreatureActionBase>) -- targeted form: base CCombatActionBase(owner, target, 150, zeroed sound), Override clear, Level, FinalAction, IsZTargetting; animation "ST_CAST_ASSASSIN_RUSH"
// Leaf: ??0CCreatureAction_CastAssassinRush@@QAE@PAVCThingCreatureBase@@PAVCThing@@_NJUCCountedPointer_CCreatureActionBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CE040, length 179 bytes (+ int3 padding), `ret 18h`. Vtable 0x01284254.
// Callees: the CCombatActionBase constructor (0x00858030 owner form / 0x00857F30 owner+target form), CCharString ctor 0x0099EBF0 /
// dtor 0x0099EAE0 ("ST_CAST_ASSASSIN_RUSH" @ 0x01284238), SetAnimationName 0x008574D0, operator delete 0x00BFE9BC (the by-value
// FinalAction's count block when its last reference goes).
// Shape: init-list members in declaration order (Override, Dir, Distance, Level, FinalAction copy -- `inc [Info]` when non-null --,
// UseCollision, IsZTargetting); the scoped named animation string; the by-value CCountedPointer<CCreatureActionBase> parameter is
// destroyed by the callee at the end (dec, destroy through the block's pointer-to-member, delete the block).
// Layout: byte-packed like the landed copy constructor 0x008CEF50 (PDB header + 0x18 = retail offsets in the comments).
struct CThingCreatureBase;
struct CThing;
struct CCreatureActionBase;
void __cdecl operator delete(void* block);  // 0x00BFE9BC

struct CCountedItem { };
struct CCPPointerInfo {                     // the shared count block
    long m_count;                           // +0x00
    void (CCountedItem::*m_pfnDestroy)();   // +0x04 (called with the item as `this`)
    CCountedItem* m_pItem;                  // +0x08
};

struct CCountedPointer_CCreatureActionBase {    // CCountedPointer<CCreatureActionBase>
    CCreatureActionBase* Data;
    CCPPointerInfo*      Info;
    CCountedPointer_CCreatureActionBase(const CCountedPointer_CCreatureActionBase& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
    ~CCountedPointer_CCreatureActionBase()
    {
        if (Info) {
            if (--Info->m_count == 0) {
                (Info->m_pItem->*Info->m_pfnDestroy)();
                operator delete(Info);
            }
        }
    }
};

struct C3DVector {                          // three floats; the memberwise user copy constructor copies each as a dword
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
    void SetAnimationName(const CCharString& name);                                                             // 0x008574D0
};

#pragma pack(push, 1)
struct CCreatureAction_CastAssassinRush : CCombatActionBase {   // vtable 0x01284254; retail sizeof 0x138
    bool          Override;                 // +0x114 (PDB +0x12c)
    C3DVector     Dir;                      // +0x115 (PDB +0x12d), unaligned
    unsigned char _pad121[3];               // +0x121
    float         Distance;                 // +0x124 (PDB +0x13c)
    long          Level;                    // +0x128 (PDB +0x140)
    CCountedPointer_CCreatureActionBase FinalAction;   // +0x12c (PDB +0x144)
    bool          UseCollision;             // +0x134 (PDB +0x14c)
    bool          IsZTargetting;            // +0x135 (PDB +0x14d)
    unsigned char _pad136[2];               // +0x136
    virtual void v00();
    CCreatureAction_CastAssassinRush(CThingCreatureBase* owner, CThing* target, bool isZTargetting, long level, CCountedPointer_CCreatureActionBase finalAction);
    CCreatureAction_CastAssassinRush(CThingCreatureBase* owner, long level, CCountedPointer_CCreatureActionBase finalAction);
    CCreatureAction_CastAssassinRush(CThingCreatureBase* owner, const C3DVector& dir, float distance, bool useCollision, long level, CCountedPointer_CCreatureActionBase finalAction);
};
#pragma pack(pop)

CCreatureAction_CastAssassinRush::CCreatureAction_CastAssassinRush(CThingCreatureBase* owner, CThing* target, bool isZTargetting, long level, CCountedPointer_CCreatureActionBase finalAction)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , Override(false)
    , Level(level)
    , FinalAction(finalAction)
    , IsZTargetting(isZTargetting)
{
    {
        CCharString name("ST_CAST_ASSASSIN_RUSH");
        SetAnimationName(name);
    }
}