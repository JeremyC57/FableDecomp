// CTrophiesBriarTrophy::Main() -- the Briar trophy entity script: keep the game updating while BriarRose is alive; once she is gone (and the script is not done) remove the trophy thing (Me)
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E9D800, length 211 bytes (224 with padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CBaseClass::~CBaseClass @ 0x0099A2E0,
// CScriptThing::~CScriptThing @ 0x004AA840 (the out-of-line copy), operator delete @ 0x00BFE9BC, IsDone @ 0x00F35B30 (on
// `this`), the game interface's slots 72 (+0x120, GetThingWithScriptName), 7 (+0x1c, Update) and 108 (+0x1b0,
// RemoveThing(thing, false, true)); the thing implementation's slot 75 (+0x12c, IsAlive) behind the inlined
// CScriptThing::IsAlive.
// Layout: the CEntityScriptBase family (as the landed CHTE_Executioner::Main 0x00D6A0D0): Game +4, CScriptThing Me +8.
// Shape: the BeardyBaldy (0x00E503D0) wait: the name is an implicit conversion; the handle has THREE destructor sites (the
// return inside the loop, a second `if (IsActiveThreadTerminating()) return;` after it, and the end): VC7.1 expands the inline destructor at
// the first site only and calls the out-of-line ~CScriptThing (0x004AA840) at the other two, which tail-merge.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CBaseClass {
public:
    virtual ~CBaseClass();                                       // 0x0099A2E0
    CBaseClass();                                                // 0x0099A2D0
};

struct SScriptThingInfo {                                   // the counted block behind CScriptThing::PImp_Info
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

struct CScriptThingImpl {                                   // the game-side thing behind PImp_Data
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive();                                 // slot 75 (+0x12c)
};

class CScriptThing : public CBaseClass { public:                               // vtable 0x01238C8C, 12 bytes
    CScriptThingImpl* PImp_Data;                            // +0x04
    SScriptThingInfo* PImp_Info;                            // +0x08
    CScriptThing(const CScriptThing& other);
    ~CScriptThing()
    {
        if (PImp_Info) {
            --PImp_Info->m_references;
            if (PImp_Info->m_references == 0) {
                PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
    bool IsAlive()                                          // 0x004AB130 (CScriptThing slot 75), inlined
    {
        return PImp_Data && PImp_Data->IsAlive();
    }
};
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void Update();                                  // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name);   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b);   // slot 108 (+0x1b0)
};

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CTrophiesBriarTrophy : public CEntityScriptBase {
public:
    void*                 __vftable;                        // +0x00
    CGameScriptInterface* Game;                             // +0x04
    CScriptThing          Me;                               // +0x08
    void Main();
};

void CTrophiesBriarTrophy::Main()
{
    CScriptThing briar = Game->GetThingWithScriptName("BriarRose");
    while (briar.IsAlive()) {
        Game->Update();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, false, true);
}