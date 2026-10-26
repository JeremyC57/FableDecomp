// CGameScriptInterface::GetHero() -- refresh the cached hero script thing (+0x30) from the main player's hero thing,
// or reset it to the null script thing, and return it
// Leaf: ?GetHero@CGameScriptInterface@@UAEAAVCScriptThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00891CA0, length 176 bytes, `ret`.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, CThing::GetScriptThing()
// @ 0x004C7E50 (by hidden pointer), CGameScriptThing::CreateNullThing() @ 0x008CFF40, CScriptThing::operator= @ 0x008AB980,
// operator delete @ 0x00BFE9BC, CBaseClass::~CBaseClass @ 0x0099A2E0. CScriptThing vtable 0x01238C8C.
// Layout: CGameScriptInterface PlayerManager +0x14, Hero (CScriptThing, 12 bytes) +0x30; CScriptThing = CBaseClass
// vptr + PImp {Data, Info}; the counted-pointer info block {RefCount, `__fastcall` DeleteFunc, Data}.
// Shape: if/else of two whole-object assignments from temporaries; each temporary's inline destructor (vtable store,
// info release re-reading the member after the decrement, `delete Info`, both words cleared, then the out-of-line
// CBaseClass dtor) is expanded per arm, VC merging the arms at the shared `operator delete` call.

struct CThing;

struct CBaseClass {
    CBaseClass();                                   // 0x0099A2D0
    virtual void v00();
    ~CBaseClass();                                  // 0x0099A2E0 (non-virtual here: only the direct call is emitted)
};

struct CCPPointerInfo {
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};

struct CCountedScriptImp {
    void* Data;
    CCPPointerInfo* Info;
    ~CCountedScriptImp()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CScriptThing : public CBaseClass {
public:
    CCountedScriptImp PImp;
    virtual ~CScriptThing() {}
    CScriptThing& operator=(const CScriptThing& other);     // 0x008AB980
};
class CGameScriptThing : public CScriptThing {      // Ego_r.pdb: CGameScriptThing derives from CScriptThing
public:
    static CScriptThing __fastcall CreateNullThing(); // 0x008CFF40 (?CreateNullThing@CGameScriptThing@@SI?AVCScriptThing@@XZ)
};

struct CThing {
    CScriptThing GetScriptThing();                  // 0x004C7E50
};

class CPlayer {
public:
    CThing* GetHeroThing();                         // 0x00487DC0
};
struct CPlayerManager {
    CPlayer& GetMainPlayer();                       // 0x00449970 (the non-const overload; 0x00449960 is the const one)
};

struct CGameScriptInterface {
    virtual void v000();
    void* World;                                    // +0x04
    void* Component;                                // +0x08
    void* DisplayEngine;                            // +0x0c
    void* DefinitionManager;                        // +0x10
    CPlayerManager* PlayerManager;                  // +0x14
    unsigned char _pad18[0x18];
    CScriptThing Hero;                              // +0x30
    virtual CScriptThing& GetHero();                // Ego_r.pdb UAE (virtual)
};

CScriptThing& CGameScriptInterface::GetHero()
{
    CThing* hero = PlayerManager->GetMainPlayer().GetHeroThing();
    if (hero)
        Hero = hero->GetScriptThing();
    else
        Hero = CGameScriptThing::CreateNullThing();
    return Hero;
}