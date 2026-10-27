// NScript::CProximityPodiumTestScript::Main() -- bind the "ProximityPodium" entity script (CProximityPodiumTestScript::CProximityPodium::Alloc), finish adding the scripted entities here and in the game interface
// Leaf: ?Main@CProximityPodiumTestScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFB1E0, 137 bytes, thiscall, plain `ret`.
// Identity (probable): slot 2 (Main) of the script vtable 0x012F6AD8 (stored by the landed CProximityPodiumTestScript constructor); no PDB entry (test script); the
//   binding's allocator 0x00EFB280 is the landed CProximityPodiumTestScript::CProximityPodium::Alloc and its name literal 0x012F6B10 is "ProximityPodium".
// Callees: scalar operator new 0x00BFEA1A (0x1C = sizeof(CEntityScriptBinding<CProximityPodiumTestScript>)), CCharString(const char*, long)
//   0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding
//   0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930, CGameScriptInterface::PostAddScriptedEntities (slot 64,
//   +0x100); the binding vtable 0x012F6B08.
// Shape (family K071, representative 0x00E96F10): the landed CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20 binding
//   idiom with flags 0: the conditional name temporary of the new-expression is tracked in ebx and destroyed after the call.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };
class __declspec(novtable) CEntityScriptBindingBase {
public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> class CEntityScriptBinding : public CEntityScriptBindingBase {
public:
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
class CGameScriptInterface {                            // slots: ghidra_out/cgsi_slots_aligned.tsv
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities() const;       // slot 64 (+0x100)
};
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
protected:
    virtual void RegisterMain();
    virtual void Main();                                // slot 2
    virtual void Init();
public:
    virtual void OnPersist(CPersistContext& context);
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);   // 0x00CB8230
    void PostAddScriptedEntities();                     // 0x00CB8930
};
class CLevelScriptBase : public CScriptBase {           // PDB CLevelScriptBase<TData>
public:
    CGameScriptInterface* Game;                         // +0x40
    CScriptDataBase* MasterScriptData;                  // +0x44
};
namespace NScript {
class CProximityPodiumTestScript : public CLevelScriptBase {
public:
    class CProximityPodium { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CProximityPodiumTestScript& parent, CScriptDataBase* data, const CScriptThing& me); };   // 0x00EFB280
    virtual void Main();
};
void CProximityPodiumTestScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CProximityPodiumTestScript>("ProximityPodium", this, &CProximityPodium::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
}
}