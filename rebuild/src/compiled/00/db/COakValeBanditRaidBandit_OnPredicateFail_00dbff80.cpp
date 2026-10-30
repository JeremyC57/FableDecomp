// COakValeBanditRaidBandit::OnPredicateFail() -- when the bandit is killed: flag the raid's bandit-killed byte, and unless the raid is over, re-target the raid's bandit thing to the one nearest the hero and flag it when that bandit is within 8 m of this one
// Leaf: OnPredicateFail
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DBFF80, 259 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CScriptThing slot 18 MsgIsKilledBy(const CCharString&) (+0x48) and slot 6 GetPos() (+0x18, const C3DVector&)
//   on the entity's own thing (+8); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CGameScriptInterface
//   (ghidra_out/cgsi_slots_aligned.tsv) GetHero 70 (+0x118) 0x00891CA0 and GetNearestWithScriptName 77 (+0x134) 0x0089A510
//   (CScriptThing by value); the CScriptThing temporary's destructor ~CScriptThing 0x004AA840; operator delete 0x00BFE9BC
//   (the inline counted-pointer assignment); __fastcall IsDistanceFromThingToPositionUnder(const CScriptThing&, const
//   C3DVector&, float) 0x00CBE4B7.
// Layout: entity-script base as the landed COakValeBanditRaidBandit::Init 0x00DBFEA0 (Game +4, Me +8, ParentClass +0x14);
//   the raid script (retail): RaidOver +0x48, BanditKilled +0x61, BanditInRange +0x62, Bandit (CScriptThing) +0x64.
// Shape: the CScriptThing assignment is the inline CCountedPointer operator= (source words read first); the name string
//   is the implicit conversion temporary built before the nested GetHero call.
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CScriptThingImp;
struct CScriptThingPImp {                          // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    SCountBlock*     Info;
    __forceinline CScriptThingPImp& operator=(const CScriptThingPImp& other)
    {
        SCountBlock* info = other.Info;
        CScriptThingImp* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};
class CScriptThing { public:                              // 12 bytes, vtable 0x01238C8C
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18) 0x004AA980
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    CScriptThingPImp PImp;                         // +0x04
    virtual ~CScriptThing();                               // 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
};
bool __fastcall IsDistanceFromThingToPositionUnder(const CScriptThing& thing, const C3DVector& position, float distance);   // 0x00CBE4B7
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_OakValeBanditRaidScript {
    unsigned char _pad00[0x48];
    bool          RaidOver;                        // +0x48
    unsigned char _pad49[0x61 - 0x49];
    bool          BanditKilled;                    // +0x61
    bool          BanditInRange;                   // +0x62
    CScriptThing  Bandit;                          // +0x64
};
struct CQ_SunnyvaleMasterData;
struct COakValeBanditRaidBandit {
    virtual void v00();
    CGameScriptInterface*       Game;              // +0x04
    CScriptThing                Me;                // +0x08
    CQ_OakValeBanditRaidScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;       // +0x18
    virtual void OnPredicateFail();
};

void COakValeBanditRaidBandit::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        ParentClass->BanditKilled = true;
        if (!ParentClass->RaidOver) {
            ParentClass->Bandit = Game->GetNearestWithScriptName(Game->GetHero(), "OakValeBanditRaidBandit");
            if (IsDistanceFromThingToPositionUnder(ParentClass->Bandit, Me.GetPos(), 8.0f))
                ParentClass->BanditInRange = true;
        }
    }
}