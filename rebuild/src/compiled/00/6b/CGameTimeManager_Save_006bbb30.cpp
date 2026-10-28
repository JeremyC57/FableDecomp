// CGameTimeManager::Save(CPersistContext& context) -- transfer the three saved state members with zero defaults.
// Leaf: ?Save@CGameTimeManager@@QAEXAAVCPersistContext@@@Z
// Identity: proven; PDB exact Save(CPersistContext&); CWorld::SaveGameStateInternal0x49F4C0 calls0x49FA74; own literals Time/TimeStep/DayCount and member offsets+8/+16/+20.
class CPersistContext { public: template<class T> void Transfer(const char*,T&,const T&); };
class CWorld; class CMainGameComponent;
class CGameTimeManager { public: CWorld* World; CMainGameComponent* GameComponent; float Time,PrevTime,TimeStep; long DayCount; void Save(CPersistContext&); };
void CGameTimeManager::Save(CPersistContext& context) { context.Transfer("Time",Time,0.0f); context.Transfer("TimeStep",TimeStep,0.0f); context.Transfer("DayCount",DayCount,0L); }