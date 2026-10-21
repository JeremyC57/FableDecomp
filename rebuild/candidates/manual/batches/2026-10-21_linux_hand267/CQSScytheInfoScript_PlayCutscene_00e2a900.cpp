// CQS_ScytheInfoScript::PlayCutscene(CCharString name, bool inMovieSequence) -- play a scripted cut-scene: take the hero under scripted control and enter him in the cut-scene cast as the hero, fix the movie camera, run the cut-scene (inside a paused movie sequence when inMovieSequence), then release the camera and clear the argument and cast maps
// Leaf: ?PlayCutscene@CQS_ScytheInfoScript@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2A900, 499 bytes (`ret 8`; the by-value name is destroyed by the callee). Callees: CBaseObject
// ctor/dtor/operator= 0x0099A380 / 0x0099A430 / 0x0099A3B0, CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0, operator delete
// 0x00BFE9BC, the cast map's operator[] 0x00CD3D2E and clear 0x00CD3699, the argument map's clear 0x009AACE0, the movie PImp dtor
// 0x006E7AB0, the __fastcall cut-scene runner 0x00CBFB7D (descriptive name; ecx = name, edx = the cast map, then 0, &cs_args, 0, true);
// CGameScriptInterface (+0x40) slots 8 StartScriptingEntity, 70 GetHero, 370 StartMovieSequence, 371 FixMovieSequenceCamera, 379
// PauseAllNonScriptedEntities (ghidra_out/cgsi_slots_aligned.tsv). Vtables: ScriptedThingBase 0x0127094C, MovieBase 0x01260EF4,
// ScriptGameResourceObjectBase 0x0126008C. Literals: the cast key @ 0x01255174 (text not recovered; "HERO" is a placeholder,
// relocation-masked), "" 0x0122D70E.
// Layout (retail = Ego_r.pdb - 0x14, 12-byte STLport maps): Game +0x40, MissionSucceeded +0x48, cs_args +0x4c, cs_chars +0x58.
// Shape: the cast-map key converts IMPLICITLY (the temporary's address is re-taken for the push); CCountedPointer::operator= copies
// the source's Data/Info into locals before releasing (two frame words); the scripted-thing handle's destructor is inline (PImp
// release + base vptr store) while the movie handle's PImp dtor is out of line (as the landed CFireHeartHolder::Main 0x00DF2D30); the
// movie-sequence name is a block-local named string; Pause/unpause go through one named copy of Game.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
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
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
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
struct CScriptThing;
struct CBaseObject {                                        // CBaseIntelligentPointer root
    virtual void v00();
    void* m_pIntelligentPointer;                            // +0x04
    CBaseObject();                                          // 0x0099A380
    ~CBaseObject();                                         // 0x0099A430
    CBaseObject& operator=(const CBaseObject& other);       // 0x0099A3B0
};
struct CScriptGameResourceObjectBase : CBaseObject {        // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
struct CScriptThingImp;
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<CScriptThingImp> PImp;                  // +0x08
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                    // 0x006E7AB0
};
struct CScriptGameResourceObjectMovieBase : CScriptGameResourceObjectBase {   // vtable 0x01260EF4
    CMovieImpPointer PImp;                                  // +0x08
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
struct CGameScriptInterface {
    VPAD8(pa)                                                                        // slots 0..7
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD1(qe)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD256(ra) VPAD32(rb) VPAD8(rc) VPAD2(rd) VPAD1(re)                             // slots 71..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5C8)
    virtual void FixMovieSequenceCamera(bool fix) const;                             // slot 371 (+0x5CC)
    VPAD4(sa) VPAD2(sb) VPAD1(sc)                                                    // slots 372..378
    virtual void PauseAllNonScriptedEntities(bool pause);                            // slot 379 (+0x5EC)
};
struct CCutsceneArgMap {                                    // map<CCharString, CCharString> (retail 12 bytes)
    void* _body[3];
    void clear();                                           // 0x009AACE0
};
struct CCutsceneCastMap {                                   // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
    void clear();                                           // 0x00CD3699
};
bool __fastcall RunScriptedCutscene(const CCharString& name, CCutsceneCastMap& cast, void* a3, CCutsceneArgMap& args, void* a5, bool a6);   // 0x00CBFB7D (descriptive)

class CQS_ScytheInfoScript {
public:
    unsigned char         _pad00[0x40];
    CGameScriptInterface* Game;                             // +0x40
    unsigned char         _pad44[0x48 - 0x44];
    bool                  MissionSucceeded;                 // +0x48 (PDB +0x5c)
    CCutsceneArgMap       cs_args;                          // +0x4c (PDB +0x60)
    CCutsceneCastMap      cs_chars;                         // +0x58 (PDB +0x70)
    void PlayCutscene(CCharString name, bool inMovieSequence);
};

void CQS_ScytheInfoScript::PlayCutscene(CCharString name, bool inMovieSequence)
{
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    cs_chars["HERO"] = hero;
    Game->FixMovieSequenceCamera(true);
    if (inMovieSequence) {
        CScriptGameResourceObjectMovieBase movie;
        {
            CCharString sequence("");
            Game->StartMovieSequence(sequence, movie);
        }
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        RunScriptedCutscene(name, cs_chars, 0, cs_args, 0, true);
        game->PauseAllNonScriptedEntities(false);
    } else
        RunScriptedCutscene(name, cs_chars, 0, cs_args, 0, true);
    Game->FixMovieSequenceCamera(false);
    cs_args.clear();
    cs_chars.clear();
}
