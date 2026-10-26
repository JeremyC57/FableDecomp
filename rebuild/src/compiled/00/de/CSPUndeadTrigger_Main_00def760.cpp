// CSP_UndeadTrigger::Main() -- the secret-passage undead trigger: spawn the parent quest's SwitchDoorsOnAndOff once, wait until the hero is within 2.5 units of Me, then turn every "SP_UndeadSpawnPoint<data>" marker into an undead soldier that hunts the hero (a "BlockADoor" marker also blocks the doors), wait until all of them are dead, unblock the doors and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DEF760, length 944 bytes, plain `ret`.
// Callees: IsActiveThreadTerminating 0x00F35B30; operator new 0x00BFEA1A; CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0, operator+(const CCharString&, const CCharString&) 0x0099F570 and operator+(const char*, const
// CCharString&) 0x0099F690 (both __fastcall); CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012DDC54),
// CScriptBase::AddSpawnedFunction 0x00CB7E50, CQ_SecretPassageScript::SwitchDoorsOnAndOff 0x00DEFB10;
// IsDistanceBetweenThingsUnder 0x00CBE2FF / AreAllThingsInVectorDead 0x00CBED00 (__fastcall); ~CScriptThing
// 0x004AA840, CScriptThing::operator= 0x008AB980 (inline at its first use, out of line at the second), operator
// delete 0x00BFE9BC, CBasicString<char>::Compare 0x00411570 (the inlined CCharString == "BlockADoor"); CScriptThing slots
// 3 GetDataString, 6 GetPos; CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 75 GetAllThingsWithScriptName,
// 91 CreateCreature, 108 RemoveThing, 451 GiveThingBestEnemyTarget.
// Literals "SwitchDoorsOnAndOff" 0x012DDF78, "ParentClass." 0x012C9934, "" 0x0122D70E, "SP_UndeadSpawnPoint"
// 0x012DDF64, "EnemySeekingHero" 0x012DDD98, "CREATURE_OK_UNDEAD_SOLDIER_SWORD" 0x0127D594, "BlockADoor" 0x012DDF58.
// Layout: Ego_r.pdb CSP_UndeadTrigger: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18,
// UndeadVec +0x1c (12-byte STLport vector in retail), TempCreature +0x28. CQ_SecretPassageScript is retail = PDB - 0x14:
// DoorsBlocked +0x4c, DoorsBeingDone +0x4d.
// Shape: CScriptThing::operator= and its CCountedPointer::operator= are __forceinline (the counted-pointer assignment
// caches the source Data/Info first, as the out-of-line 0x008AB980 body does); the second use, `UndeadVec[i] =
// TempCreature`, binds the element to a reference first and assigns under `#pragma inline_depth(0)` so only operator=
// stays out of line; the spawned function is the CUndeadRisingZombieAttacker::OnPredicateFail 0x00E0DE90 new-expression.
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                              // CBasicString<char>, 13 bytes
    char*         PData;                               // +0x00
    unsigned long StringLength;                        // +0x04
    unsigned long DataLength : 31;                     // +0x08
    bool          UseFastExtend : 1;                   // +0x0c
};
class CCharStringData { public:                               // 17 bytes
    CBasicStringChar Data;                             // +0x00
    long             NoRefs;                           // +0x0d
};
#pragma pack(pop)
long __fastcall Compare(const char* a, const char* b); // 0x00411570 CBasicString<char>::Compare
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return Compare(PStringData->Data.PData, text) ? false : true;
    }
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
class C3DVector { public: float X, Y, Z; };
void operator delete(void* p);                         // 0x00BFE9BC
struct CCountBlock {
    long References;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CCountedPointer {                               // CCountedPointer<CScriptThing>: {Data, Info}
    void*        Data;
    CCountBlock* Info;
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        void* data = other.Data;
        CCountBlock* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->References;
                if (Info->References == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->References;
        }
        return *this;
    }
};
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void t00(); virtual void t01(); virtual void t02();
    virtual CCharString GetDataString() const;         // slot 3 (+0x0c)
    virtual void t04(); virtual void t05();
    virtual const C3DVector& GetPos() const;           // slot 6 (+0x18)
    CCountedPointer PImp;                              // +0x04
    virtual ~CScriptThing();                                   // 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }   // 0x008AB980 out of line
};
struct CScriptThingVector {                            // STLport vector<CScriptThing>
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    CScriptThing& operator[](unsigned int n) { return _M_start[n]; }
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall AreAllThingsInVectorDead(const CScriptThingVector& things);                                // 0x00CBED00
class CSpawnedFuncBase { public:                              // retail 0x34 bytes
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                         // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                 // +0x34
    T* PClass;                                         // +0x38
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
class CScriptBase { public:                                   // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};
struct CQ_SecretPassageScript : CScriptBase {
    unsigned char _pad40[0x4c - 0x40];
    bool DoorsBlocked;                                 // +0x4C
    bool DoorsBeingDone;                               // +0x4D
    void SwitchDoorsOnAndOff();                        // 0x00DEFB10
};
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CGameScriptInterface { public:
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD2(qe)                              // slots 8..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD4(ra)                                                                        // slots 71..74
    virtual long GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const;   // slot 75 (+0x12c)
    VPAD8(sa) VPAD4(sb) VPAD2(sc) VPAD1(sd)                                          // slots 76..90
    virtual CScriptThing CreateCreature(const CCharString& defName, const C3DVector& pos, const CCharString& scriptName, bool a) const;   // slot 91 (+0x16c)
    VPAD16(ta)                                                                       // slots 92..107
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;       // slot 108 (+0x1b0)
    VPAD256(ua) VPAD64(ub) VPAD16(uc) VPAD4(ud) VPAD2(ue)                            // slots 109..450
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;   // slot 451 (+0x70c)
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CSP_UndeadTrigger : CEntityScriptBase {
    CQ_SecretPassageScript* ParentClass;               // +0x14
    void*                   PMasterData;               // +0x18 CQ_SunnyvaleMasterData*
    CScriptThingVector      UndeadVec;                 // +0x1c
    CScriptThing            TempCreature;              // +0x28
    void Main();
};

void CSP_UndeadTrigger::Main()
{
    if (!ParentClass->DoorsBeingDone) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->AddSpawnedFunction(new CSpawnedFunc<CQ_SecretPassageScript>(CCharString("ParentClass.") + CCharString("SwitchDoorsOnAndOff"), 0, ParentClass, &CQ_SecretPassageScript::SwitchDoorsOnAndOff), CCharString(""));
        ParentClass->DoorsBeingDone = true;
    }
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.5f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CCharString spawnPointName = "SP_UndeadSpawnPoint" + Me.GetDataString();
    Game->GetAllThingsWithScriptName(spawnPointName, UndeadVec);
    for (unsigned int i = 0; i < UndeadVec.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        TempCreature = Game->CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD", UndeadVec[i].GetPos(), "EnemySeekingHero", true);
        Game->GiveThingBestEnemyTarget(TempCreature, Game->GetHero());
        if (UndeadVec[i].GetDataString() == "BlockADoor") {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->DoorsBlocked = true;
        }
        CScriptThing& undead = UndeadVec[i];
#pragma inline_depth(0)
        undead = TempCreature;
#pragma inline_depth()
    }
    if (IsActiveThreadTerminating())
        return;
    while (!AreAllThingsInVectorDead(UndeadVec)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->DoorsBlocked = false;
    Game->RemoveThing(Me, false, true);
}