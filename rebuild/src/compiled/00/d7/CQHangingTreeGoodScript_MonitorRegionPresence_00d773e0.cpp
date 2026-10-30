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
    const CCharString& operator=(const char* text);           // 0x0099EFE0
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
struct CScriptThingImpl;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
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