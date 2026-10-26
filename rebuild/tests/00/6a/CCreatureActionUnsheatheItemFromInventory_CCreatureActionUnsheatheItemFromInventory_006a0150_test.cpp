// Behaviour test for 0x006A0150 CCreatureAction_UnsheatheItemFromInventory ctor: base parameters (-1, 0.55, animation chosen
// for the item with transition 4, sound pair zero, 100, c), the item pointer and the counted follow-up action.
#include <cstdio>
#include <cstring>
class CThing { public: const char* anim; };
class CThingCreatureBase { public: int id; };
class CAnimationEntry { public: char name[32]; long transition; CAnimationEntry(const char* n, long t) : transition(t) { std::strcpy(name, n); } };
static const char* GetAnimationToUse(CThingCreatureBase*, CThing* item) { return item->anim; }
static const char* GetAnimationFromName(CThingCreatureBase*, const char* name) { return name; }
class CSoundPair { public: long a, b; CSoundPair() : a(0), b(0) { } };
class CCreatureActionBase { public:
    CThingCreatureBase* owner; float fa, fb; char anim[32]; long transition; long sa, sb; long c, d;
    CCreatureActionBase(CThingCreatureBase* o, float a, float b, const CAnimationEntry& e, const CSoundPair& s, long cc, long dd)
        : owner(o), fa(a), fb(b), transition(e.transition), sa(s.a), sb(s.b), c(cc), d(dd) { std::strcpy(anim, e.name); }
    virtual ~CCreatureActionBase() { }
};
struct SCountBlock { long refs; void* object; };
struct CCountedPointer { CCreatureActionBase* object; SCountBlock* count;
    CCountedPointer(CCreatureActionBase* p) : object(p) { if (p) { count = new SCountBlock; count->refs = 1; count->object = object; } else count = 0; }
    ~CCountedPointer() { if (count && --count->refs == 0) { delete object; delete count; } } };
struct CUnsheathe : CCreatureActionBase {
    CThing* PItem; CCountedPointer PFollowUpAction;
    CUnsheathe(CThingCreatureBase* owner, CThing* item, CCreatureActionBase* followUp, long c)
        : CCreatureActionBase(owner, -1.0f, 0.55f, CAnimationEntry(GetAnimationFromName(owner, GetAnimationToUse(owner, item)), 4), CSoundPair(), 100, c)
        , PItem(item), PFollowUpAction(followUp) { }
};
int main()
{
    CThingCreatureBase hero; CThing sword; sword.anim = "UNSHEATHE_SWORD";
    CSoundPair z;
    CCreatureActionBase* next = new CCreatureActionBase(&hero, 0, 0, CAnimationEntry("X", 0), z, 0, 0);
    int ok;
    {
        CUnsheathe a(&hero, &sword, next, 7);
        ok = a.owner == &hero && a.fa == -1.0f && a.fb == 0.55f && std::strcmp(a.anim, "UNSHEATHE_SWORD") == 0 && a.transition == 4
          && a.sa == 0 && a.sb == 0 && a.c == 100 && a.d == 7 && a.PItem == &sword && a.PFollowUpAction.object == next
          && a.PFollowUpAction.count && a.PFollowUpAction.count->refs == 1;
        CUnsheathe b(&hero, &sword, 0, 1);
        ok = ok && b.PFollowUpAction.count == 0;
    }
    if (!ok) { std::printf("006a0150_TEST FAIL\n"); return 1; }
    std::printf("006a0150_TEST PASS\n");
    return 0;
}