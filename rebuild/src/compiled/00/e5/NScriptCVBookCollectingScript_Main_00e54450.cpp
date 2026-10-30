// NScript::CV_BookCollectingScript::Main() -- the book-collecting quest main: wait for BowerstoneSlums to load, bind the BS_Teacher entity script and post the scripted entities to the script and to the game
// Leaf: ?Main@CV_BookCollectingScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E54450, 282 bytes, thiscall, plain `ret`.
// Identity (proven): the row is referenced once, from slot 2 (Main) of the RTTI-less level-script vtable 0x012E5EFC, which the
//   landed NScript::CV_BookCollectingScript ctor 0x00E54310 stores (slot 1 the landed RegisterMain 0x00E543B0, slot 4 OnPersist
//   0x00E54880); Ego_r.pdb ?Main@CV_BookCollectingScript@NScript@@UAEXXZ (0x00EAAB70, after RegisterMain 0x00EAAA40). The
//   factory it binds is the landed ?Alloc@CBS_Teacher@CV_BookCollectingScript@NScript@@SI...@Z 0x00E54580 ("BS_Teacher").
//   Catalog label sub_00e54450.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930
//   (Ego_r spellings); CGameScriptInterface slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities.
// Data: the binding vtable 0x012E5F2C (CEntityScriptBinding<CV_BookCollectingScript>, emitted inline).
// Literals: "BowerstoneSlums" 0x012CB55C, "BS_Teacher" 0x012E5F34.
// Layout: the landed ctor 0x00E54310 (Game +0x40, PMasterData +0x44); no quest member is touched here.
// Shape: the landed CV_RockTrollFirstEncounterScript::Main 0x00EC3D40 model (CEntityScriptBinding with its inline ctor, the
//   name temporary's conditional-destruction flag word); here the region wait comes before the binding.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
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
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) { }
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BookCollectingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CBS_Teacher {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BookCollectingScript& parent, CScriptDataBase* data, const CScriptThing& me);   // 0x00E54580
    };
    virtual void Main();
};

void CV_BookCollectingScript::Main()
{
    while (!Game->IsRegionLoaded("BowerstoneSlums")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BookCollectingScript>("BS_Teacher", this, &CBS_Teacher::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
}
}   // namespace NScript