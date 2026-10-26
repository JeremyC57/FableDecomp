// CV_AssassinAttacksScript::Init() -- reset the assassin-attack state and the five region records (names and limits from the script def), then spawn the WatchForQuestComplete thread
// Leaf: ?Init@CV_AssassinAttacksScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E39540, length 268 bytes.
// Callees: CCharString::operator=(const char*) @ 0x0099EFE0, vector<long>::resize(size_t, const long&) @ 0x004428F0,
// CCharString::operator=(const CCharString&) @ 0x0099EFB0, operator new @ 0x00BFEA1A, CSpawnedFuncBase::CSpawnedFuncBase(
// const CCharString&, unsigned long) @ 0x00CDD450, CScriptBase::AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&)
// @ 0x00CB7E50, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0; the thread runs
// CV_AssassinAttacksScript::WatchForQuestComplete @ 0x00E39650 (CSpawnedFunc0 vtable 0x012E35C4).
// Globals: pGlobalScriptDef @ 0x0143E90C (region names CArray<CCharString> +0x3cc, region limits CArray<long> +0x3d8);
// strings 0x0122D70E "", 0x012E3838 "WatchForQuestComplete".
// Layout (retail = Ego_r.pdb - 0x14, the 12-byte vector): RegionIndex +0x48, AssassinObjectCount +0x4c, Activated +0x50,
// RegionOfMostRecentKill +0x54, RegionInfo[5] +0x58 (RegionData {name, count, max, assassinActive}, 16 bytes),
// RegionKillCounts +0xa8, QuestComplete +0xb4, RewardsGiven +0xb5, GivenQuestCard +0xb6.
// Shape: the landed CQ_FireHeartScript::CheckCompletion 0x00D3AD60 spawn idiom (the thread name an implicit conversion
// that exists only when operator new succeeded, guarded by a flag bit; the empty region name an explicit temporary);
// the resize fill value is a `0` temporary in the frame; the def is re-read for the second array after the store.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};

void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A

struct CSpawnedFuncBase {                               // retail 0x34 bytes (CScriptProcessBase + thread state)
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                          // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};

template <class T> struct CSpawnedFunc0 : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc0(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) const { return *(_M_start + n); }
    void resize(unsigned long n, const T& value);       // 0x004428F0
};

struct CScriptDef {
    unsigned char             _pad000[0x3cc];
    CRetailVector<CCharString> AssassinAttackRegionNames;   // +0x3cc
    CRetailVector<long>        AssassinAttackRegionLimits;  // +0x3d8
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CScriptBase {                                    // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};
struct RegionData {
    CCharString name;                                   // +0x00
    long        count;                                  // +0x04
    long        max;                                    // +0x08
    bool        assassinActive;                         // +0x0c
};
enum AA_LocationIdentifier { AA_LOCATION_NONE = 0 };
namespace NScript {
class CV_AssassinAttacksScript : public CScriptBase {
public:
    void*                 GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    AA_LocationIdentifier RegionIndex;                  // +0x48
    long                  AssassinObjectCount;          // +0x4c
    bool                  Activated;                    // +0x50
    CCharString           RegionOfMostRecentKill;       // +0x54
    RegionData            RegionInfo[5];                // +0x58
    CRetailVector<long>   RegionKillCounts;             // +0xa8
    bool                  QuestComplete;                // +0xb4
    bool                  RewardsGiven;                 // +0xb5
    bool                  GivenQuestCard;               // +0xb6
    void Init();
    void WatchForQuestComplete();                       // 0x00E39650
};
} // namespace NScript
using NScript::CV_AssassinAttacksScript;

void NScript::CV_AssassinAttacksScript::Init()
{
    AssassinObjectCount = 0;
    Activated = false;
    RegionOfMostRecentKill = "";
    QuestComplete = false;
    RewardsGiven = false;
    GivenQuestCard = false;
    RegionKillCounts.resize(5, 0);
    for (long i = 0; i < 5; ++i) {
        RegionInfo[i].count = 0;
        RegionInfo[i].max = pGlobalScriptDef->AssassinAttackRegionLimits[i];
        RegionInfo[i].name = pGlobalScriptDef->AssassinAttackRegionNames[i];
        RegionInfo[i].assassinActive = false;
    }
    AddSpawnedFunction(new CSpawnedFunc0<CV_AssassinAttacksScript>("WatchForQuestComplete", 0, this,
                                                                   &NScript::CV_AssassinAttacksScript::WatchForQuestComplete),
                       CCharString("", -1));
}