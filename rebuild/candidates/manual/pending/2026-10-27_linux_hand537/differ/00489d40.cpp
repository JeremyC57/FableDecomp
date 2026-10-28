// CPlayer::CreateCharacter(const CCharString& defName) -- find the character's start holy site (or use the debug start position), place a co-op player one unit off the main player's character, set the world map's player position, create the player creature from the named (or the player def's) character def, set its orientation, and take it as the player character
// Grade: DIFFER(815v815) -- frame slots only (registers now agree: holy site esi, player number edi): ours homes pos at [esp+1Ch] and forward at [esp+28h] (retail forward 1Ch / pos 28h); the holy-site out-local shares the up vector's slot (retail shares the name temp's slot at +0Ch) and the offset temporary shares forward (retail up)
// Leaf: ?CreateCharacter@CPlayer@@AAE_NABVCCharString@@@Z
// Tried: about 30 probes (ctor/member/named rh forms, scope blocks, declaration orders, named number locals) (2026-10-22); chunk 537 (RTM): real PDB names (private CreateCharacter, LocateCharacterStartHolySite, PeekPos/GetPos, GetMainPlayerNumber / GetMainPlayer, CInitBaseClass::IsValid, CBaseIntelligentPointer GetPItem/SetPItem, CWorld::GetWorldMap slot 12, CWorldMap::GetMapNumberAt / SetPlayerPos, CTCPhysicsBase ResetRHSet / GetRHSet, CThingPlayerCreature::Create, NProgressDisplay::DisplayProgress, GetDefGlobalIndexFromName, PlayerDef_Object +0x21C) and named `number` / `mainNumber` locals fix the esi/edi pair; the written next step (holy-site out-local in the outer scope, debug vectors in a sibling block), 30 declaration permutations, debug-branch forms and offset-vector forms leave the slots unchanged. | next: lane gap: frame-slot -- VC7.1's slot sharing for the three C3DVector temporaries follows a flow order no tried source form reproduces.
#include <string.h>
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer

#pragma pack(push, 1)
template <class T> class CBasicString { public:
    T*            PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const T* text) const;              // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
    long               NoRefs;
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(char c);              // 0x0099F040
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float fraction, bool a, bool b);   // 0x009E9F40
}

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
};
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() {}
};

#define V8(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3(); virtual void n##4(); virtual void n##5(); virtual void n##6(); virtual void n##7();
class CTCPhysicsBase { public:
    V8(a) V8(b) V8(c) V8(d)
    virtual void ResetRHSet(const CRightHandedSet& rh);    // slot 32 (+0x80)
    virtual void e1(); virtual void e2(); virtual void e3(); virtual void e4(); virtual void e5(); virtual void e6(); virtual void e7();
    V8(f) V8(g) V8(h) V8(i)
    virtual CRightHandedSet GetRHSet(float time) const;    // slot 71 (+0x11C)
    virtual const CRightHandedSet& GetRHSet() const;       // slot 72 (+0x120)
};
class CBaseObject { public: };
class CThing : public CBaseObject { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                         // +0x60 (retail)
    const C3DVector& GetPos() const;                   // 0x004C73D0
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;                  // 0x006A4D00
};
class CThingHolySite : public CThingPhysical { public: };
class CThingPlayerCreatureInit { public:
    CThingPhysical* OriginalThing;
    CThingPhysical* PreviousThing;
    bool            PlaceAtEndOfList;
    CThingPlayerCreatureInit(CThingPhysical* original = 0, CThingPhysical* previous = 0)
        : OriginalThing(original), PreviousThing(previous), PlaceAtEndOfList(false) { }
};
class CThingPlayerCreature : public CThingPhysical { public:
    static CThingPlayerCreature* __fastcall Create(long defIndex, const C3DVector& pos, long playerNumber, const CThingPlayerCreatureInit& init);   // 0x006AC910
};

class CBaseIntelligentPointer { public:
    CBaseObject* PItem;
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
    CBaseObject* GetPItem() const;                     // 0x00A01B50
    void SetPItem(CBaseObject* item);                  // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01231C4C
    CIntelligentPointer(T* item) { SetPItem(item); }
    T* operator->() const { return (T*)GetPItem(); }
};

class CWorldMap { public:
    V8(a) V8(b)
    virtual long GetMapNumberAt(const C3DVector& pos) const;   // slot 16 (+0x40)
    enum ELoadType { LOAD_TYPE_0 };
    void SetPlayerPos(long playerNumber, const C3DVector& pos, ELoadType loadType);   // 0x005063E0
};
class CWorld { public:
    V8(a)
    virtual void b0(); virtual void b1(); virtual void b2(); virtual void b3();
    virtual CWorldMap& GetWorldMap();                  // slot 12 (+0x30)
    unsigned char _pad04[0x174];
    long          PlayerCharacterCreated;              // +0x178
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public: };
class CDefPointeeBase { public: };
class CPlayerDef : public CDefPointeeBase { public:
    unsigned char _pad00[0x3c];
    long          CharacterDef;                        // +0x3c (Ego_r +0x40)
};
class CInitBaseClass { public:
    bool IsValid() const;                              // 0x0099A330
};
class CPlayer;
class CPlayerManager { public:
    long GetMainPlayerNumber() const;                  // 0x00449700
    CPlayer& GetMainPlayer();                          // 0x00449970
};

extern bool      GSetPlayerStartPos;                   // 0x013B8647
extern C3DVector GOverridePlayerStartPos;              // 0x013B8650

class CPlayer : public CPlayerLayout { public:
    CIntelligentPointer<CThingPlayerCreature>& ControlledCreaturePtr() { return *(CIntelligentPointer<CThingPlayerCreature>*)ControlledCreature; }
    CIntelligentPointer<CThingPlayerCreature>& PlayerCharacterPtr() { return *(CIntelligentPointer<CThingPlayerCreature>*)PlayerCharacter; }
    CInitBaseClass& InitBase() { return *(CInitBaseClass*)this; }
private:
    bool LocateCharacterStartHolySite(const CThingHolySite** site) const;   // 0x00488B20
    bool CreateCharacter(const CCharString& defName);
};

bool CPlayer::CreateCharacter(const CCharString& defName)
{
    ControlledCreaturePtr().GetPItem();
    PlayerCharacterPtr().GetPItem();
    const CThingHolySite* site;
    {
        const CThingHolySite* holySite = 0;
        if (!LocateCharacterStartHolySite(&holySite) && !GSetPlayerStartPos)
            return false;
        site = holySite;
    }
    C3DVector pos;
    CRightHandedSet rh;
    if (GSetPlayerStartPos && World->GetWorldMap().GetMapNumberAt(GOverridePlayerStartPos)) {
        pos = GOverridePlayerStartPos;
        C3DVector up(0.0f, 1.0f, 0.0f);
        C3DVector forward(0.0f, 0.0f, -1.0f);
        rh.Up = up;
        rh.Forward = forward;
    } else {
        pos = site->PeekPos();
        rh = site->PhysicsTC->GetRHSet();
    }
    long number = Number;
    if (number != PlayerManager->GetMainPlayerNumber()) {
        if (PlayerManager->GetMainPlayer().InitBase().IsValid() && PlayerManager->GetMainPlayer().ControlledCreaturePtr().GetPItem()) {
            const C3DVector& p = PlayerManager->GetMainPlayer().ControlledCreaturePtr()->GetPos();
            pos = C3DVector(p.X + 1.0f, p.Y + 1.0f, p.Z);
        } else {
            const C3DVector& p = site->PeekPos();
            pos = C3DVector(p.X + 1.0f, p.Y + 1.0f, p.Z);
        }
    }
    World->GetWorldMap().SetPlayerPos(Number, pos, CWorldMap::LOAD_TYPE_0);
    CThingPlayerCreatureInit init;
    CCharString name(defName);
    if (name == "")
        name = ((const CPlayerDef*)PlayerDef_Object)->CharacterDef;
    long defIndex = DefinitionManager->GetDefGlobalIndexFromName(name);
    CThingPlayerCreature* creature = CThingPlayerCreature::Create(defIndex, pos, Number, init);
    NProgressDisplay::DisplayProgress("CPlayer::CreateCharacter", -1.0f, false, false);
    creature->PhysicsTC->ResetRHSet(rh);
    PlayerCharacterPtr().SetPItem(CIntelligentPointer<CThingPlayerCreature>(creature).GetPItem());
    RespawnInfo[0xc] = false;
    long mainNumber = Number;
    if (mainNumber == PlayerManager->GetMainPlayerNumber())
        World->PlayerCharacterCreated = 1;
    return true;
}