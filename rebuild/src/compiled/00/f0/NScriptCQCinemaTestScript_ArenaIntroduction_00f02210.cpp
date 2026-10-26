// CQ_CinemaTestScript::ArenaIntroduction() -- take the hero under scripted control (priority 4), cast him as "Hero" and play the ARENA_HOH_INTRO cut-scene in a movie sequence
// Layout: only Game (+0x40, right after the 0x40-byte retail CScriptBase) is touched; no rebuild/include/engine header exists for the class.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F02210, length 586 bytes; called from 0x00F021F9 (CQ_CinemaTestScript::PhaseOne).
// Callees: CBaseObject ctor 0x0099A380 / dtor 0x0099A430 / operator= 0x0099A3B0, malloc 0x00BFEA0E / free 0x00BFEA14 (the
//   cast map's head node, inline), the map's recursive erase 0x00CD344A and operator[] 0x00CD3D2E, operator delete
//   0x00BFE9BC, movie PImp dtor 0x006E7AB0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   RunCutsceneMacro 0x00CBFB7D (__fastcall); through the interface (+0x40): StartScriptingEntity (8), GetHero (70),
//   StartMovieSequence (370).
// Strings: "Hero" 0x012C4570, "" 0x0122D70E, "ARENA_HOH_INTRO" 0x012F7370.
// Shape: the landed NScript::CCS_PlayCutsceneScript::Main 0x00F01480 cast-map / handle / movie model (all inline); a
//   second scripted-thing handle is constructed and never used.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
};
class CScriptThing;
class CBaseObject { public:
    void* m_pIntelligentPointer;
protected:
    CBaseObject();                                       // 0x0099A380
    CBaseObject& operator=(const CBaseObject& other);    // 0x0099A3B0
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
struct CScriptThingImp;
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C
    CCountedPointer<CScriptThingImp> PImp;
    void Release();                                    // 0x00CD2770
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                               // 0x006E7AB0
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    CMovieImpPointer PImp;
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
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
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD1(qe)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD256(ra) VPAD32(rb) VPAD8(rc) VPAD2(rd) VPAD1(re)                             // slots 71..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5C8)
    virtual void FixMovieSequenceCamera(bool fix) const;                             // slot 371 (+0x5CC)
    VPAD4(sa) VPAD2(sb) VPAD1(sc)                                                    // slots 372..378
    virtual void PauseAllNonScriptedEntities(bool pause);                            // slot 379 (+0x5EC)
};
struct CCutsceneCastMap {                              // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (retail STLport, 12 bytes)
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x24 - 16]; };
    node*         head;
    unsigned long size;
    unsigned char compare;
    __forceinline CCutsceneCastMap()
        : head(0)
    {
        head = (node*)malloc(sizeof(node));
        size = 0;
        head->colour = 0;
        head->parent = 0;
        head->left   = head;
        head->right  = head;
    }
    __forceinline ~CCutsceneCastMap()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
    void Erase(node* root);                            // 0x00CD344A
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
namespace NScript {
struct CScriptBase {                                   // retail 0x40 bytes
    unsigned char _body[0x40];
};
class CQ_CinemaTestScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                        // +0x40
    void*                 Data;                        // +0x44
    void ArenaIntroduction();
};
void CQ_CinemaTestScript::ArenaIntroduction()
{
    CScriptGameResourceObjectScriptedThingBase hero;
    CScriptGameResourceObjectScriptedThingBase unused;
    CCutsceneCastMap cast;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    cast["Hero"] = hero;
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    RunCutsceneMacro("ARENA_HOH_INTRO", cast, 0, 0, false, true);
}
}