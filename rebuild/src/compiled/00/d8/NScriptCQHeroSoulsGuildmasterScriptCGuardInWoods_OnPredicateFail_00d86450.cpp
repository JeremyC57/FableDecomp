// NScript::CQ_HeroSoulsGuildmasterScript::CGuardInWoods::OnPredicateFail() -- when the entity was killed (by anyone), decrement the parent quest's GuardInWoodsCount
// Leaf: ?OnPredicateFail@CGuardInWoods@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d86450, 60 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): slot 5 (OnPredicateFail, as the landed CV_SickChildScript::CWomanToAttract::OnPredicateFail 0x00ECD390) of the entity vtable 0x012D4BB8 that NScript::CQ_HeroSoulsGuildmasterScript::CGuardInWoods::Alloc 0x00D86390 (landed) stores; it decrements ParentClass+0x50: Ego_r.pdb CQ_HeroSoulsGuildmasterScript::GuardInWoodsCount (PDB +0x64, retail +0x50 as the landed CGuardInWoods::Main 0x00D864B0 places it).
// Callees: CScriptThing slot 18 MsgIsKilledBy(const CCharString&) (+0x48, 0x004AAAF0) on the entity's own thing (+8);
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0. String "" 0x0122D70E.
// Shape (family L137): the bool result is kept in bl across the temporary's destructor.
// PDB: Ego_r.pdb / FableWin.pdb spell it ?OnPredicateFail@CGuardInWoods@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ (private virtual).
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
class CQ_HeroSoulsGuildmasterScript { public:
    unsigned char _pad[0x50];
    long GuardInWoodsCount;                          // +0x50
    struct CGuardInWoods {                       // vtable 0x012D4BB8
        CGameScriptInterface* Game;                // +0x04
        CScriptThing          Me;                  // +0x08
        CQ_HeroSoulsGuildmasterScript* ParentClass;     // +0x14
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
void CQ_HeroSoulsGuildmasterScript::CGuardInWoods::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
        --ParentClass->GuardInWoodsCount;
}
}