// CAIStateGroup_HoverHornetQueen::Spawn() -- play the shoot-babies action at the tracked thing, then register every live CREATURE_HORNET_PICNIC creature within 60 units as the boss's helper (copying the queen's level-persistent state onto it); terminate when no longer runnable
// Grade: DIFFER(445v445) -- two scheduling/register gaps only: the filter's index store lands after the PCreature load (retail between the PBrain and PCreature loads), and the 0xad GetInterface keeps the creature in eax and builds the map with lea edi,[eax+0x44] (retail edi, then add edi,0x44); frame, slots, loop caching and everything else line up
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095B820, length 445 bytes (+ int3 padding).
// Callees: CAIStateGroup_HoverHornetQueen::IsStillRunnable @ 0x0095A420 (twice), CAIStateGroupBase::Terminate @ 0x008FCF40,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (+0x14), CCreatureAction_QueenHornetShootBabies ctor @ 0x0095CB80 / the
// base ~CCombatActionBase @ 0x0062E900, AIAction_PerformActionAndWait @ 0x0097DB70, CCharString(const char*, long) @ 0x0099EBF0 /
// ~CCharString @ 0x0099EAE0 ("CREATURE_HORNET_PICNIC" @ 0x01295300), CGameDefinitionManager::Get @ 0x0044C6B0 (static
// __fastcall), CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CThing::GetPosition @ 0x004C73D0, __ftol2
// @ 0x00BFEA70, PeekThingSearchTools @ 0x008FD000, the rough-area collector @ 0x008E3EB0 (box, def-index filter, output
// vector; the manifest "GetPBestThingInRoughArea<...IsVeryCloseMotionlessCreature...>" is a BSim guess),
// CThing::IsLevelPersistent @ 0x004C82A0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xad CTCBoss = bit 13 of
// +0x34), CTCBoss::SetHelperCreature @ 0x007AF310, the CRT free @ 0x00BFEA14 (the local vector's storage).
// Layout: the tracked thing at +0x14 (the group's first pointer); CThing flag bytes +0x92 (bit 4: level persistent, written per
// helper) and +0x93 (bit 2: KillOnLevelUnload, tested before IsLevelPersistent) are retail offsets (Ego_r.pdb +0x10 higher, names from the PDB).
struct C3DVector {
    float X, Y, Z;
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    CTCBase* Find(long interfaceType)
    {
        long type = interfaceType;
        SEntry* it = LowerBound(&type);
        if (it == last || interfaceType < it->type)
            it = last;
        return it->component;
    }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;          // +0x92 bit 4
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1; // +0x93 bit 0
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;        // +0x93 bit 2
    unsigned char m_flags93_3 : 5;
    const C3DVector& GetPosition();             // 0x004C73D0
    bool IsLevelPersistent();                   // 0x004C82A0
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            *ppOut = (T*)m_interfaces.Find(interfaceType);
        }
    }
};
struct CThingCreatureBase;
struct CThingAICreature;
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) { }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
struct CThingFilter_HasDefGlobalIndex {
    long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(long index) : DefGlobalIndex(index) { }
};
extern "C" void __cdecl free(void* block);    // 0x00BFEA14
struct CThingPtrVector {                        // STLport vector<CThing*> (12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingPtrVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingPtrVector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CThing*& operator[](unsigned int n) { return _M_start[n]; }
};
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CThingPtrVector& out) const;   // 0x008E3EB0
};
long CollectSink_95b820(const void* self, long seen, CThing*** slots);   // stand-in: the area walk the collector does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for the rough-area collector @ 0x008E3EB0 (a template instantiated in retail's TU): it reads its const
// reference arguments and fills the output vector, and lets no argument escape.
__declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CThingPtrVector& out) const
{
    out._M_finish = out._M_start + CollectSink_95b820(this, area.MinX + filter.DefGlobalIndex, 0);
}
#pragma auto_inline(on)
struct CTCBoss {                                // interface 0xad
    void SetHelperCreature(CThingAICreature* helper);   // 0x007AF310
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x114 - 4];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_QueenHornetShootBabies : CCombatActionBase {
    CCreatureAction_QueenHornetShootBabies(CThingCreatureBase* owner, CThing* thing);   // 0x0095CB80
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14 (descriptive)
    bool IsStillRunnable();                     // 0x0095A420
    void Spawn();
};

#define IS_THING_ALIVE(p) ((p) != 0 && !((p)->m_flags91 & 1))

void CAIStateGroup_HoverHornetQueen::Spawn()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    AIAction_PerformActionAndWait(&CCreatureAction_QueenHornetShootBabies((CThingCreatureBase*)GetCreature(), PTarget.GetPItem()), false);
    CThingPtrVector helpers;
    {
        long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_HORNET_PICNIC");
        CThingFilter_HasDefGlobalIndex filter(index);
        const C3DVector& pos = GetCreature()->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        PeekThingSearchTools()->GetAllThingsInRoughArea(C2DBoxI(x - 60, y - 60, x + 60, y + 60), filter, helpers);
    }
    bool persistent = false;
    CThing* me = GetCreature();
    if (!me->KillOnLevelUnload || me->IsLevelPersistent())
        persistent = true;
    CTCBoss* boss;
    GetCreature()->GetInterface(&boss, 0xad);
    unsigned int count = helpers.size();
    for (unsigned int i = 0; i < count; ++i) {
        CThing* helper = helpers[i];
        if (IS_THING_ALIVE(helper)) {
            helper->LevelPersistent = persistent;
            boss->SetHelperCreature((CThingAICreature*)helpers[i]);
        }
    }
    if (!IsStillRunnable())
        Terminate();
}
