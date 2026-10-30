// NScript::CHeroBoastsScript::Main() -- boast dispatcher: every script frame collect the boasts the hero has just made and spawn a "ControlNewBoast" thread for each (boast type and quest name), until the script terminates
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE1AF0, length 376 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940, AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&)
//   @ 0x00CB7E50, CSpawnedFuncBase(const CCharString&, unsigned long) @ 0x00CDD450, operator new @ 0x00BFEA1A,
//   CCharString(const char*, long) @ 0x0099EBF0 / copy constructor @ 0x0099EC30 / ~CCharString @ 0x0099EAE0
//   ("ControlNewBoast" @ 0x012C36D4, "" @ 0x0122D70E), the vector's _STLP_alloc_proxy constructor @ 0x0074F4B0 (manifest
//   "__EH_epilog3" is a folded twin) and destructor @ 0x004E34B2 (manifest "CActiveFile::OnReadFinished"), the thread
//   body ControlNewBoast @ 0x00CE1C90; CGameScriptInterface (vtable 0x01260F0C) slots 7 NewScriptFrame and 34
//   MsgOnBoastsMade(vector<pair<long, CCharString> >&).
// Layout: as the landed boast rows (CScriptBase retail 0x40 bytes, GSI +0x40); the spawned function is
//   CSpawnedFunc2<CHeroBoastsScript, long, CCharString> (0x44 bytes: the 0x34-byte CSpawnedFuncBase, Func +0x34, PClass
//   +0x38, Arg1 +0x3C, Arg2 +0x40; vtable 0x012C36B8), modelled on the landed CQ_FireHeartScript::CheckCompletion
//   0x00D3AD60.
// Shape: the boast vector is a loop-body local (STLport _Vector_base with an out-of-line _STLP_alloc_proxy); the
//   by-value quest-name argument is a conditional temporary (flag bit in ebx); the empty region name is an explicit
//   temporary; the vector lives in its own `{}` block so its destructor runs before the frame call.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
namespace std {
template <class T> class allocator { public: allocator() { } };
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class V, class A> struct _STLP_alloc_proxy : A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p);                 // 0x0074F4B0
};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(A(), (T*)0) { }
    ~vector();                                          // 0x004E34B2
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
}
typedef std::vector<std::pair<long, CCharString> > BoastVector;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T, class A1, class A2> struct CSpawnedFunc2 : CSpawnedFuncBase {   // vtable 0x012C36B8
    void (T::*Func)(A1, A2);                            // +0x34
    T* PClass;                                          // +0x38
    A1 Arg1;                                            // +0x3c
    A2 Arg2;                                            // +0x40
    CSpawnedFunc2(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(A1, A2), A1 a1, A2 a2)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Arg1(a1), Arg2(a2) { }
    virtual void SuspendableProcess() { (PClass->*Func)(Arg1, Arg2); }
};
namespace NScript {
class CQ_SunnyvaleMasterData;
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {
class CHeroBoastsScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    void ControlNewBoast(long boast, CCharString questName);   // 0x00CE1C90
    void Main();
};

void CHeroBoastsScript::Main()
{
    while (!IsActiveThreadTerminating()) {
        {
            BoastVector boasts;
            GSI->MsgOnBoastsMade(boasts);
            for (unsigned int i = 0; i != boasts.size(); ++i) {
                if (IsActiveThreadTerminating())
                    return;
                AddSpawnedFunction(new CSpawnedFunc2<CHeroBoastsScript, long, CCharString>("ControlNewBoast", 0, this,
                                       &CHeroBoastsScript::ControlNewBoast, boasts[i].first, boasts[i].second),
                                   CCharString("", -1));
            }
            if (IsActiveThreadTerminating())
                return;
        }
        GSI->NewScriptFrame();
    }
}
}