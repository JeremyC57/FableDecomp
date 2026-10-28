// NScript::CV_BordelloScript::CBordelloClient::OnPersist(CPersistContext& context) -- transfer the three saved state members with zero defaults.
// Leaf: ?OnPersist@CBordelloClient@CV_BordelloScript@NScript@@EAEXAAVCPersistContext@@@Z
// Identity: proven; vtable slot4 at0x12E3B58; PDB nested CBordelloClient unique member triple BrainState+28 ClientID+32 SexChance+36; own three literals agree.
class CPersistContext { public: template<class T> void Transfer(const char*,T&,const T&); };
class CGameScriptInterfaceBase; class CScriptThing;
class CEntityScriptBase { public: virtual void OnPersist(CPersistContext&); CGameScriptInterfaceBase* Game; unsigned char Me[8]; unsigned char _base10[4]; };
namespace NScript { class CQ_SunnyvaleMasterData; class CV_BordelloScript { public: class CBordelloClient : public CEntityScriptBase { private: virtual void OnPersist(CPersistContext&); public: CV_BordelloScript* ParentClass; CQ_SunnyvaleMasterData* PMasterData; long BrainState,ClientID,SexChance,NextLine; void InvokePersist(CPersistContext& c) { OnPersist(c); } }; }; }
void NScript::CV_BordelloScript::CBordelloClient::OnPersist(CPersistContext& context) { context.Transfer("BrainState",BrainState,0L); context.Transfer("ClientID",ClientID,0L); context.Transfer("SexChance",SexChance,0L); }