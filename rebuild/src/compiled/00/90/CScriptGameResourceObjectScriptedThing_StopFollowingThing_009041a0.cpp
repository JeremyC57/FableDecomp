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

class CThing_Retail { public:
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

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
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (target.IsAlive()) {
                CTCScriptedControl* control;
                thing->GetPItem()->GetInterface(&control, 0x1f);
                if (control->CurrentActionType == 4) {
                    CCharString name(g_actionFollowCreature, -1);
                    control->TerminateActionByName(name);
                } else {
                    CThing_Retail* targetThing = ((CThing_Retail*)target.GetPThing());
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