#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static int g_frame = 0;
static int g_termAt = 1000;
static void (*g_hook)(int frame) = 0;
static void Log(const std::string& s) { char b[16]; std::sprintf(b, "@%d", g_frame); g_log += s; g_log += b; g_log += ";"; }
static void Reset(int termAt) { g_log = ""; g_frame = 0; g_termAt = termAt; g_hook = 0; }
class CCharString { public:
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
};
class CScriptThing { public:
    std::string Name;
    CScriptThing() { }
    CScriptThing(const std::string& name) : Name(name) { }
};
class CScriptBase { public:
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
};
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log.c_str()); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CUndeadRisingZombieAttacker::OnPredicateFail spawns the parent quest's ActivateNextLamp (named
// "ParentClass.ActivateNextLamp", region "") only when the Undead Rising centre exists within 6 units of the zombie
class C3DVector { public:
    float X, Y, Z;
    float MagSquared() const { return X * X + Y * Y + Z * Z; }
};
static C3DVector operator-(const C3DVector& a, const C3DVector& b) { C3DVector r; r.X = a.X - b.X; r.Y = a.Y - b.Y; r.Z = a.Z - b.Z; return r; }
static CCharString operator+(const CCharString& a, const CCharString& b) { return CCharString((a.S + b.S).c_str()); }
static bool g_centreExists = true;
struct CScriptThingP : CScriptThing {
    C3DVector Pos;
    bool Null;
    CScriptThingP() : Null(true) { Pos.X = Pos.Y = Pos.Z = 0.0f; }
    CScriptThingP(const std::string& n, float x, float y) : CScriptThing(n), Null(false) { Pos.X = x; Pos.Y = y; Pos.Z = 0.0f; }
    bool IsNull() const { return Null; }
    C3DVector GetPos() const { return Pos; }
};
class CGameScriptInterface { public:
    CScriptThingP GetThingWithScriptName(const CCharString& name)
    { return g_centreExists && name.S == "UndeadRisingCentre" ? CScriptThingP(name.S, 0.0f, 0.0f) : CScriptThingP(); }
};
class CSpawnedFuncBase { public: std::string Name; virtual ~CSpawnedFuncBase() { } virtual void Run() = 0; };
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    T* PClass;
    void (T::*Func)();
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)()) : PClass(pclass), Func(func) { Name = name.S; (void)id; }
    void Run() { (PClass->*Func)(); }
};
struct CQ_UndeadRisingScript {
    int Lamps;
    CSpawnedFuncBase* Last;
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& region) { Log("spawn " + func->Name + " [" + region.S + "]"); delete Last; Last = func; }
    void ActivateNextLamp() { ++Lamps; }
};
struct CUndeadRisingZombieAttacker {
    CGameScriptInterface* Game;
    CQ_UndeadRisingScript* ParentClass;
    CScriptThingP Me;
    void OnPredicateFail();
};
void CUndeadRisingZombieAttacker::OnPredicateFail()
{
    CScriptThingP centre = Game->GetThingWithScriptName("UndeadRisingCentre");
    if (!centre.IsNull() && (Me.GetPos() - centre.GetPos()).MagSquared() < 36.0f)
        ParentClass->AddSpawnedFunction(new CSpawnedFunc<CQ_UndeadRisingScript>(CCharString("ParentClass.") + CCharString("ActivateNextLamp"), 0, ParentClass, &CQ_UndeadRisingScript::ActivateNextLamp), CCharString(""));
}
int main()
{
    CGameScriptInterface game;
    CQ_UndeadRisingScript parent;
    parent.Lamps = 0; parent.Last = 0;
    CUndeadRisingZombieAttacker zombie;
    zombie.Game = &game;
    zombie.ParentClass = &parent;
    Reset(1000); zombie.Me = CScriptThingP("Z", 3.0f, 4.0f);   // distance 5
    zombie.OnPredicateFail();
    CHECK("00e0de90", g_log == "spawn ParentClass.ActivateNextLamp []@0;" && parent.Last != 0);
    parent.Last->Run();
    CHECK("00e0de90", parent.Lamps == 1);
    Reset(1000); zombie.Me = CScriptThingP("Z", 6.0f, 0.0f);   // exactly 6: not inside
    zombie.OnPredicateFail();
    CHECK("00e0de90", g_log == "");
    Reset(1000); g_centreExists = false; zombie.Me = CScriptThingP("Z", 0.0f, 0.0f);
    zombie.OnPredicateFail();
    CHECK("00e0de90", g_log == "");
    delete parent.Last;
    std::printf("00e0de90_TEST PASS\n");
    return 0;
}