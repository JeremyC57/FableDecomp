// NScript::CQ_HeroSoulsNostroScript::CSummonerToKill::OnPredicateFail() -- when the entity was killed (by anyone), decrement the parent quest's SummonerCount
// Leaf: ?OnPredicateFail@CSummonerToKill@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d884e0, 60 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): slot 5 (OnPredicateFail, as the landed CV_SickChildScript::CWomanToAttract::OnPredicateFail 0x00ECD390) of the entity vtable 0x012D4E8C that NScript::CQ_HeroSoulsNostroScript::CSummonerToKill::Alloc 0x00D88420 (landed) stores; it decrements ParentClass+0x48: Ego_r.pdb CQ_HeroSoulsNostroScript::SummonerCount (PDB +0x5C, retail +0x48).
// Callees: CScriptThing slot 18 MsgIsKilledBy(const CCharString&) (+0x48, 0x004AAAF0) on the entity's own thing (+8);
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0. String "" 0x0122D70E.
// Shape (family L137): the bool result is kept in bl across the temporary's destructor.
// PDB: Ego_r.pdb / FableWin.pdb spell it ?OnPredicateFail@CSummonerToKill@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ (private virtual).
// Layout (as the landed 0x00ECD390): interface +4, Me +8, ParentClass +0x14, PMasterData +0x18.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CScriptThingMethods {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
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
};
class CScriptThing : public CScriptThingMethods { public:        // 12 bytes, vtable 0x01238C8C
    void* PImp_Data;
    void* PImp_Info;
};
class CGameScriptInterface;
namespace NScript {
class CQ_HeroSoulsNostroScript { public:
    unsigned char _pad[0x48];
    long SummonerCount;                          // +0x48
    struct CSummonerToKill {                       // vtable 0x012D4E8C
        CGameScriptInterface* Game;                // +0x04
        CScriptThing          Me;                  // +0x08
        CQ_HeroSoulsNostroScript* ParentClass;     // +0x14
        void*                 PMasterData;         // +0x18
        virtual void v0();
        virtual void Main();
        virtual void v2();
        virtual void v3();
        virtual void v4();
    private:
        virtual void OnPredicateFail();            // slot 5 (Ego_r.pdb: private virtual, EAE)
    };
};
void CQ_HeroSoulsNostroScript::CSummonerToKill::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
        --ParentClass->SummonerCount;
}
}