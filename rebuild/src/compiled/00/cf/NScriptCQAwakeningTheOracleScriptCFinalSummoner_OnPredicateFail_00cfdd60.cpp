// NScript::CQ_AwakeningTheOracleScript::CFinalSummoner::OnPredicateFail() -- when the entity was killed (by anyone), decrement the parent quest's FinalSummonersAlive
// Leaf: ?OnPredicateFail@CFinalSummoner@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00cfdd60, 60 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): slot 5 (OnPredicateFail, as the landed CV_SickChildScript::CWomanToAttract::OnPredicateFail 0x00ECD390) of the entity vtable 0x012C7D84 that NScript::CQ_AwakeningTheOracleScript::CFinalSummoner::Alloc 0x00CFDCA0 (landed) stores; it decrements ParentClass+0x60: Ego_r.pdb CQ_AwakeningTheOracleScript::FinalSummonersAlive (PDB +0x74, retail +0x60 with the entity-script base 0x14 smaller).
// Callees: CScriptThing slot 18 MsgIsKilledBy(const CCharString&) (+0x48, 0x004AAAF0) on the entity's own thing (+8);
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0. String "" 0x0122D70E.
// Shape (family L137): the bool result is kept in bl across the temporary's destructor.
// PDB: Ego_r.pdb / FableWin.pdb spell it ?OnPredicateFail@CFinalSummoner@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ (private virtual).
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
class CQ_AwakeningTheOracleScript { public:
    unsigned char _pad[0x60];
    long FinalSummonersAlive;                          // +0x60
    struct CFinalSummoner {                       // vtable 0x012C7D84
        CGameScriptInterface* Game;                // +0x04
        CScriptThing          Me;                  // +0x08
        CQ_AwakeningTheOracleScript* ParentClass;     // +0x14
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
void CQ_AwakeningTheOracleScript::CFinalSummoner::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
        --ParentClass->FinalSummonersAlive;
}
}