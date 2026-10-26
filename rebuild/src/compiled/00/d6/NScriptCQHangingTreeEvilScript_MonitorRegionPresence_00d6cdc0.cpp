// CQ_HangingTreeEvilScript::MonitorRegionPresence() -- record BowerstoneJail as visited, then for each region change: wait for the party to leave and the new region to load, and if it has a party spawn point flag a spawn there (unless already spawned in that region), flag the bandits for removal on a region other than the last one spawned in, and remember the region; otherwise fail the quest for leaving the main route
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6CDC0, length 692 bytes. Spawned from CQ_HangingTreeEvilScript::DoMission
//   (0x00D6C067); the HTE twin of CQ_HangingTreeGoodScript::MonitorRegionPresence 0x00D773E0.
// Callees: CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 / ~CCharString
//   0x0099EAE0 / operator=(const char*) 0x0099EFE0 / operator!=(const CCharString&) 0x0099E900, Compare 0x00411570
//   (__fastcall; the inline CCharString ==), vector<CCharString>::_M_insert_overflow 0x00433530 (inline push_back) /
//   push_back 0x0044BFF0 (out of line), CScriptThing::IsAlive 0x004AB130, ~CScriptThing 0x004AA840,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40): NewScriptFrame (7),
//   IsRegionLoaded (12), MsgOnRegionLoaded (22), GetThingWithScriptName (72).
// Strings: "BowerstoneJail" 0x012C7788, "HTE_PartySpawnPoint" 0x012D2274, "TEXT_QST_029_FAILURE_LEFT_MAIN_ROUTE"
//   0x012D23D8.
// Layout (retail = Ego_r.pdb - 0x14, 12-byte vectors): MissionFailed +0x4d, SpawnHere +0x4f, SwitchingRegions +0x51,
//   RemoveBandits +0x52, ReasonForFailing +0x5c, CurrentLevel +0xa8, AreasSpawnedIn +0xc4 (two earlier vectors).
// Shape: CCharString == as the landed CV_ChapelOfEvilScript::WatchForRegionEntry 0x00E6B750 (CBasicString<char> data,
//   Compare); STLport push_back inline (the overflow path out of line).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
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
    bool operator!=(const CCharString& other) const;    // 0x0099E900
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
    CCharString* end() { return _M_finish; }
    CCharString& back() { return *(end() - 1); }
    void _M_insert_overflow(CCharString* pos, const CCharString& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00433530
    void push_back(const CCharString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
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
struct CScriptBase {                                        // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
class CQ_HangingTreeEvilScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
    bool IntroCutscenePlayed;                               // +0x48
    bool BanditsStarted;                                    // +0x49
    bool BanditKilled;                                      // +0x4a
    bool PartySuccessfullySetUp;                            // +0x4b
    bool MissionSucceeded;                                  // +0x4c
    bool MissionFailed;                                     // +0x4d
    bool BanditsLeftRegion;                                 // +0x4e
    bool SpawnHere;                                         // +0x4f
    bool AtWayPoint;                                        // +0x50
    bool SwitchingRegions;                                  // +0x51
    bool RemoveBandits;                                     // +0x52
    unsigned char _pad53[0x5c - 0x53];
    CCharString ReasonForFailing;                           // +0x5c
    unsigned char _pad60[0xa8 - 0x60];
    CCharString CurrentLevel;                               // +0xa8
    unsigned char _padac[0xc4 - 0xac];
    CCharStringVector AreasSpawnedIn;                       // +0xc4
    void MonitorRegionPresence();
};
void CQ_HangingTreeEvilScript::MonitorRegionPresence()
{
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
            CScriptThing spawnPoint = Game->GetThingWithScriptName("HTE_PartySpawnPoint");
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
                    if (!SpawnHere && CurrentLevel != AreasSpawnedIn.back()) {
                        if (IsActiveThreadTerminating())
                            return;
                        RemoveBandits = true;
                    }
                }
                if (IsActiveThreadTerminating())
                    return;
                if (SpawnHere) {
                    if (IsActiveThreadTerminating())
                        return;
                    AreasSpawnedIn.push_back(CurrentLevel);
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                }
            } else {
                if (IsActiveThreadTerminating())
                    return;
                SpawnHere = false;
                ReasonForFailing = "TEXT_QST_029_FAILURE_LEFT_MAIN_ROUTE";
                MissionFailed = true;
            }
            if (RemoveBandits) {
                if (IsActiveThreadTerminating())
                    return;
                RemoveBandits = false;
            }
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
}