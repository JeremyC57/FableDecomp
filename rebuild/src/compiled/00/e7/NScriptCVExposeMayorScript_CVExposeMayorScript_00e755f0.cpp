// CV_ExposeMayorScript::CV_ExposeMayorScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012E96CC, four entity handles (Suspicious, LadyGrey, Sheriff, Rhodri) and the empty Evidence string
// Leaf: ??0CV_ExposeMayorScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E755F0, length 142 bytes (padded to 144), `ret 8`.
// Factory 0x00E7B290 (`push 0A0h`, this ctor) is registered at 0x00CD866B under "S_VEM" / "V_ExposeMayor".
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 (x4),
// CCharString::CCharString() @ 0x0099E4B0.
// Layout (retail = Ego_r.pdb - 0x14): Path +0x48, StageNumber +0x4c, six bools +0x50..+0x55 (not initialised),
// seh_Suspicious +0x58, seh_LadyGrey +0x68, seh_Sheriff +0x78, seh_Rhodri +0x88 (vtable 0x0127094C), Evidence +0x98,
// MaxFlashDistFromStables +0x9c; size 0xA0.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CGameScriptInterface { public:                      // retail vtable @ 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
class CTimer { public:                                    // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
class CScriptBase { public:                               // 0x40 bytes, the quest-script root (vtable 0x012C1648)
    virtual void v00();
    unsigned char _body[0x3c];                     // +0x04..+0x3f
    CScriptBase();                                 // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CCountedPointerWords {                     // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CBaseObject { public:                               // CBaseIntelligentPointer root
    void* m_pIntelligentPointer;                   // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                     // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual void v00();
};
template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};
class CCharString { public:
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
};
namespace NScript {
class CV_ExposeMayorScript : public CQuestScriptObject {
public:   // vtable 0x012E96CC
    long Path;                                     // +0x48 (PATH_ID)
    long StageNumber;                              // +0x4c (STAGE_IN_PATH)
    bool HeroFlashedLightsCorrectly;               // +0x50
    bool PrisonerTalkedAboutLamp;                  // +0x51
    bool ResetLamp;                                // +0x52
    bool SaidYesToBecomeMayor;                     // +0x53
    bool QuestCardGiven;                           // +0x54
    bool MayorExposeGhostCutscenePlayed;           // +0x55
    CScriptGameResourceObjectScriptedThingBase seh_Suspicious;   // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_LadyGrey;     // +0x68
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;      // +0x78
    CScriptGameResourceObjectScriptedThingBase seh_Rhodri;       // +0x88
    CCharString Evidence;                          // +0x98
    float MaxFlashDistFromStables;                 // +0x9c
    virtual void v00();
    CV_ExposeMayorScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CV_ExposeMayorScript;
NScript::CV_ExposeMayorScript::CV_ExposeMayorScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}