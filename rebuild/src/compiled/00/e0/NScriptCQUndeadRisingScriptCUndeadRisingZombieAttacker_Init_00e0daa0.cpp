// NScript::CQ_UndeadRisingScript::CUndeadRisingZombieAttacker::Init() -- spawn the parent script's LookAfterHealth thread on the entity's own thing
// Leaf: ?Init@CUndeadRisingZombieAttacker@CQ_UndeadRisingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00e0daa0, 258 bytes, thiscall, plain `ret` (virtual, entity vtable slot 2), no EH frame.
// Identity (proven): slot 2 (Init) of the entity vtable 0x012DFF64 that the landed NScript::CQ_UndeadRisingScript::CUndeadRisingZombieAttacker::Alloc 0x00E0CF20 stores; Ego_r.pdb / FableWin.pdb name ?Init@CUndeadRisingZombieAttacker@CQ_UndeadRisingScript@NScript@@EAEXXZ in the PDB sets; it spawns ?LookAfterHealth@CQ_UndeadRisingScript@NScript@@QAEXVCScriptThing@@@Z 0x00E0DBB0 (the member pointer stored at +0x34; landed) under the name "ParentClass." + "LookAfterHealth".
// Callees: operator new 0x00BFEA1A (0x48), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   operator+(const CCharString&, const CCharString&) 0x0099F570 (__fastcall), CSpawnedFuncBase(const CCharString&, unsigned
//   long) 0x00CDD450, CScriptThing(const CScriptThing&) 0x004ABE90 (the functor's member copy) / ~CScriptThing 0x004AA840,
//   CScriptBase::AddSpawnedFunction 0x00CB7E50. Data: CSpawnedFunc1<CQ_UndeadRisingScript, CScriptThing> vtable 0x012E0088,
//   "ParentClass." 0x012C9934, "LookAfterHealth" 0x012E00A4, "" 0x0122D70E.
// Shape (family L139): Ego_r.pdb CSpawnedFunc1<T, CScriptThing> (0x48: Func +0x34, PClass +0x38, Par1 +0x3c); the by-value
//   argument copy of Me is inline, the member copy inside the inlined functor constructor is the out-of-line 0x004ABE90
//   (`#pragma inline_depth(1)` around the function, the counted-pointer copy __forceinline); the name temporaries are
//   conditional (flags 7 in ebx); the region name is SpawnFunction's default CCharString argument, built after the
//   new-expression so ParentClass is re-read for the call.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& lhs, const CCharString& rhs);   // 0x0099F570
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
class CBaseClass { public:                              // vtable 0x01231710
    virtual void v00();
    CBaseClass() { }
};
struct CCountBlock { long References; };
struct CCountedPointer {
    void* Data;
    CCountBlock* Info;
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
};
class CScriptThing : public CBaseClass { public:        // 12 bytes, vtable 0x01238C8C
    virtual void v00();
    CCountedPointer PImp;
    CScriptThing(const CScriptThing& other) : CBaseClass(), PImp(other.PImp) { }
    virtual ~CScriptThing();                            // 0x004AA840
};
class CSpawnedFuncBase {                                // retail 0x34 bytes
public:
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T, class P1> class CSpawnedFunc1 : public CSpawnedFuncBase {
public:
    void (T::*Func)(P1);                                // +0x34
    T* PClass;                                          // +0x38
    P1 Par1;                                            // +0x3c
    CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1), P1 par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) {}
    virtual void SuspendableProcess() { (PClass->*Func)(Par1); }
};
class CScriptBase {
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void SpawnFunction(CSpawnedFuncBase* func, const CCharString& regionName = CCharString("")) { AddSpawnedFunction(func, regionName); }
};
class CGameScriptInterface;
namespace NScript {
class CQ_UndeadRisingScript : public CScriptBase {
public:
    void LookAfterHealth(CScriptThing thing);
    struct CUndeadRisingZombieAttacker {
        CGameScriptInterface* Game;
        CScriptThing Me;
        CQ_UndeadRisingScript* ParentClass;
        void* PMasterData;
        virtual void v0();
        virtual void Main();
    private:
        virtual void Init();
    };
};
#pragma inline_depth(1)
void CQ_UndeadRisingScript::CUndeadRisingZombieAttacker::Init()
{
    ParentClass->SpawnFunction(new CSpawnedFunc1<CQ_UndeadRisingScript, CScriptThing>(CCharString("ParentClass.") + CCharString("LookAfterHealth"), 0, ParentClass, &CQ_UndeadRisingScript::LookAfterHealth, Me));
}
}