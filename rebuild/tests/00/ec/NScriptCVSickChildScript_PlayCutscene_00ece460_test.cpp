// Behaviour test for 0x00ECE460 NScript::CV_SickChildScript::PlayCutscene(CCharString name, CCharString arg): the source's
// body over a test copy of the model (CCharString over a char buffer, a small array map in place of the STLport tree,
// a recording game interface and RunCutsceneMacro).
#include <cstdio>
#include <cstring>
#include <cctype>

static int StrICmp(const char* a, const char* b)
{
    for (;; ++a, ++b) {
        int ca = std::tolower((unsigned char)*a), cb = std::tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        if (ca == 0) return 0;
    }
}
class CCharStringData {
public:
    char* PData;
    char  Buffer[64];
};
static int gLiveStrings = 0;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString() : PStringData(0) { }
    CCharString(const char* text, long length = -1) : PStringData(new CCharStringData) { (void)length; std::strcpy(PStringData->Buffer, text); PStringData->PData = PStringData->Buffer; ++gLiveStrings; }
    CCharString(const CCharString& o) : PStringData(0) { if (o.PStringData) { PStringData = new CCharStringData; std::strcpy(PStringData->Buffer, o.PStringData->PData); PStringData->PData = PStringData->Buffer; ++gLiveStrings; } }
    ~CCharString() { if (PStringData) { delete PStringData; --gLiveStrings; } }
    const CCharString& operator=(const CCharString& other)
    {
        if (this != &other) { this->~CCharString(); PStringData = 0; new (this) CCharString(other); }
        return *this;
    }
    void* operator new(size_t, void* p) { return p; }
    void* operator new(size_t n) { return ::operator new(n); }
    void operator delete(void* p) { ::operator delete(p); }
    void operator delete(void*, void*) { }
    const char* GetAsCharArray() const { return PStringData == 0 ? "" : PStringData->PData; }
    bool operator!=(const CCharString& other) const { return StrICmp(GetAsCharArray(), other.GetAsCharArray()) != 0; }
};
enum EScriptAIPriority { };
class CCPPointerInfo { public: long RefCount; void (*DeleteFunc)(void* data); void* Data; };
template <class T> class CCountedPointer {
public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer& operator=(const CCountedPointer& other) { Data = other.Data; Info = other.Info; return *this; }
    ~CCountedPointer() { Data = 0; Info = 0; }
};
class CBaseClass { public: virtual ~CBaseClass() { } };
class CScriptThing { public: int Id; };
class CBaseObject : public CBaseClass {
protected:
    CBaseObject() : IntelligentPointer(0) { }
    virtual ~CBaseObject() { }
    CBaseObject& operator=(const CBaseObject&) { return *this; }
public:
    void* IntelligentPointer;
};
class CScriptGameResourceObjectBase : public CBaseObject { public: CScriptGameResourceObjectBase() { } virtual ~CScriptGameResourceObjectBase() { } };
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {
        CScriptGameResourceObjectBase::operator=(other);
        PImp = other.PImp;
        return *this;
    }
};
namespace tstd {
template <class K, class V> class map {
public:
    K keys[8]; V values[8]; unsigned int count;
    map() : count(0) { }
    V& operator[](const K& k)
    {
        for (unsigned int i = 0; i < count; ++i)
            if (!(keys[i] != k)) return values[i];
        keys[count] = k;
        return values[count++];
    }
    const V* find(const char* k) const
    {
        CCharString key(k);
        for (unsigned int i = 0; i < count; ++i)
            if (!(keys[i] != key)) return &values[i];
        return 0;
    }
};
}
static const char* gRunName = 0;
static char gRunNameBuf[64];
static int gRuns = 0, gCamera = -1, gCameraSets = 0;
static void* gWitch = 0; static void* gMum = 0; static void* gHero = 0;
static char gArg1[64];
static int gArgCount = -1;
void RunCutsceneMacro(const CCharString& name, tstd::map<CCharString, CScriptGameResourceObjectScriptedThingBase>& cast,
                      tstd::map<CCharString, bool>* flags, tstd::map<CCharString, CCharString>* args, bool a5, bool a6)
{
    ++gRuns;
    std::strcpy(gRunNameBuf, name.GetAsCharArray()); gRunName = gRunNameBuf;
    const CScriptGameResourceObjectScriptedThingBase* w = cast.find("WITCH");
    const CScriptGameResourceObjectScriptedThingBase* m = cast.find("MUM");
    const CScriptGameResourceObjectScriptedThingBase* h = cast.find("HERO");
    gWitch = w ? (void*)w->PImp.Data : 0; gMum = m ? (void*)m->PImp.Data : 0; gHero = h ? (void*)h->PImp.Data : 0;
    gArgCount = (int)args->count;
    const CCharString* a = args->find("$ARG1");
    std::strcpy(gArg1, a ? a->GetAsCharArray() : "-");
    if (flags != 0 || a5 || !a6 || gCamera != 1) gRuns = -100;
}
static CScriptGameResourceObjectScriptedThingBase gHeroObject;
class CGameScriptInterfaceBase {
public:
    CScriptThing Hero;
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority)
    {
        if (thing.Id != 7 || (int)priority != 4) return false;
        object.PImp.Data = &gHeroObject;
        return true;
    }
    virtual CScriptThing& GetHero() { return Hero; }
    virtual void FixMovieSequenceCamera(bool fix) const { gCamera = fix ? 1 : 0; ++gCameraSets; }
};
class CScriptBase { public: unsigned char _body[0x40]; };
namespace NScript {
class CV_SickChildScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;
    void*                     PMasterData;
    CScriptGameResourceObjectScriptedThingBase seh_Mother;
    CScriptGameResourceObjectScriptedThingBase seh_Witch;
    void PlayCutscene(CCharString name, CCharString arg);
};
void CV_SickChildScript::PlayCutscene(CCharString name, CCharString arg)
{
    tstd::map<CCharString, CCharString> csargs;
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    if (arg != "NULL")
        csargs["$ARG1"] = arg;
    tstd::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["WITCH"] = seh_Witch;
    cast["MUM"] = seh_Mother;
    cast["HERO"] = hero;
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro(name, cast, 0, &csargs, false, true);
    Game->FixMovieSequenceCamera(false);
}
}

int main()
{
    static CScriptGameResourceObjectScriptedThingBase witchImpl, mumImpl;
    CGameScriptInterfaceBase game;
    game.Hero.Id = 7;
    NScript::CV_SickChildScript script;
    script.Game = &game;
    script.seh_Witch.PImp.Data = &witchImpl;
    script.seh_Mother.PImp.Data = &mumImpl;
    script.PlayCutscene(CCharString("SC_WITCH_GIVES_CURE"), CCharString("MUSHROOM"));
    int ok = gRuns == 1 && std::strcmp(gRunName, "SC_WITCH_GIVES_CURE") == 0 && gWitch == &witchImpl && gMum == &mumImpl
          && gHero == &gHeroObject && gArgCount == 1 && std::strcmp(gArg1, "MUSHROOM") == 0 && gCamera == 0 && gCameraSets == 2;
    script.PlayCutscene(CCharString("SC_INTRO"), CCharString("null"));     // "NULL" compares case-insensitively
    ok = ok && gRuns == 2 && std::strcmp(gRunName, "SC_INTRO") == 0 && gArgCount == 0 && std::strcmp(gArg1, "-") == 0 && gCameraSets == 4;
    ok = ok && gLiveStrings == 0;
    if (!ok) { std::printf("00ece460_TEST FAIL\n"); return 1; }
    std::printf("00ece460_TEST PASS\n");
    return 0;
}