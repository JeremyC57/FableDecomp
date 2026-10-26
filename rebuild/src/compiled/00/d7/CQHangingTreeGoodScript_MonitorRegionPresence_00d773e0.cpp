// CQ_HangingTreeGoodScript::MonitorRegionPresence() -- record BowerstoneJail as visited, then for each region change: wait for the party to leave and the new region to load, and if it has a party spawn point flag a spawn there (unless already spawned in that region or the party is not ready) and remember the region, otherwise fail the quest for leaving the main route
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D773E0, length 686 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 / ~CCharString
//   0x0099EAE0 / operator=(const char*) 0x0099EFE0, Compare 0x00411570 (__fastcall; the inline CCharString ==),
//   vector<CCharString>::_M_insert_overflow 0x00433530 (inline push_back) / push_back 0x0044BFF0 (out of line),
//   CScriptThing::IsAlive 0x004AB130, ~CScriptThing 0x004AA840, CScriptBase::IsActiveThreadTerminating 0x00CB7940;
//   through the interface (+0x40): NewScriptFrame (7), IsRegionLoaded (12), MsgOnRegionLoaded (22),
//   GetThingWithScriptName (72).
// Strings: "BowerstoneJail" 0x012C7788, "HTG_PartySpawnPoint" 0x012D2A44, "TEXT_QST_030_FAILURE_LEFT_MAIN_ROUTE"
//   0x012D2EEC.
// Layout (retail = Ego_r.pdb - 0x14, 12-byte vectors): MissionFailed +0x4b, SwitchingRegions +0x4c, SpawnHere +0x4f,
//   PartyReadyToTransition +0x53, CurrentLevel +0x98, ReasonForFailing +0x9c, AreasSpawnedIn +0xc4.
// Shape: CCharString == as the landed CV_ChapelOfEvilScript::WatchForRegionEntry 0x00E6B750 (CBasicString<char> data,
//   Compare); STLport push_back inline (the overflow path out of line).
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const CBasicStringChar& other) const
    {
        if (StringLength != other.StringLength)
            return false;
        return Compare_00411570(PData, other.PData) ? false : true;
    }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (!PStringData || !other.PStringData)
            return false;
        return PStringData->Data == other.PStringData->Data;
    }
};
inline void* operator new(unsigned int, void* p) { return p; }
struct __false_type { __false_type() { } };
struct CCharStringVector {                      // std::vector<CCharString> (12 bytes in retail)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CCharString& operator[](unsigned int n) { return _M_start[n]; }
    template <class Tag> void _M_insert_overflow(CCharString* pos, const CCharString& x, const Tag&, unsigned int fill_len, bool atend);   // 0x00433530
    void push_back(const CCharString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
// Same-TU body of vector<CCharString>::_M_insert_overflow 0x00433530 (tag unused).
template <class Tag> __declspec(noinline) void CCharStringVector::_M_insert_overflow(CCharString* pos, const CCharString& x, const Tag&,
                                                                                    unsigned int fill_len, bool atend)
{
    unsigned int oldSize = size();
    unsigned int len = oldSize + (oldSize < fill_len ? fill_len : oldSize);
    CCharString* newStart = len ? (CCharString*)malloc(len * sizeof(CCharString)) : 0;
    CCharString* newFinish = newStart;
    CCharString* p;
    for (p = _M_start; p != pos; ++p, ++newFinish)
        new (newFinish) CCharString(*p);
    for (; fill_len != 0; --fill_len, ++newFinish)
        new (newFinish) CCharString(x);
    if (!atend)
        for (p = pos; p != _M_finish; ++p, ++newFinish)
            new (newFinish) CCharString(*p);
    for (p = _M_start; p != _M_finish; ++p)
        p->~CCharString();
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}
class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
    CBaseClass();                                           // 0x0099A2D0
};
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
struct CScriptThingImpl;
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    ~CScriptThing();                                        // 0x004AA840
    bool IsAlive() const;                                   // 0x004AB130 (slot 75, called directly)
};
class CGameScriptInterface { public:                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30) IsRegionLoaded
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021();
    virtual bool MsgOnRegionLoaded(CCharString& region) const;   // slot 22 (+0x58) MsgOnRegionLoaded
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046();
    virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120) GetThingWithScriptName
};
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                        // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
namespace NScript {
class CQ_HangingTreeGoodScript : public CScriptBase { public:
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
    bool LeaderSpokenTo;                                    // +0x48
    bool PartySuccessfullySetUp;                            // +0x49
    bool MissionSucceeded;                                  // +0x4a
    bool MissionFailed;                                     // +0x4b
    bool SwitchingRegions;                                  // +0x4c
    bool ReassignLeaderNow;                                 // +0x4d
    bool PartyDefeated;                                     // +0x4e
    bool SpawnHere;                                         // +0x4f
    bool AtWayPoint;                                        // +0x50
    bool HeroKilledPrisoner;                                // +0x51
    bool QuestStartScreenShown;                             // +0x52
    bool PartyReadyToTransition;                            // +0x53
    unsigned char _pad54[0x98 - 0x54];
    CCharString CurrentLevel;                               // +0x98
    CCharString ReasonForFailing;                           // +0x9c
    unsigned char _pada0[0xc4 - 0xa0];
    CCharStringVector AreasSpawnedIn;                       // +0xc4
    void MonitorRegionPresence();
};
void CQ_HangingTreeGoodScript::MonitorRegionPresence()
{
    bool firstTime = true;
    CurrentLevel = "BowerstoneJail";
    AreasSpawnedIn.push_back(CurrentLevel);
    if (IsActiveThreadTerminating())
        return;
    do {
        while (!Game->IsRegionLoaded(CurrentLevel)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        SwitchingRegions = false;
        while (Game->IsRegionLoaded(CurrentLevel)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        SwitchingRegions = true;
        while (!Game->MsgOnRegionLoaded(CurrentLevel)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptThing spawnPoint = Game->GetThingWithScriptName("HTG_PartySpawnPoint");
            if (spawnPoint.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                SpawnHere = true;
                for (unsigned int i = 0; i < AreasSpawnedIn.size(); ++i) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (CurrentLevel == AreasSpawnedIn[i]) {
                        if (IsActiveThreadTerminating())
                            return;
                        SpawnHere = false;
                    }
                }
                if (IsActiveThreadTerminating())
                    return;
                if (!PartyReadyToTransition) {
                    if (IsActiveThreadTerminating())
                        return;
                    SpawnHere = false;
                }
                if (SpawnHere) {
                    if (IsActiveThreadTerminating())
                        return;
                    AreasSpawnedIn.push_back(CurrentLevel);
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (firstTime) {
                    if (IsActiveThreadTerminating())
                        return;
                    firstTime = false;
                }
            } else {
                if (IsActiveThreadTerminating())
                    return;
                SpawnHere = false;
                ReasonForFailing = "TEXT_QST_030_FAILURE_LEFT_MAIN_ROUTE";
                MissionFailed = true;
            }
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
}