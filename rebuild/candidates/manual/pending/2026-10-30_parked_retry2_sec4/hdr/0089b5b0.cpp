// CGameScriptInterface::StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& resource, EScriptAIPriority priority) -- refuse when the thing is already scripted at a higher priority, wait (yielding script frames) while its scripted-control component is busy, then hand the resource a new scripted-thing object for it; on failure give the resource a null scripted thing
// Grade: DIFFER(454v454) -- one register: retail reloads the count block into eax for operator delete after the fastcall deleter, ours into ecx
// Leaf: StartScriptingEntity
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089B5B0, 454 bytes, `ret 0xc`.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   the thing's script-priority accessor 0x006D5AD0, the component-map LowerBound 0x0040F020 (TC 0x1f scripted control),
//   CScriptGameResourceObjectScriptedThing ctor 0x009039D0 (via operator new 0x00BFEA1A), the resource's pointer setter
//   0x008AB960 and operator= 0x008ABD10, the null-object factory 0x00903810 (fastcall), CBaseObject dtor 0x0099A430,
//   operator delete 0x00BFE9BC; the script thing's virtual slots 11 (thing) and 75 (alive); own slot 7 (new script frame).
// Layout: local projections; the retail CThing keeps its component bitset at +0x20 and the component map at +0x44.
#include "engine/CCountedPointer.h"
#pragma pack(push, 1)
struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry* Start;
    ComponentEntry* Finish;
    ComponentEntry* LowerBound(const long& key);         // 0x0040F020
};
struct CThing {
    void*          VTable;
    unsigned char  Base[0xc];
    unsigned char  ScriptControlled;                     // +0x10
    unsigned char  Base2[0xf];
    unsigned long  UsingComponents[9];                   // +0x20
    ComponentMap   Components;                           // +0x44
    unsigned char  Metadata[0x45];
    unsigned char  LifeFlags;                            // +0x91
    long GetScriptPriority() const;                      // 0x006D5AD0
};
#pragma pack(pop)
struct CTCScriptedControl { unsigned char Data[0x18]; bool Busy; };
struct CBaseIntelligentPointer {
    void* Info;
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CThing* GetPItem() const;                            // 0x00A01B50
    void SetPItem(CThing* item);                         // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { SetPItem(item); }
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
struct CGameFlags { unsigned char Data[5]; bool Quitting; };
extern CGameFlags* g_pGameFlags;                         // 0x013D2838
struct CBaseObject {
    CBaseObject();
    virtual ~CBaseObject();                              // 0x0099A430
};

void operator delete(void* p);
struct CScriptGameResourceObjectScriptedThingImpl;
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0126008C
    long Spare;
    ~CScriptGameResourceObjectScriptedThingBase() { }
    void Set(CScriptGameResourceObjectScriptedThingImpl* p);    // 0x008AB960
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& o);   // 0x008ABD10
};
struct CScriptGameResourceObjectScriptedThing : CScriptGameResourceObjectScriptedThingBase {   // vtable 0x0127094C
    CCountedPointer<CScriptGameResourceObjectScriptedThingImpl> PImpl;
};
struct CGameScriptInterface;
struct CScriptGameResourceObjectScriptedThingImpl {
    unsigned char Data[0x28];
    CScriptGameResourceObjectScriptedThingImpl(CGameScriptInterface* gsi, CThing* thing, long owner, long priority);   // 0x009039D0
};
CScriptGameResourceObjectScriptedThing __fastcall MakeNullScriptedThing(CGameScriptInterface* gsi);   // 0x00903810
struct CGameScriptInterface {
    virtual void v000();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
    unsigned char Data[0x24];
    long          Owner;                                 // +0x28
    bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& resource, long priority);
};
template <class T, long TYPE> __forceinline T* GetComponent(CThing* thing)
{
    T* result;
    if (thing->UsingComponents[TYPE >> 5] & (1u << (TYPE & 31))) {
        ComponentMap* map = &thing->Components;
        ComponentEntry* it = map->LowerBound(TYPE);
        ComponentEntry* end = map->Finish;
        result = (T*)((it == end || it->Type > TYPE) ? end : it)->Component;
    }
    return result;
}

bool CGameScriptInterface::StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& resource, long priority)
{
    if (thing.IsAlive()) {
        CIntelligentPointer<CThing> p(thing.GetThing());
        if (p.GetPItem()->ScriptControlled == 1 && p.GetPItem()->GetScriptPriority() > priority)
            return false;
        CTCScriptedControl* control = GetComponent<CTCScriptedControl, 0x1f>(p.GetPItem());
        while (control->Busy) {
            if (!thing.IsAlive() || g_pGameFlags->Quitting)
                return false;
            NewScriptFrame();
        }
        if (p.GetPItem() && !(p.GetPItem()->LifeFlags & 1)) {
            resource.Set(new CScriptGameResourceObjectScriptedThingImpl(this, p.GetPItem(), Owner, priority));
            return true;
        }
    }
    resource = MakeNullScriptedThing(this);
    return false;
}
