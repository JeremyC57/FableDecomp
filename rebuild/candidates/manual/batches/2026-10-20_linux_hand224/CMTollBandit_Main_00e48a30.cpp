// CM_TollBandit::Main() -- toll-bandit spawner entity script: when the "TollBandit" thing is dead and the script is not done, create the script def's toll-bandit creature at Me's position under the script name "TollBandit"
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E48A30, 263 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, CDefString::operator CCharString() 0x00415D70 (the script def
//   0x0143E90C, creature def name at +0x438), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840,
//   IsDone 0x00F35B30; CScriptThing slots 6 GetPos (+0x18) and 76 IsDead (+0x130); CGameScriptInterface slots 72
//   GetThingWithScriptName (+0x120) and 91 CreateCreature (+0x16c).
// Shape: the landed CScriptThing model (inline `~CScriptThing() {}` over the inline counted-pointer release): VC7.1 itself
//   inlines the destructor of the GetThingWithScriptName temporary in the bool initialiser and calls the out-of-line copy
//   for the discarded CreateCreature result; the "TollBandit" implicit conversion is built before the def-string
//   conversion (right-to-left argument order, implicit temporaries first).
struct CCharString {                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CDefString {
    long Index;
    operator CCharString() const;                        // 0x00415D70
};
struct CScriptDef {
    unsigned char _pad[0x438];
    CDefString TollBanditCreature;                       // +0x438
};
extern CScriptDef* g_pScriptDef;                         // 0x0143E90C
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
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
struct C3DVector { float X, Y, Z; };
struct CBaseClass {                                      // polymorphic root of the script handles
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                   // out of line 0x004AA840
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18) 0x004AA980
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual bool IsDead() const;                         // slot 76 (+0x130) 0x004AA890
};
struct CGameScriptInterface {
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
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090();
    virtual CScriptThing CreateCreature(const CCharString& creature, const C3DVector& pos, const CCharString& scriptName, bool b) const;   // slot 91 (+0x16c)
};
class CM_TollBandit {
public:
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    bool IsDone();                            // 0x00F35B30
    void Main();
};

void CM_TollBandit::Main()
{
    bool dead = Game->GetThingWithScriptName("TollBandit").IsDead();
    if (dead && !IsDone())
        Game->CreateCreature(g_pScriptDef->TollBanditCreature, Me.GetPos(), "TollBandit", false);
}
