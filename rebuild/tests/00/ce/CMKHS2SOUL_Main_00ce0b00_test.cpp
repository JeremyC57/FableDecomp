// Behaviour test for 0x00CE0B00 (sibling of the landed 0x00CE0920, soul threshold 1): model plus the body verbatim from final.cpp (the one reference binding has a
// lifetime-equivalent named-object spelling under g++, which rejects binding a temporary to a non-const reference),
// callees stubbed. Checks the wait for a freed soul, the DG_SOUL_TRAPPED effect at Me's position and XY angle, its
// re-creation whenever it dies (the old effect released, the new one held), termination at every test, and that every
// effect is released on every exit.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_effects = 0, g_created = 0, g_iatt = 0, g_termAt = 1 << 30, g_frames = 0, g_stopFrame = 1 << 30, g_freeFrame = 0, g_life = 2;
static bool g_badArg = false;
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    ~CCharString() { }
};
struct CScriptThingImp { int DiesAt; long RefCount; };
template <class T> class CCountedPointer { public:
    T* Data;
    CCountedPointer() : Data(0) { }
    explicit CCountedPointer(T* d) : Data(d) { if (Data) ++Data->RefCount; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data) { if (Data) ++Data->RefCount; }
    void Release() { if (Data && --Data->RefCount == 0) { delete Data; --g_effects; } Data = 0; }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (other.Data) ++other.Data->RefCount;
        Release();
        Data = other.Data;
        return *this;
    }
    ~CCountedPointer() { Release(); }
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public:
    CCountedPointer<CScriptThingImp> PImp;
    C3DVector Pos;
    CScriptThing() { Pos.X = 1.0f; Pos.Y = 2.0f; Pos.Z = 3.0f; }
    __forceinline ~CScriptThing() {}
    const C3DVector& GetPos() const { return Pos; }
    float GetAngleXY() const { return 0.5f; }
    bool IsAlive() const { return PImp.Data && g_frames < PImp.Data->DiesAt; }
};
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    CScriptThing CreateEffect(const CCharString& effect, const C3DVector& position, const CCharString& bone, float angle, bool b, bool c) const
    {
        if (strcmp(effect.Text, "DG_SOUL_TRAPPED") != 0 || strcmp(bone.Text, "") != 0 || position.X != 1.0f || position.Z != 3.0f || angle != 0.5f || b || c)
            g_badArg = true;
        CScriptThingImp* imp = new CScriptThingImp;
        imp->DiesAt = g_frames + g_life; imp->RefCount = 0;
        ++g_effects; ++g_created;
        CScriptThing t;
        t.PImp = CCountedPointer<CScriptThingImp>(imp);
        return t;
    }
};
struct CQ_HeroSouls2Script { long SoulsFreed; };

class CMK_HS2_SOUL {
public:
    CGameScriptInterface* Game;
    CScriptThing          Me;
    CQ_HeroSouls2Script*  ParentClass;
    bool IsDone()
    {
        ++g_iatt;
        if (g_frames >= g_freeFrame) ParentClass->SoulsFreed = 2;
        return g_iatt >= g_termAt || g_frames >= g_stopFrame;
    }
    void Main();
};

void CMK_HS2_SOUL::Main()
{
    while (ParentClass->SoulsFreed <= 1) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
#ifdef _MSC_VER
    CScriptThing& effect = Game->CreateEffect("DG_SOUL_TRAPPED", Me.GetPos(), "", Me.GetAngleXY(), false, false);
#else   // g++ cannot bind the temporary to a non-const reference: a named object has the same lifetime
    CScriptThing effect = Game->CreateEffect("DG_SOUL_TRAPPED", Me.GetPos(), "", Me.GetAngleXY(), false, false);
#endif
    for (;;) {
        if (IsDone())
            return;
        if (!effect.IsAlive()) {
#pragma inline_depth(0)
            if (IsDone())
                return;
#pragma inline_depth()
            effect.PImp = Game->CreateEffect("DG_SOUL_TRAPPED", Me.GetPos(), "", Me.GetAngleXY(), false, false).PImp
#pragma inline_depth(0)
            ;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}

static void Run(CMK_HS2_SOUL& s, int termAt, int stopFrame, int freeFrame, int life)
{
    static CGameScriptInterface gsi;
    static CQ_HeroSouls2Script quest;
    g_effects = 0; g_created = 0; g_iatt = 0; g_frames = 0; g_badArg = false;
    g_termAt = termAt; g_stopFrame = stopFrame; g_freeFrame = freeFrame; g_life = life;
    quest.SoulsFreed = freeFrame == 0 ? 2 : 1;
    s.Game = &gsi; s.ParentClass = &quest;
    s.Main();
}
int main()
{
    CMK_HS2_SOUL s;
    Run(s, 1 << 30, 9, 3, 2);       CHECK(g_frames == 9 && g_created == 3 && g_effects == 0 && !g_badArg);   // created at frame 3, recreated at 5 and 7, stopped at 9
    Run(s, 1 << 30, 4, 9, 2);       CHECK(g_frames == 4 && g_created == 0 && g_effects == 0);               // no soul freed yet
    Run(s, 1, 1 << 30, 0, 2);       CHECK(g_frames == 0 && g_created == 0 && g_iatt == 1 && g_effects == 0); // after the wait
    Run(s, 2, 1 << 30, 0, 2);       CHECK(g_frames == 0 && g_created == 1 && g_iatt == 2 && g_effects == 0); // loop top
    Run(s, 3, 1 << 30, 0, 0);       CHECK(g_frames == 0 && g_created == 1 && g_iatt == 3 && g_effects == 0); // dead effect, re-test (out-of-line exit)
    Run(s, 1 << 30, 6, 0, 1);       CHECK(g_frames == 6 && g_created == 6 && g_effects == 0);               // dies every frame: recreated each frame
    char line[64];
    _snprintf(line, sizeof(line), "00ce0b00_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}