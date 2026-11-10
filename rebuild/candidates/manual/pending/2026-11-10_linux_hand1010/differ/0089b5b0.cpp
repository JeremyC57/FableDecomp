// CGameScriptInterface::StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& resource, EScriptAIPriority priority) -- refuse when the thing is already scripted at a higher priority, wait (yielding script frames) while its scripted-control component is busy, then hand the resource a new scripted-thing object for it; on failure give the resource a null scripted thing
// Grade: DIFFER(ours v retail) -- 2 lines (probe-measured, local-model draft): the CreateNullHandle temporary's inline ~CCountedPointer reloads Info for operator delete into ecx (edx with the shared CBaseObject/CIntelligentPointer/CVectorMap headers), retail eax. Shared-header rebase blocked: engine/CScriptGameResourceObjectScriptedThing.h is a flat generated layout (no base class, no ctor / CreateNullHandle), and the CThing priority accessor needs CThingAICreature::GetCurrentStateGroupScriptPriority
// Leaf: ?StartScriptingEntity@CGameScriptInterface@@UAE_NABVCScriptThing@@AAVCScriptGameResourceObjectScriptedThingBase@@W4EScriptAIPriority@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089B5B0, 454 bytes, `ret 0xc`.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   the thing's script-priority accessor 0x006D5AD0, the component-map LowerBound 0x0040F020 (TC 0x1f scripted control),
//   CScriptGameResourceObjectScriptedThing ctor 0x009039D0 (via operator new 0x00BFEA1A), the resource's pointer setter
//   0x008AB960 and operator= 0x008ABD10, the null-object factory 0x00903810 (fastcall), CBaseObject dtor 0x0099A430,
//   operator delete 0x00BFE9BC; the script thing's virtual slots 11 (thing) and 75 (alive); own slot 7 (new script frame).
// Layout: local projections; the retail CThing keeps its component bitset at +0x20 and the component map at +0x44.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CTCBase { public: virtual ~CTCBase(); };
class CThing : public CBaseObject { public:
    unsigned char  _pad08[0x10 - 0x08];
    unsigned char  ScriptControlled;                     // +0x10
    unsigned char  _pad11[0x20 - 0x11];
    std::bitset<274> UsingComponents;                    // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char  _pad54[0x91 - 0x54];
    unsigned char  LifeFlags;                            // +0x91
    long GetCurrentStateGroupScriptPriority() const;                      // 0x006D5AD0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::StatGetInterfaceType()))
            return false;
        ETCInterfaceType key = T::StatGetInterfaceType();
        CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
        std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
        if (it == map->end() || T::StatGetInterfaceType() < it->first)
            it = map->end();
        *out = static_cast<T*>(it->second);
        return true;
    }
};
class CTCScriptedControl : public CTCBase { public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SCRIPTED_CONTROL; }
    unsigned char Data[0x18 - 4]; bool Busy;   // +0x18
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
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CGameScriptInterface;
enum EScriptAIPriority { };
class CScriptGameResourceObjectScriptedThing : public CScriptGameResourceObjectScriptedThingBase {
public:
    unsigned char Data[0x28 - 0x10];
    CScriptGameResourceObjectScriptedThing(CGameScriptInterface& scriptInterface, CThing& thing, long owner, EScriptAIPriority priority);   // 0x009039D0
    static CScriptGameResourceObjectScriptedThingBase __fastcall CreateNullHandle(CGameScriptInterface& scriptInterface);   // 0x00903810
};
class CGameScriptInterface { public:
    virtual void v000();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
    unsigned char Data[0x24];
    long          Owner;                                 // +0x28
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& resource, EScriptAIPriority priority);
};
bool CGameScriptInterface::StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& resource, EScriptAIPriority priority)
{
    if (thing.IsAlive()) {
        CIntelligentPointer<CThing> p(thing.GetThing());
        if (p.Get()->ScriptControlled == 1 && p.Get()->GetCurrentStateGroupScriptPriority() > priority)
            return false;
        CTCScriptedControl* control;
        p.Get()->GetTC(&control);
        while (control->Busy) {
            if (!thing.IsAlive() || g_pGameFlags->Quitting)
                return false;
            NewScriptFrame();
        }
        if (p.Get() && !(p.Get()->LifeFlags & 1)) {
            resource.SetImplementation(new CScriptGameResourceObjectScriptedThing(*this, *static_cast<CThing*>(p.GetPItem()), Owner, priority));
            return true;
        }
    }
    resource = CScriptGameResourceObjectScriptedThing::CreateNullHandle(*this);
    return false;
}
