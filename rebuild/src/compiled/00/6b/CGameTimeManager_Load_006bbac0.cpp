// CGameTimeManager::Load(CPersistContext& context) -- transfer the three saved state members with zero defaults.
// Leaf: ?Load@CGameTimeManager@@QAEXAAVCPersistContext@@@Z
// Identity: proven; PDB exact Load(CPersistContext&); CWorld::LoadGameStateInternal0x4A21F0 calls0x4A2A60 and string-load0x6BBBA0 calls0x6BBBF1; own literals Time/TimeStep/DayCount and member offsets+8/+16/+20.
class CPersistContext { public: template<class T> void Transfer(const char*,T&,const T&); };
class CWorld; class CMainGameComponent;
class CGameTimeManager { public: CWorld* World; CMainGameComponent* GameComponent; float Time,PrevTime,TimeStep; long DayCount; void Load(CPersistContext&); };
void CGameTimeManager::Load(CPersistContext& context) { context.Transfer("Time",Time,0.0f); context.Transfer("TimeStep",TimeStep,0.0f); context.Transfer("DayCount",DayCount,0L); }