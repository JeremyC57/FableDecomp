// CEntityScriptBinding<NScript::CQ_EndGameFocalSitesScript>::CEntityScriptBinding(const CCharString& name, CQ_EndGameFocalSitesScript* parentScript, ClassAllocFunc allocFunc, unsigned long flags) -- the class-allocator binding constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E95390, length 53 bytes (64 with int3 padding).
// Leaf: ??0?$CEntityScriptBinding
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30. Vtable 0x012ECC98.
// Layout from the PDB (CEntityScriptBinding<NScript::CQ_EndGameFocalSitesScript>, 28 bytes): base
// CEntityScriptBindingBase (vptr only) +0, EntityScriptName +4, PParentScript +8, Func (member-function
// pointer) +0xc, ClassAllocFunc +0x10, ClassAllocValid +0x14, Flags +0x18 -- the same offsets the landed
// CreateActiveEntityScript bodies (0x00D82210 and siblings) read. Four arguments (`ret 0x10`), matching the
// manifest's parameter list; this is the overload that binds a class allocator, so Func is left untouched
// and ClassAllocValid is set to true.
// Shape: the derived vtable store sits between the name push and the copy-constructor call (the base has no
// constructor of its own, so only one vtable store is emitted); the three dword members are stored after the
// call in the scheduler's order (+0x18, +8, +0x10) and the bool last.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

class CScriptDataBase;
class CScriptThing;
class CEntityScriptBase;
template<class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
};

// The script class is single-inheritance, so its member-function pointer (Func) is one dword (PDB: Func +0xc,
// ClassAllocFunc +0x10); an incomplete forward declaration would give the 16-byte general form.
namespace NScript { struct __single_inheritance CQ_EndGameFocalSitesScript; }

class CEntityScriptBindingBase { public:
    virtual ~CEntityScriptBindingBase();
};

template<class ParentScript>
struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__cdecl *TClassAllocFunc)(ParentScript*, CScriptDataBase*, CScriptThing*);
    typedef void (ParentScript::*TFunc)(CScriptThing*);
    CCharString     EntityScriptName;   // +0x04
    ParentScript*   PParentScript;      // +0x08
    TFunc           Func;               // +0x0c
    TClassAllocFunc ClassAllocFunc;     // +0x10
    bool            ClassAllocValid;    // +0x14
    unsigned long   Flags;              // +0x18
    CEntityScriptBinding(const CCharString& name, ParentScript* parentScript, TClassAllocFunc allocFunc, unsigned long flags);
    virtual ~CEntityScriptBinding();
};

template<class ParentScript>
CEntityScriptBinding<ParentScript>::CEntityScriptBinding(const CCharString& name, ParentScript* parentScript, TClassAllocFunc allocFunc, unsigned long flags)
    : EntityScriptName(name), PParentScript(parentScript), ClassAllocFunc(allocFunc), ClassAllocValid(true), Flags(flags)
{
}

template struct CEntityScriptBinding<NScript::CQ_EndGameFocalSitesScript>;