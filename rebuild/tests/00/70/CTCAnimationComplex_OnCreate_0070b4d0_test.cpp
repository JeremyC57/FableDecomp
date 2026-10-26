// Behaviour test for CTCAnimationComplex::OnCreate (0x0070B4D0).
#include <cstdio>
#include <cstring>

class CCharString { public:
    const char* PStringData;
    CCharString(const char* text, long = -1) : PStringData(text) {}
    ~CCharString() {}
};
class CAnimationEntry { public: long AnimIndex; };
class CAnimFilterBase { public:
    virtual bool Accept(const CAnimationEntry*) const { return true; }
    virtual ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase { virtual bool Accept(const CAnimationEntry*) const { return true; } };
static const char* g_askedName = 0;
static long g_askedIndex = -1;
static CAnimationEntry g_entry = { 42 };
class CAnimationSet { public:
    int dummy;
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase&) const
    { g_askedName = name.PStringData; g_askedIndex = index; return &g_entry; }
};
struct CBlendPair { float m_first, m_second; CBlendPair() : m_first(0.0f), m_second(0.0f) {} };
class CAnimInit { public:
    long Mode; unsigned char AnimType; bool Flag05; long AnimIndex; float Scales[3]; CBlendPair Blend;
    unsigned long Flags; long Arg24; long Percent;
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    { AnimType = (unsigned char)animType; Arg24 = arg24; AnimIndex = animIndex; Mode = mode; Flag05 = flag05; Flags = flags; Percent = percent; }
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    { Set(animType, arg24, animIndex, mode, flag05, flags, percent); }
};
static int g_resets = 0;
static CAnimInit* g_played = 0;
static CAnimInit g_copy(0, 0, 0, 0, false, 0, 0);
static long g_mode = -1, g_delay = -1;
class CAnimateThing { public:
    void ResetAnimations_0070ec30() { ++g_resets; }
    void SetMainAnim(const CAnimInit& init, long mode, long delay) { g_copy = init; g_mode = mode; g_delay = delay; }
};
static int g_destroyed = 0;
class CDefPointeeBase { public:
    virtual void Destroy0() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public: T* Object; CDefPointer() : Object(0) {} };
template <class T> struct CCountedDefPointer {
    T* Object;
    CCountedDefPointer(T* object) : Object(object) { if (Object) Object->AddRef(); }
    ~CCountedDefPointer() { if (Object) Object->Release(); }
};
class CAppearanceDef : public CDefPointeeBase { public: CAnimationSet Animation; };
static CAppearanceDef* g_appearance = 0;
static long g_baseRefDuringCall = 0;
class CThingBaseDef : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const
    { g_baseRefDuringCall = RefCount; out->Object = g_appearance; ++g_appearance->RefCount; return true; }
};
class CThing { public:
    CThingBaseDef* PBaseDef;
    CCountedDefPointer<CThingBaseDef> GetBaseDef() const { return CCountedDefPointer<CThingBaseDef>(PBaseDef); }
};
class CTCAnimationComplex {
public:
    CThing* ParentThing; CAnimateThing* PAnimation;
    void OnCreate()
    {
        CAnimateThing* animation = PAnimation;
        animation->ResetAnimations_0070ec30();
        const CThingBaseDef* baseDef = ParentThing->GetBaseDef().Object;
        CDefPointer<CAppearanceDef> out;
        baseDef->GetSubDef(&out);
        CAppearanceDef* def;
        const CAnimationEntry* entry;
        {
            CCharString name("DEFAULT");
            def = out.Object;
            entry = def->Animation.GetAnimation(name, 0, CAnimFilterNoGroup());
        }
        CAnimInit init(6, 0, entry->AnimIndex, 1, false, 0, 100);
        animation->SetMainAnim(init, 1, 0);
        if (def) {
            if (--def->RefCount == 0)
                def->Destroy();
        }
    }
};

int main()
{
    CThingBaseDef base; base.RefCount = 1;
    CAppearanceDef app; app.RefCount = 0; g_appearance = &app;
    CThing thing; thing.PBaseDef = &base;
    CAnimateThing animator;
    CTCAnimationComplex c; c.ParentThing = &thing; c.PAnimation = &animator;
    c.OnCreate();
    bool ok = g_resets == 1 && base.RefCount == 1 && g_baseRefDuringCall == 1 && std::strcmp(g_askedName, "DEFAULT") == 0
        && g_askedIndex == 0 && g_copy.AnimType == 6 && g_copy.AnimIndex == 42 && g_copy.Mode == 1 && !g_copy.Flag05
        && g_copy.Percent == 100 && g_copy.Arg24 == 0 && g_mode == 1 && g_delay == 0 && app.RefCount == 0 && g_destroyed == 1;
    if (!ok) { std::printf("0070b4d0_TEST FAIL\n"); return 1; }
    std::printf("0070b4d0_TEST PASS\n");
    return 0;
}