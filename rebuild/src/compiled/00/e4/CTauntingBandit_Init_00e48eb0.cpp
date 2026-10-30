// CTauntingBandit::Init() -- reset the anger state, put Me in the friendly-bandit faction and make Me wander around the bandit camp centre marker between 0 and the script def's toll-bandit wander distance
// Leaf: ?Init@CTauntingBandit@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E48EB0, length 356 bytes (plain `ret`; entity-script slot "Init").
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, ~CBaseClass @ 0x0099A2E0, operator
//   delete @ 0x00BFE9BC (the counted-pointer info block), and the game interface slots (ghidra_out/cgsi_slots_aligned.tsv)
//   72 GetThingWithScriptName, 601 EntitySetInFaction, 763 SetWanderCentrePoint(CScriptThing, C3DVector), 766
//   SetWanderMinDistance(CScriptThing, float), 769 SetWanderMaxDistance(CScriptThing, float); CScriptThing slot 6 GetPos
//   (0x004AA980, rtti-vtables.tsv).
// Data: "FACTION_BANDITS_FRIENDLY" @ 0x012D218C, "M_BanditCampCentrePos" @ 0x012E4DF0, pGlobalScriptDef @ 0x0143E90C whose
//   +0x428 long is BanditTollBanditWanderDistance (PDB +0x45C; the retail def is 0x34 lower here).
// Layout: Ego_r.pdb CTauntingBandit (0x24): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18,
//   AngerCount +0x1C, AngryWithHero +0x20 (unshifted).
// Shape: the by-value CScriptThing arguments are copy-built in their stack slots (inline CBaseClass ctor: both vtable
//   stores; null-tested add-ref); the marker temporary dies after the centre-point call (vtable store, inline release,
//   cleared words, ~CBaseClass); the 0.0f minimum is pushed from the zero register.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

struct CScriptThingImpl;
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImpl> PImp;              // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
    ~CScriptThing() {}
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18) 0x004AA980
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad000[0x428];
    long BanditTollBanditWanderDistance;                 // +0x428 (PDB +0x45C)
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

class CTauntingBandit {
public:
    void*                 __vftable;                     // +0x00
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
    void*                 ParentClass;                   // +0x14 CV_BanditTollScript*
    void*                 PMasterData;                   // +0x18 CQ_SunnyvaleMasterData*
    long                  AngerCount;                    // +0x1c
    bool                  AngryWithHero;                 // +0x20
    void Init();
};

void CTauntingBandit::Init()
{
    AngerCount = 0;
    AngryWithHero = false;
    Game->EntitySetInFaction(Me, "FACTION_BANDITS_FRIENDLY");
    Game->SetWanderCentrePoint(Me, Game->GetThingWithScriptName("M_BanditCampCentrePos").GetPos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, (float)pGlobalScriptDef->BanditTollBanditWanderDistance);
}