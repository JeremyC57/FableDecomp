// CWomanToAttract::DoLeavingCutscene() -- the laughing woman leaves: take the hero under scripted control, cast him as HERO and the woman's own handle as WOMAN, pass the ingredient object as the $ITEM argument and run CS_SICK_LAUGHING_WOMAN_LEAVES with the movie camera fixed
// Leaf: ?DoLeavingCutscene@CWomanToAttract@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED0B10, 747 bytes, plain `ret`.
// Callees: CBaseObject ctor/dtor/operator= 0x0099A380 / 0x0099A430 / 0x0099A3B0, CCharString 0x0099EBF0 / 0x0099EAE0 /
//   operator= 0x0099EFB0, CDefString::operator CCharString 0x00415D70, CRT malloc 0x00BFEA0E / free 0x00BFEA14 (map headers),
//   operator delete 0x00BFE9BC, cast map operator[] 0x00CD3D2E / _M_erase 0x00CD344A, argument map operator[] 0x009AC700 /
//   _M_erase 0x009AA830, cut-scene runner 0x00CBFB7D (__fastcall); CGameScriptInterface slots 8 StartScriptingEntity,
//   70 GetHero, 371 FixMovieSequenceCamera. Literals "HERO" 0x01255174, "WOMAN" 0x012F2B04, "$ITEM" 0x012F2AFC,
//   "CS_SICK_LAUGHING_WOMAN_LEAVES" 0x012F2ADC; pGlobalScriptDef 0x0143E90C (IngredientObject at retail +0x738, as landed).
// Layout: Ego_r.pdb CWomanToAttract (52 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData
//   +0x18, GivenObject +0x1c, TimesMadeLaugh +0x20, seh_me +0x24.
// Shape: the landed CQ_WaspBossScript::WaspIntro 0x00E12F20 model; both local maps (cast, then arguments) are destroyed
//   inline, and the $ITEM value converts from the CDefString inside the assignment (key temporary destroyed first).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

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

struct CScriptThingImp;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;              // +0x04
    virtual ~CScriptThing() {}                          // out of line: 0x004AA840
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C
    CCountedPointer<CScriptThingImp> PImp;
    // implicit destructor; out of line: 0x007E74D0
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {                                                   // out of line: 0x008ABD10
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                // 0x006E7AB0
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    CMovieImpPointer PImp;
};

struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (retail STLport, 12 bytes)
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
    __forceinline ~CCutsceneCastMap()                   // inline here (out of line elsewhere: 0x00CDBFB0)
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
    void Erase(node* root);                             // 0x00CD344A
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap {                                // map<CCharString, CCharString> (retail STLport, 12 bytes)
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x18 - 16]; };
    node*         head;
    unsigned long size;
    unsigned char compare;
    __forceinline CCutsceneArgMap()
        : head(0)
    {
        head = (node*)malloc(sizeof(node));
        size = 0;
        head->colour = 0;
        head->parent = 0;
        head->left   = head;
        head->right  = head;
    }
    __forceinline ~CCutsceneArgMap()
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
    void Erase(node* root);                             // 0x009AA830
    CCharString& operator[](const CCharString& key);    // 0x009AC700
};
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CDefString { public:
    long Index;
    operator CCharString() const;                       // 0x00415D70
};
class CScriptDef { public:
    unsigned char _pad[0x738];
    CDefString IngredientObject;                        // +0x738
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

class CEntityScriptBase { public:                              // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                         // +0x04
    unsigned char         Me[0xc];                      // +0x08 CScriptThing
};
struct CWomanToAttract : CEntityScriptBase {
    void* ParentClass;                                  // +0x14 CV_SickChildScript*
    void* PMasterData;                                  // +0x18
    bool  GivenObject;                                  // +0x1c
    long  TimesMadeLaugh;                               // +0x20
    CScriptGameResourceObjectScriptedThingBase seh_me;  // +0x24
    void DoLeavingCutscene();
};

void CWomanToAttract::DoLeavingCutscene()
{
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    CCutsceneCastMap cast;
    cast["HERO"] = hero;
    cast["WOMAN"] = seh_me;
    CCutsceneArgMap args;
    args["$ITEM"] = pGlobalScriptDef->IngredientObject;
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro("CS_SICK_LAUGHING_WOMAN_LEAVES", cast, 0, &args, false, true);
    Game->FixMovieSequenceCamera(false);
}