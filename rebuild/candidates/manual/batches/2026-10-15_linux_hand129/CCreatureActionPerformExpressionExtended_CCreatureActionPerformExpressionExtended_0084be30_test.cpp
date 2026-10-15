// Behaviour test for 0x0084BE30 (see src/0084be30.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("0084be30_TEST FAIL\n"); return 1; } } while (0)
struct CSoundPair { int FirstSound; int LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
// Combat-action base model: records what the derived constructor handed it.
struct CCombatActionBase {
    void* owner; void* thing; int actionCode; CSoundPair sound; char animName[48];
    CCombatActionBase(void* o, int code, const CSoundPair& s) : owner(o), thing(0), actionCode(code), sound(s) { animName[0] = 0; }
    CCombatActionBase(void* o, void* t, int code, const CSoundPair& s) : owner(o), thing(t), actionCode(code), sound(s) { animName[0] = 0; }
    void SetAnimationName(const char* n) { std::strcpy(animName, n); }
};
static CSoundPair GetSoundPair(void* owner, const char* name)
{
    CSoundPair p; p.FirstSound = owner ? 1 : 0; p.LastSound = name ? (int)std::strlen(name) : -1; return p;
}

// Creature-action base model (0x00693B30): owner, two floats, animation, sound, two words.
struct CActionBase {
    void* owner; float a, b; const void* anim; CSoundPair sound; int c, d; int AnimDelayTime;
    CActionBase(void* o, float fa, float fb, const void* an, const CSoundPair& s, int fc, int fd)
        : owner(o), a(fa), b(fb), anim(an), sound(s), c(fc), d(fd), AnimDelayTime(5) {}
};
static const void* GetAnimationEntry(void* owner, const char* name) { return name; }

struct CDef { int refCount; };
struct CDefPtr { CDef* p; CDefPtr(CDef* d) : p(d) {} CDefPtr(const CDefPtr& o) : p(o.p) { if (p) ++p->refCount; } };
struct CPtr { void* item; CPtr(void* i) : item(i) {} CPtr(const CPtr& o) : item(o.item) {} };
struct CExpr : CActionBase { CDefPtr PExpressionDef; CPtr PPerformToThing; int ExpressionItemDefIndex; bool EmoteIconAdded; int FrameCount; bool ReplacedAction; unsigned int SoundEffectID;
    CExpr(CDef* d, void* t) : CActionBase(0, 0.0f, 0.0f, 0, CSoundPair(), 0, 0), PExpressionDef(d), PPerformToThing(t), ExpressionItemDefIndex(3),
        EmoteIconAdded(true), FrameCount(11), ReplacedAction(false), SoundEffectID(99) {}
    CExpr(const CExpr& o) : CActionBase(o), PExpressionDef(o.PExpressionDef), PPerformToThing(o.PPerformToThing), ExpressionItemDefIndex(o.ExpressionItemDefIndex),
        EmoteIconAdded(o.EmoteIconAdded), FrameCount(o.FrameCount), ReplacedAction(o.ReplacedAction), SoundEffectID(o.SoundEffectID) {} };
int main()
{
    CDef def; def.refCount = 1; int t; CExpr src(&def, &t); CExpr a(src);
    CHECK(def.refCount == 2 && a.PExpressionDef.p == &def && a.PPerformToThing.item == &t && a.ExpressionItemDefIndex == 3);
    CHECK(a.EmoteIconAdded && a.FrameCount == 11 && !a.ReplacedAction && a.SoundEffectID == 99);
    CExpr nodef(0, 0); CExpr b(nodef); CHECK(b.PExpressionDef.p == 0);
    std::printf("0084be30_TEST PASS\n");
    return 0;
}
