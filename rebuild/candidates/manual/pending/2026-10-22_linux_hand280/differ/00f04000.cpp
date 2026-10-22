// CAT_Phase1Archer::Main() -- while the script runs: once the parent quest flags the archers to leave (+0x48 or +0x49), take scripting control of the archer, walk it to the nearest REGION_EXIT_POINT, and fade it out there
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F04000, length 630 bytes. Entity-script slot 1 (Main).
// Callees: CBaseIntelligentPointer ctor @ 0x0099A380 / dtor @ 0x0099A430, the scripted-thing handle (vtable 0x0127094C)
//          IsScripting @ 0x00CD23B9 / Release @ 0x00CD2770 / MoveToPosition @ 0x007E72F0 / destructor @ 0x007E74D0,
//          IsDone @ 0x00F35B30, CGameScriptInterface slots 7 NewScriptFrame (+0x1c), 8 StartScriptingEntity (+0x20),
//          81 GetNearestWithDefName (+0x144), IsDistanceBetweenThingsUnder @ 0x00CBE2FF and FadeOutAndKillEntity
//          @ 0x00CBE716 (__fastcall), ~CScriptThing @ 0x004AA840, CCharString @ 0x0099EBF0 / 0x0099EAE0, operator
//          delete @ 0x00BFE9BC; the zero vector 0x0143E8E0. String "REGION_EXIT_POINT" 0x012CA510.
// Layout: the entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14 (two leave flags at +0x48/+0x49); the
//         handle as the landed CUndeadRisingZombieAttacker::Main 0x00E0CFF0 (16 bytes).
struct CCharString {                           // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                            // 0x0099EAE0
};

void __cdecl operator delete(void* block);     // 0x00BFE9BC
struct C3DVector { float X, Y, Z; };
extern const C3DVector g_ZeroVector;           // 0x0143E8E0

struct CScriptThingImpl {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
};
struct CScriptThingImplAlive {
    virtual bool IsAlive() const;              // (slot 75 +0x12c; modelled below)
};
struct CCPPointerInfo;
struct CBaseClass {
    CBaseClass();
    virtual ~CBaseClass();
};
struct CScriptThing : CBaseClass {             // 12 bytes, vtable 0x01238C8C
    CScriptThing* PImp_Data;                   // +0x04
    CCPPointerInfo* PImp_Info;                 // +0x08
    ~CScriptThing();                           // 0x004AA840
    virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual const C3DVector& GetPos() const { if (!PImp_Data) return g_ZeroVector; return PImp_Data->GetPos(); }   // slot 6
    virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11(); virtual void s12();
    virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18();
    virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24();
    virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30();
    virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36();
    virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40(); virtual void s41(); virtual void s42();
    virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47(); virtual void s48();
    virtual void s49(); virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54();
    virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59(); virtual void s60();
    virtual void s61(); virtual void s62(); virtual void s63(); virtual void s64(); virtual void s65(); virtual void s66();
    virtual void s67(); virtual void s68(); virtual void s69(); virtual void s70(); virtual void s71(); virtual void s72();
    virtual void s73(); virtual void s74();
    virtual bool IsAlive() const { return PImp_Data && PImp_Data->IsAlive(); }   // slot 75 (+0x12c)
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool a, float time, bool b);                 // 0x00CBE716

struct SCountBlock {                           // the shared count block of a CCountedPointer
    long  m_references;                        // +0x00
    void (__fastcall* m_pfnDestroy)(void*);    // +0x04 destroys the object (in ecx)
    void* m_pObject;                           // +0x08
};
struct CCountedPointerWords {                  // {Data, Info}
    void* Data;
    SCountBlock* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
    ~CCountedPointerWords()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBaseIntelligentPointer {               // vtable 0x0129A7D8
    virtual void v00();
    void* m_pIntelligentPointer;               // +0x04
    CBaseIntelligentPointer();                 // 0x0099A380
    ~CBaseIntelligentPointer();                // 0x0099A430
};
struct CScriptGameResourceObjectBase : CBaseIntelligentPointer {   // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
enum EScriptEntityMoveType { MOVE_TYPE_1 = 1 };
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                 // +0x08
    bool IsScripting();                        // 0x00CD23B9
    void Release();                            // 0x00CD2770
    void MoveToPosition(const C3DVector& pos, float speed, EScriptEntityMoveType type, bool a, bool b);   // 0x007E72F0
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };
struct CGameScriptInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void NewScriptFrame() const;       // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78();
    virtual void v79(); virtual void v80();
    virtual CScriptThing GetNearestWithDefName(const CScriptThing& thing, const CCharString& defName) const;   // slot 81 (+0x144)
};
struct CQ_AmbushTradersScript {
    unsigned char _pad[0x48];
    bool ArchersLeave;                         // +0x48
    bool ArchersRetreat;                       // +0x49
};

class CAT_Phase1Archer {
public:
    void*                   __vftable;         // +0x00
    CGameScriptInterface*   GSI;               // +0x04
    CScriptThing            Thing;             // +0x08
    CQ_AmbushTradersScript* ParentClass;       // +0x14
    void*                   PMasterData;       // +0x18
    bool IsDone();                             // 0x00F35B30
    void Main();
};

void CAT_Phase1Archer::Main()
{
    CScriptGameResourceObjectScriptedThingBase handle;
    while (!IsDone()) {
        if (ParentClass->ArchersLeave || ParentClass->ArchersRetreat) {
            if (IsDone())
                return;
            if (handle.IsScripting())
                handle.Release();
            while (!GSI->StartScriptingEntity(Thing, handle, SCRIPT_AI_PRIORITY_4)) {
                GSI->NewScriptFrame();
                if (IsDone())
                    return;
            }
            if (IsDone())
                return;
            CScriptThing exitPoint = GSI->GetNearestWithDefName(Thing, "REGION_EXIT_POINT");
            if (exitPoint.IsAlive()) {
                if (IsDone())
                    return;
                if (handle.IsScripting()) {
                    if (IsDone())
                        return;
                    handle.MoveToPosition(exitPoint.GetPos(), 1.0f, MOVE_TYPE_1, false, true);
                }
                while (!IsDistanceBetweenThingsUnder(Thing, exitPoint, 2.0f)) {
                    GSI->NewScriptFrame();
                    if (IsDone())
                        return;
                }
                if (IsDone())
                    return;
                FadeOutAndKillEntity(Thing, true, 1.0f, true);
            }
        }
        GSI->NewScriptFrame();
    }
}
