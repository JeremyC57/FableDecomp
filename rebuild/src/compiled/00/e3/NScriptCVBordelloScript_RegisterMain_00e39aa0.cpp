// NScript::CV_BordelloScript::RegisterMain() -- spawn the script's "Main" thread: AddSpawnedFunction(new CSpawnedFunc<CV_BordelloScript>("Main", 0, this, &CV_BordelloScript::Main), "")
// Leaf: ?RegisterMain@CV_BordelloScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E39AA0, 144 bytes, plain `ret`. Slot 1 (RegisterMain) of the script vtable @ 0x012E38B0.
// Identity: the vtable is stored by the script's Alloc, which CScriptInfoManager::RegisterScripts (0x00CD52D0) registers under
//   the name "V_Bordello" (site 0x00CD8539); script classes are C<registered name>Script in NScript (the PDB name sets hold
//   ?RegisterMain@CV_BordelloScript@NScript@@UAEXXZ). Family F001 (163 byte-identical bodies) -- the manifest label of every member was
//   propagated from one body; this one is CV_BordelloScript's.
// Callees: scalar operator new @ 0x00BFEA1A (0x3c bytes), CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0,
//   CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long) @ 0x00CDD450, CScriptBase::AddSpawnedFunction @ 0x00CB7E50.
// Data: CSpawnedFunc<CV_BordelloScript> vtable @ 0x012E38C4; the thread name literal "Main" @ 0x012C2F94; the empty region name @ 0x0122D70E;
//   the member pointer is the vcall thunk ??_9@$B7AE @ 0x00CDD440 (`mov eax,[ecx]; jmp [eax+8]`: Main is virtual slot 2;
//   ICF folded every class's thunk into one body).
// Layout: CSpawnedFuncBase is 0x34 bytes (vptr + 0x30), CSpawnedFunc<T> adds the member pointer Func +0x34 and PClass +0x38
//   (0x3c); CScriptBase is retail 0x40 bytes with the virtuals ~CScriptBase, RegisterMain, Main, Init, OnPersist (PDB order;
//   the retail vtable has exactly these five slots).
// Shape: the new-expression's CCharString temporary is conditional (built only when new succeeded), so VC7.1 keeps a flag
//   in ebx and destroys the temporary at the end only if bit 0 is set; the region name is the CCharString built by the inline
//   SpawnFunction helper after the new-expression (arguments of AddSpawnedFunction would otherwise build it first).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
class CSpawnedFuncBase {                                // retail 0x34 bytes
public:
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                          // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T> class CSpawnedFunc : public CSpawnedFuncBase {
public:
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
protected:
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
public:
    virtual void OnPersist(CPersistContext& context);
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CV_BordelloScript : public CScriptBase {
public:
    virtual void RegisterMain();
    virtual void Main();
};
void CV_BordelloScript::RegisterMain()
{
    SpawnFunction(new CSpawnedFunc<CV_BordelloScript>("Main", 0, this, &CV_BordelloScript::Main));
}
}