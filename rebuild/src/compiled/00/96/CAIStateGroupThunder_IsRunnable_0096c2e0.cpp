// CAIStateGroup_Thunder::IsRunnable() -- when the creature's target (interface 8) is a creature: on a fresh hit, latch the "ThunderArenaCentre" thing and count the hit, and once the count reaches the def's threshold latch the target; otherwise latch the target when the creature's interface-0xAD value exceeds the def's limit
// Leaf: ?IsRunnable@CAIStateGroup_Thunder
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096C2E0, length 524 bytes (plain `ret`).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (non-const; interfaces 8 and 0xAD, inlined GetInterface ternary form),
//          interface 8 slot 29 (+0x74, the current target), CDefClassBase::GetSubDef @ 0x008E3240 (manifest
//          GetSubDef<CAICreatureWillPowerIndicatorDef> label is propagated: this def reads +0x3c/+0x40), FindHitEvent
//          @ 0x0096C250 (labelled CAIStateGroup_JackDragon), CCharString 0x0099EBF0/0x0099EC30/0x0099EAE0,
//          PeekThingSearchTools 0x008FD000, GetPFirstThing<HasScriptName> 0x0056D050, CBaseIntelligentPointer
//          0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10, CIntelligentPointer<const CThing>(const CThing*) 0x00668B60,
//          interface 0xAD's 0x007AED40 (manifest "CWorld::DrawGetEnvironment" is propagated).
// Shape: the const-thing pointer is a named local in its own block (an unnamed temporary lets VC use the ctor's eax
//          `this` and tail-merge the two latch exits); GetInterface uses a separate `long type` key local.
// Layout: Ego_r.pdb CAIStateGroup_Thunder (pack 1): PArenaCentre +0x14, bIsRangeAttack +0x1c, PTargetThing +0x1d, HitCount +0x28.
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
class CThingFilter_HasScriptName {
public:
    CCharString ScriptName;
    CThingFilter_HasScriptName(const CCharString& name) : ScriptName(name) { }
};
class CThing;
class CThingSearchTools {
public:
    template<class F, class T> bool GetPFirstThing(const F& filter, T** ppThing) const;   // 0x0056D050
};
void* Find_CThingSearchTools_GetPFirstThing(const void* tools, const void* filter);
#pragma auto_inline(off)
template<class F, class T> __declspec(noinline) bool CThingSearchTools::GetPFirstThing(const F& filter, T** ppThing) const
{
    void* thing = Find_CThingSearchTools_GetPFirstThing(this, &filter);
    if (!thing)
        return false;
    *ppThing = (T*)thing;
    return true;
}
#pragma auto_inline(on)
struct CDefVirtual {
    virtual ~CDefVirtual();
    virtual void Destroy();                            // slot 1
};
struct CThunderDef : CDefVirtual {
    long          RefCount;                            // +0x04
    unsigned char _pad08[0x3c - 0x08];
    long          HitsBeforeTarget;                    // +0x3c
    long          EnvironmentThreshold;                // +0x40
};
class CDefClassBase { public:
    template <class T> bool GetSubDef(T** out);        // 0x008E3240
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(T** out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    *out = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                   // +0x10 (2: creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x1c];
    CDefClassBase* m_pDef;                      // +0x70
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
struct CTCTargeting {                           // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetTarget();                // slot 29 (+0x74)
};
struct CTCEnvironmentValue {                    // interface 0xAD
    long GetValue();                            // 0x007AED40
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerThing : CBaseIntelligentPointer {          // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item);                // 0x00668B60
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {       // vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerConstThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000 (Ego_r.pdb ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ, protected)  // 0x008FD000
public:
};
#pragma pack(push, 1)
struct CAIStateGroup_Thunder : CAIStateGroupBase {
    CIntelligentPointerThing PArenaCentre;      // +0x14
    bool bIsRangeAttack;                        // +0x1c
    CIntelligentPointerCreature PTargetThing;   // +0x1d
    unsigned char _pad25[3];
    long HitCount;                              // +0x28
    bool FindHitEvent();                        // 0x0096C250
    virtual bool IsRunnable();
};
#pragma pack(pop)

bool CAIStateGroup_Thunder::IsRunnable()
{
    CThing* target;
    {
        CTCTargeting* targeting;
        PBrain->PCreature->GetInterface(&targeting, 8);
        target = targeting->GetTarget();
    }
    if (target == 0 || target->ThingClass != 2)
        return false;
    CThunderDef* def = 0;
    PBrain->PCreature->m_pDef->GetSubDef(&def);
    if (FindHitEvent()) {
        CThing* arena = 0;
        {
            CCharString name("ThunderArenaCentre");
            PeekThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName(name), &arena);
        }
        PArenaCentre = CIntelligentPointerThing(arena);
        if (++HitCount == def->HitsBeforeTarget) {
            {
                CIntelligentPointerConstThing targetPointer(target);
                PTargetThing = targetPointer;
            }
            if (--def->RefCount == 0)
                def->Destroy();
            return true;
        }
    }
    CTCEnvironmentValue* env;
    PBrain->PCreature->GetInterface(&env, 0xad);
    if (env->GetValue() > def->EnvironmentThreshold) {
        PTargetThing = CIntelligentPointerCreature(target);
        if (--def->RefCount == 0)
            def->Destroy();
        return true;
    }
    if (--def->RefCount == 0)
        def->Destroy();
    return false;
}