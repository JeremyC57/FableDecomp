// CScriptGameResourceObjectScriptedThing::StopFollowingThing(const CScriptThing&) -- end a running follow action by name, or else make the scripted thing perform EXPRESSION_WAIT at the target
// Leaf: StopFollowingThing
// Retail 0x009041A0, 363 bytes, ret 4. Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PThing +0x14), the
// target's CScriptThing::IsAlive (slot 75) / GetPThing (slot 11), CVectorMap::LowerBound 0x0040F020 (GetInterface 0x1F
// CTCScriptedControl, 0x42 CTCEntityEvents), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0
// ("CActionFollowCreature" 0x0128D850, "EXPRESSION_WAIT" 0x012757DC), CTCScriptedControl::TerminateActionByName
// 0x00712CE0, CGameDefinitionManager::Get 0x0044C6B0 (__fastcall) and GetDef<T> 0x00593666, operator new 0x00BFEA1A,
// NEntityEvents::CEventExpressionPerformedTo ctor 0x0084B010 (landed), CTCEntityEvents::AddEvent 0x008113C0.
// Layout: engine/CScriptGameResourceObjectScriptedThing.h (PThing +0x14) through the landed PerformExpression 0x00904940
// method view; CTCScriptedControl's current action type at +0x1C (4 = following).
// Shape: if/else arms after the unchecked control lookup (a plain early return mis-packs the frame); the def is a
// CDefPointer released with DecRefToZero. Same-TU auto_inline(off) stand-ins for LowerBound, GetDef<T> (the landed
// GetDef 0x0083B000 shape) and the event ctor (reads the def only) tell VC7.1 the def never escapes, so the release
// keeps it in ecx (`dec [ecx+4] ; jne`) instead of reloading it -- all three are needed.
#include <stddef.h>
#include "engine/CScriptGameResourceObjectScriptedThing.h"

void* __cdecl operator new(size_t size);           // 0x00BFEA1A

class CTCBase;

struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (STLport lower_bound over the 8-byte {type, component} entries).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x20];
    unsigned long  ThingTypeFlags;                 // +0x6c
    unsigned char  _pad70[0x21];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CIntelligentPointer_Retail {                // CIntelligentPointer<CThing>
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 CBaseIntelligentPointer::GetPItem
};

class CScriptThing {                               // vtable 0x01238C8C
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CThing_Retail* GetPThing() const;      // slot 11 (+0x2c) 0x004AAA60
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                  // slot 75 (+0x12c) 0x004AB130
};

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
    virtual void v00();
};
}

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
class CTCScriptedControl { public:                        // interface 0x1f component
    unsigned char _pad00[0x1c];
    long          CurrentActionType;               // +0x1c (4 = following)
    bool TerminateActionByName(const CCharString& name);   // 0x00712CE0
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CExpressionDef { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long RefCount;                                 // +0x04
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
    CDefPointer<CExpressionDef> GetPDefFromInstantiationName(const CCharString& name);   // 0x009ADA40 (hidden result pointer)
    bool GetDef(const CCharString& name, CDefPointer<CExpressionDef>* out);   // 0x00593666 GetDef<T>
};
#pragma auto_inline(off)
// Same-TU stand-in for CDefinitionManager::GetDef<T> 0x00593666 (the landed GetDef 0x0083B000 shape): VC7.1 then knows
// the def pointer's address does not escape, so the release keeps the def in ecx (`dec [ecx+4] ; jne`).
bool CGameDefinitionManager::GetDef(const CCharString& name, CDefPointer<CExpressionDef>* out)
{
    CDefPointer<CExpressionDef> def = GetPDefFromInstantiationName(name);
    if (def.Object) {
        *out = def;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
namespace NEntityEvents {
class CEventExpressionPerformedTo { public:               // 0x20 bytes
    void*           __vftable;                     // +0x00
    long            m_eventType;                   // +0x04
    void*           m_targetVtable;                // +0x08 CIntelligentPointer
    void*           m_target;                      // +0x0c
    CExpressionDef* m_def;                         // +0x10
    long            m_expression;                  // +0x14
    long            m_a;                           // +0x18
    long            m_b;                           // +0x1c
    CEventExpressionPerformedTo(void* target, long expression, const CDefPointer<CExpressionDef>& def, long a, long b);   // 0x0084B010
};
#pragma auto_inline(off)
// Same-TU stand-in for the event constructor 0x0084B010 (landed): it only READS the def pointer (copy + add-ref), so
// VC7.1 knows the caller's def local is not changed by the call.
CEventExpressionPerformedTo::CEventExpressionPerformedTo(void* target, long expression, const CDefPointer<CExpressionDef>& def, long a, long b)
{
    m_eventType = 0;
    m_target = target;
    m_def = def.Object;
    if (m_def)
        ++m_def->RefCount;
    m_expression = expression;
    m_a = a;
    m_b = b;
}
#pragma auto_inline(on)
}
class CTCEntityEvents { public:                           // interface 0x42 component
    void AddEvent(NEntityEvents::CEventExpressionPerformedTo* event);   // 0x008113C0
};
extern const char g_actionFollowCreature[];        // 0x0128D850 "CActionFollowCreature"
extern const char g_expressionWait[];              // 0x012757DC "EXPRESSION_WAIT"

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void StopFollowingThing(const CScriptThing& target);
};

void CScriptGameResourceObjectScriptedThing_Methods::StopFollowingThing(const CScriptThing& target)
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (target.IsAlive()) {
                CTCScriptedControl* control;
                thing->GetPItem()->GetInterface(&control, 0x1f);
                if (control->CurrentActionType == 4) {
                    CCharString name(g_actionFollowCreature, -1);
                    control->TerminateActionByName(name);
                } else {
                    CThing_Retail* targetThing = target.GetPThing();
                    CThing_Retail* me = thing->GetPItem();
                    CDefPointer<CExpressionDef> def;
                    {
                        CCharString name(g_expressionWait, -1);
                        CGameDefinitionManager::Get().GetDef(name, &def);
                    }
                    CTCEntityEvents* events;
                    if (me->GetInterface(&events, 0x42))
                        events->AddEvent(new NEntityEvents::CEventExpressionPerformedTo(targetThing, 0, def, -1, -1));
                }
            }
        }
    }
}