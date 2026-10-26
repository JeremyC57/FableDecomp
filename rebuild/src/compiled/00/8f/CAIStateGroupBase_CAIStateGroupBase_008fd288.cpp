// CAIStateGroupBase::~CAIStateGroupBase() -- the implicit destructor: destroys Name (CCharString at +0x0C), then chains
// to the base CBaseClass destructor (tail jump); no vtable store (retail: push esi ; mov esi,ecx ; lea ecx,[esi+0Ch] ;
// call 0x0099EAE0 ; mov ecx,esi ; pop esi ; jmp 0x0099A2E0).
// Leaf: ??1CAIStateGroupBase@@UAE@XZ
// Retail: Fable.exe @ 0x008FD288, 23 bytes. Ego_r ??1CAIStateGroupBase@@UAE@XZ (virtual: the base CBaseClass has a
// virtual destructor); Ego_r layout: vptr, PBrain +4, ScriptAIPriority +8, Name (CCharString) +0x0C, Terminated +0x10,
// Priority +0x11, Enabled +0x12. Callers reach it as the base destructor of their state groups (namepass D1).
// Was modelled as `FableDestVal_008fd288` (inner/outer stand-ins). The destructor is written empty on a
// __declspec(novtable) class, as for 0x0062E900 ~CCombatActionBase: VC7.1 then emits no vptr store, as the implicit one.
class CBaseClass {
public:
    virtual ~CBaseClass();                         // 0x0099A2E0
};
class CCharString {
public:
    ~CCharString();                                // 0x0099EAE0
    void* PString;
};
class CAIBrain;
class __declspec(novtable) CAIStateGroupBase : public CBaseClass {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                              // +0x04
    long ScriptAIPriority;                         // +0x08
    CCharString Name;                              // +0x0C
    bool Terminated;                               // +0x10
    unsigned char Priority;                        // +0x11
    bool Enabled;                                  // +0x12
};
CAIStateGroupBase::~CAIStateGroupBase()
{
}
