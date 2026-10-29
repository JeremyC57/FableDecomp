// CSpectator::Init() -- the chicken-kicking spectator script picks its number from whichever "Spectator1"/"Spectator2" marker it stands on (else 3)
// Leaf: Init
// Retail 0x00E63730, 352 bytes. Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, the script
// interface's GetThingWithScriptName (slot 72, +0x120, CScriptThing by hidden pointer), IsDistanceBetweenThingsUnder
// 0x00CBE2FF (__fastcall, distance 1.0f pushed before the nested call), the CScriptThing temporary's inline destructor
// (vtable 0x01238C8C, counted pimpl release + operator delete 0x00BFE9BC, CBaseClass::~CBaseClass 0x0099A2E0), IsDone 0x00F35B30.
// Layout: Ego_r.pdb CSpectator (the CV_ChickenKickingScript one, 0x20 bytes): CEntityScriptBase {vptr, Game +4,
// CScriptThing Me +8}, ParentClass +0x14, PMasterData +0x18, SpectatorNumber +0x1C. Model as the landed CKG_Sheriff::Init
// 0x00D1DFF0. Strings: "Spectator1" 0x012E7450, "Spectator2" 0x012E7418.
// Shape: each marker test is a named bool filled inside its own block (name + temporary destroyed before the test);
// every arm is `if (IsActiveThreadTerminating()) return; SpectatorNumber = n;` -- VC hoists the second pair's shared IsDone call above
// the branch on the second result.
namespace NScript { class CPredicateBase; }
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
class CBaseClass {
public:
    virtual ~CBaseClass();                                      // 0x0099A2E0
};
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CScriptThingImp;
struct CScriptThingPImp {                          // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    SCountBlock*     Info;
    __forceinline ~CScriptThingPImp()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CScriptThingPImp PImp;
    __forceinline ~CScriptThing() {}
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
};
extern const char g_spectator1[];                  // 0x012E7450 "Spectator1"
extern const char g_spectator2[];                  // 0x012E7418 "Spectator2"
struct CV_ChickenKickingScript;
struct CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CSpectator : public CEntityScriptBase {
    void*                    __vftable;            // +0x00
    CGameScriptInterface*    Game;                 // +0x04
    CScriptThing             Me;                   // +0x08
    CV_ChickenKickingScript* ParentClass;          // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;          // +0x18
    long                     SpectatorNumber;      // +0x1c
    void Init();
};

void CSpectator::Init()
{
    bool nearFirst;
    {
        CCharString name(g_spectator1, -1);
        nearFirst = IsDistanceBetweenThingsUnder(Me, Game->GetThingWithScriptName(name), 1.0f);
    }
    if (nearFirst) {
        if (IsActiveThreadTerminating())
            return;
        SpectatorNumber = 1;
        return;
    }
    bool nearSecond;
    {
        CCharString name(g_spectator2, -1);
        nearSecond = IsDistanceBetweenThingsUnder(Me, Game->GetThingWithScriptName(name), 1.0f);
    }
    if (nearSecond) {
        if (IsActiveThreadTerminating())
            return;
        SpectatorNumber = 2;
        return;
    }
    if (IsActiveThreadTerminating())
        return;
    SpectatorNumber = 3;
}