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
class CGameScriptInterfaceBase { public:                // the script interface (slots: ghidra_out/cgsi_slots_aligned.tsv, CGameScriptInterface vtable 0x01260F0C)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& name) const;   // slot 12 (+0x30)
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036();
    virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities() const;   // slot 64 (+0x100)
};
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};

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