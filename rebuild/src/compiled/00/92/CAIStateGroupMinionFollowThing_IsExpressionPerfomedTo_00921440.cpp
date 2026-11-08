// CAIStateGroup_MinionFollowThing::IsExpressionPerfomedTo(const CCharString& expression) -- true (and the event consumed) when an expression-performed-to event (type 3) naming this expression is pending and the scratchpad allows following; EXPRESSION_FOLLOW retargets the followee, the event's extra data overrides the follow distance and the obstacle flag
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z; 004365b0: ?LowerBound@CInterfaceMap@@QBEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00921440, length 405 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x42 = CTCEntityEvents, bit 2 of +0x28), its const
// twin @ 0x004365B0 (interface 0xAA = CTCAIScratchpad, bit 10 of +0x34, through a const thing; key in the dead argument
// slot), CTCEntityEvents::GetEvent<CEventExpressionPerformedTo, CIsEventOfType> @ 0x00922440 (manifest names the
// CEventChildHurtGoToMother instantiation: propagated), the expression def's virtual slot 10 (name by value),
// CDefString::operator==(const CCharString&) @ 0x004B7980, CTCEntityEvents::RemoveEvent @ 0x008112D0 (manifest
// "remove_if<..CIsType>" is propagated), CBasicString<char>::operator==(const char*) @ 0x004115A0 (inline CCharString ==
// "EXPRESSION_FOLLOW", strcmp intrinsic on the empty string), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CIntelligentPointer<const CThing> ctor @ 0x00668B60, SetPItem @ 0x00A01B90, dtor @ 0x00A01C10.
// Layout: no engine header for this class; members from Ego_r.pdb (PFollowee +0x14, FollowDistance +0x1c,
// AvoidDynamicObstacles +0x20; CEventExpressionPerformedTo PerformedBy +8, PExpressionDef +0x10, ExtraData1/2 +0x18/+0x1c;
// CTCAIScratchpad AllowedToFollow +0x6c).
// Shape: the def is fetched through a by-value getter (the add-ref copy lives in esi, its CDefString name result reuses
// the filter temporary's slot); the scratchpad lookup is the const out-parameter GetInterface (key in the dead argument
// slot); `== "EXPRESSION_FOLLOW"` is the inline CCharString compare with `strcmp(text, "")` on the null block.
#include "fable_compiler.h"
#include <string.h>

class CTCBase;
class CThing;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    FABLE_FORCEINLINE unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        return (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
    }
};

#pragma pack(push, 1)
template <class T> class CBasicString { public:                       // CBasicString<char>
    char* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public: CBasicString<char> Data; long NoRefs; };
#pragma pack(pop)

class CCharString { public:
    CCharStringData* PStringData;
    FABLE_FORCEINLINE bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

class CDefString { public:
    long Index;
    CDefString(const CDefString& other) : Index(other.Index) { }
    bool operator==(const CCharString& name) const;     // 0x004B7980
};

class CExpressionDef { public:
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetName() const;         // slot 10 (+0x28)
    long RefCount;                              // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DeleteThis(); }
};

struct CDefPointerExpression {
    CExpressionDef* m_p;
    CDefPointerExpression(const CDefPointerExpression& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CDefPointerExpression() { m_p->Release(); }
    CExpressionDef* operator->() const { return m_p; }
};

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
    CIntelligentPointerConstThing(const CThing* thing);     // 0x00668B60
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    FABLE_FORCEINLINE CIntelligentPointerCreature& operator=(const CIntelligentPointerConstThing& other)
    {
        SetPItem((CBaseObject*)(((CThing*)other.GetPItem())));
        return *this;
    }
};

struct CEventExpressionPerformedTo {
    unsigned char _pad[8];
    CIntelligentPointerCreature PerformedBy;    // +0x08
    CDefPointerExpression PExpressionDef;       // +0x10
    long DurationInFrames;                      // +0x14
    long ExtraData1;                            // +0x18
    long ExtraData2;                            // +0x1c
    CDefPointerExpression GetPExpressionDef() const { return PExpressionDef; }
};

class CIsEventOfType { public:
    long Type;
    CIsEventOfType(long type) : Type(type) { }
};

class CTCEntityEvents { public:                        // interface 0x42
    template<class T, class F> bool GetEvent(T** out, const F* filter);
    void RemoveEvent(void* event);              // 0x008112D0
};

class CTCAIScratchpad { public:                        // interface 0xAA
    unsigned char _pad[0x6c];
    bool AllowedToFollow;                       // +0x6c
};

long TmplSink(const void* self, long key);      // stand-in: the callee's own work (outside the graded bytes)
template<> __declspec(noinline) bool CTCEntityEvents::GetEvent<CEventExpressionPerformedTo, CIsEventOfType>(
        CEventExpressionPerformedTo** out, const CIsEventOfType* filter)
{
    *out = (CEventExpressionPerformedTo*)TmplSink(*out, filter->Type);
    return *out != 0;
}
#pragma auto_inline(on)

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_MinionFollowThing : CAIStateGroupBase {
    CIntelligentPointerCreature PFollowee;      // +0x14
    float FollowDistance;                       // +0x1c
    bool AvoidDynamicObstacles;                 // +0x20
    bool PreviousPushableByHero;                // +0x21
    bool ExpressionUsed;                        // +0x22
    bool FollowerAdded;                         // +0x23
    long CurrentMovementType;                   // +0x24
    bool IsExpressionPerfomedTo(const CCharString& expression);
};

bool CAIStateGroup_MinionFollowThing::IsExpressionPerfomedTo(const CCharString& expression)
{
    CTCEntityEvents* events;
    if (PBrain->PCreature->GetInterface(&events, 0x42)) {
        CEventExpressionPerformedTo* event = 0;
        if (events->GetEvent(&event, &CIsEventOfType(3))) {
            if (event->GetPExpressionDef()->GetName() == expression) {
                const CThing* creature = PBrain->PCreature;
                CTCAIScratchpad* scratchpad;
                if (!creature->GetInterface(&scratchpad, 0xaa) || scratchpad->AllowedToFollow) {
                    if (expression == "EXPRESSION_FOLLOW")
                        PFollowee = ((CThing*)event->PerformedBy.GetPItem());
                    if (event->ExtraData1 > -1)
                        FollowDistance = (float)event->ExtraData1;
                    if (event->ExtraData2 > -1)
                        AvoidDynamicObstacles = event->ExtraData2 != 0;
                    events->RemoveEvent(event);
                    return true;
                }
                events->RemoveEvent(event);
            }
        }
    }
    return false;
}