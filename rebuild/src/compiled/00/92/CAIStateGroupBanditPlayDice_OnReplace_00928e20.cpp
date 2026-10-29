//   CTCDiceGame::GetCurrentPlayer() (`return CurrentPlayer.GetPItem();`) compared as `DiceGame->GetCurrentPlayer() ==
//   GetCreature()`: the extra inline level gives DiceGame its own temp (eax, `lea ecx,[eax+10h]`) and lets the creature load
//   (edi) schedule ahead of the call, with the later action/IsFree block keeping retail's colouring. Direct
//   `CurrentPlayer.GetPItem()` in either operand order, operator== forms, or a creature local all give 503/505 DIFFERs.
// CAIStateGroup_BanditPlayDice::OnReplace() -- leave the dice game (free the position, drop the player count, clear the current player if it is us, post the watching-out animation), re-sheathe, then re-roll the join/initiate frames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00928E20, length 505 bytes.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C), CTCDiceGame::SetCurrentPlayer @ 0x00929550 (manifest
//          "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//          CCreatureAction_PlayAnimation @ 0x008424F0, CBrainOutput @ 0x00835620, IsFreeToPerformAction @ 0x00662210,
//          CCountedPointer::Reset @ 0x00668D90, SetBrainOutput @ 0x0088B8F0, ~CCreatureActionBase @ 0x00693EF0,
//          UnsheatheDefaultWeapon @ 0x00664A50, GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CWorld::GetFrame
//          @ 0x0049D870, __ftol2 @ 0x00BFEA70.
// Layout: PDB CAIStateGroup_BanditPlayDice: DiceGamePosition +0x14 (CTCExclusiveUse*, User +0xc), DiceGame +0x18 (CTCDiceGame*,
//          NumberOfPlayers +0xc, CurrentPlayer +0x10), FrameToInitiate +0x1c, FrameToJoin +0x20, WasCarryingWeapon +0x24.
class CTCBase;
class CThing;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCreatureActionBase;
struct CCreatureActionQueue {
    long GetCurrentActionID();                  // 0x00692620
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad4c[0x100 - 0x4c];
    CCreatureActionQueue* PActionQueue;         // +0x100
    void UnsheatheDefaultWeapon();              // 0x00664A50
    bool IsPerformingAnyAction();               // 0x006622A0
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {    // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
struct CCountBlock {
    long Count;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
struct CCountedPointer {
    void* Object;
    CCountBlock* Block;
    void Reset(void* object);                   // 0x00668D90
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};
class CBrainOutput { public:                           // 0x48 bytes
    unsigned long Flags;
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                    // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;            // +0x40
    CBrainOutput();                             // 0x00835620
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Clone() const;                // slot 11 (+0x2c)
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
class CTCExclusiveUse { public:
    unsigned char _pad[0xc];
    CIntelligentPointer<CThing> User;              // +0x0c
    void SetUser(CThing* user) { User = CIntelligentPointer<CThing>(user); }
};
class CTCDiceGame { public:
    unsigned char _pad[0xc];
    long NumberOfPlayers;                       // +0x0c
    CIntelligentPointer<CThing> CurrentPlayer;     // +0x10
    void SetCurrentPlayer(CThing* player);      // 0x00929550
    CThing* GetCurrentPlayer() const { return CurrentPlayer.GetPItem(); }
};
class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
};
struct CAIStateGroup_BanditPlayDice : CAIStateGroupBase {
    CTCExclusiveUse* DiceGamePosition;          // +0x14
    CTCDiceGame* DiceGame;                      // +0x18
    long FrameToInitiate;                       // +0x1c
    long FrameToJoin;                           // +0x20
    bool WasCarryingWeapon;                     // +0x24
    void OnReplace();
};

void CAIStateGroup_BanditPlayDice::OnReplace()
{
    if (DiceGame) {
        DiceGamePosition->SetUser(0);
        --DiceGame->NumberOfPlayers;
        if (DiceGame->GetCurrentPlayer() == GetCreature())
            DiceGame->SetCurrentPlayer(0);
        DiceGame = 0;
        CCreatureAction_PlayAnimation action(GetCreature(), "ST_DICEGAME_WATCHING_OUTOF", CSoundPair(), false, true, false, 0, 0);
        CBrainOutput output;
        if (GetCreature()->IsFreeToPerformAction(&action)) {
            output.Flags |= 8;
            CCreatureActionBase* performed = &action;
            output.PAction.Reset(performed->Clone());
            PBrain->SetBrainOutput(output);
        }
    }
    if (WasCarryingWeapon)
        GetCreature()->UnsheatheDefaultWeapon();
    FrameToJoin = (long)(CWorld::GetFrame() + (WorldRandomRange(*GetPWorldSeed(), 750) + 2250) * 0.5f);
    FrameToInitiate = CWorld::GetFrame() + WorldRandomRange(*GetPWorldSeed(), 750) + 2250;
}