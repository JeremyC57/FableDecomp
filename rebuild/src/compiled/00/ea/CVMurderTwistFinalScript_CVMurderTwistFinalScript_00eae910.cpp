// CV_MurderTwistFinalScript::CV_MurderTwistFinalScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012EF978, the empty Sheriff script thing, the empty Guards vector, the Sheriff entity handle and the empty seh_Guards vector
// Leaf: ??0CV_MurderTwistFinalScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EAE910, length 107 bytes (padded to 112), `ret 8`.
// Factory 0x00EB6A60 (`push 88h`, this ctor) is registered at 0x00CD9094 under "S_VMTF" / "V_MurderTwistFinal".
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CBase::CBase @ 0x0099A2D0 (CScriptThing root, vtable 0x01238C8C),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380.
// Layout (retail = Ego_r.pdb - 0x14, then -0x18 after the 12-byte Guards vector): SheriffState +0x48,
// AssassinState +0x4c, TaskDeclined +0x50, PreemptiveAttackInProgress +0x51, Sheriff +0x54, Guards +0x60,
// seh_Sheriff +0x6c (vtable 0x0127094C), seh_Guards +0x7c; size 0x88.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CGameScriptInterface {                      // retail vtable @ 0x01260F0C
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
struct CTimer {                                    // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
struct CScriptBase {                               // 0x40 bytes, the quest-script root (vtable 0x012C1648)
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
struct CBaseObject {                               // CBaseIntelligentPointer root
    virtual void v00();
    void* m_pIntelligentPointer;                   // +0x04
    CBaseObject();                                 // 0x0099A380
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 0x10 bytes
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
namespace N_0099a2d0 { struct CBase {
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();                              // 0x0099A2E0
}; } using N_0099a2d0::CBase;
struct CScriptThingImp;
struct CCPPointerInfo;
struct CCountedPointer_CScriptThing {              // CScriptThing::PImp
    CScriptThingImp* Data;                         // +0
    CCPPointerInfo*  Info;                         // +4
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};
struct CScriptThing : CBase {                      // vtable 0x01238C8C, 12 bytes
    CCountedPointer_CScriptThing PImp;             // +0x04
    CScriptThing() { }
    virtual ~CScriptThing();
};
struct CV_MurderTwistFinalScript : CQuestScriptObject {   // vtable 0x012EF978
    long SheriffState;                             // +0x48 (ESheriffState)
    long AssassinState;                            // +0x4c (EAssassinState)
    bool TaskDeclined;                             // +0x50
    bool PreemptiveAttackInProgress;               // +0x51
    CScriptThing Sheriff;                          // +0x54
    CRetailVector<CScriptThing> Guards;            // +0x60
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;   // +0x6c
    CRetailVector<CScriptGameResourceObjectScriptedThingBase> seh_Guards;   // +0x7c
    virtual void v00();
    CV_MurderTwistFinalScript(void* data, CGameScriptInterface* game);
};
CV_MurderTwistFinalScript::CV_MurderTwistFinalScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}