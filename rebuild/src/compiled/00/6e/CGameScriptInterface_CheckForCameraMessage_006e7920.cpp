// CGameScriptInterface::CheckForCameraMessage(const CCharString& message) const -- whether the most recent camera
// message event (type 0x2B) targets a live thing whose scripted camera spline (TC 0x3F) carries this message; a
// match consumes the event
// Leaf: ?CheckForCameraMessage@CGameScriptInterface@@UBE_NABVCCharString@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E7920, length 192 bytes, `ret 4`.
// Callees: CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType> @ 0x004AB8B0 (CWorld+0x60),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the TC map lower bound @ 0x0040F020,
// CTCCameraPointScriptedSpline::HasMessage(const CCharString&) @ 0x0075D2B0, CMessageEventManager::RemoveMessage
// @ 0x00493B5B.
// Layout: CGameScriptInterface World +0x04; CWorld message manager +0x60; CMessageEvent PTarget +0x48
// (CIntelligentPointer<CThing>); CThing ThingTypeFlags +0x6C, TC bitset +0x20 (TC 0x3F = +0x24 bit 31), TC map
// +0x44, dead bit +0x91 bit 0.
// Shape: the event's target comes through the intelligent pointer's typed getter -- GetPItem() null test, then the
// THING_TYPE test of IsThingOfType<CThing> whose zero mask is copied into a local (VC7.1 folds `flags & 0` late and
// keeps the dead store of the item plus `xor al,al ; jne`, as in the landed 0x008ABDF0), then GetPItem() again; the
// filter type and the TC key share the `push ecx` frame word; `HasMessage(..) == true` gives `cmp al,1`; the world's
// message manager is reached through its CCountedPointer member's inline operator-> (a plain pointer member rotates
// eax/ecx/edx through the whole body).

#include "fable_compiler.h"
class CCharString;

class CTCBase;
// agentNF16: the stand-in CInterfaceMap_Retail struct is the explicit specialisation of CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom); 0x0040F020 is its LowerBound (NF14 / established spelling).
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap_Retail;
class CThing { public:
    enum { ThingTypeMask = 0 };
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];       // +0x20 std::bitset of the interface types present
    CInterfaceMap_Retail m_interfaces;                       // +0x44
    unsigned char _pad4c[0x20];
    unsigned long ThingTypeFlags;         // +0x6c
    unsigned char _pad70[0x21];
    unsigned char m_dead : 1;             // +0x91 bit 0
    unsigned char m_91_1 : 7;
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::value_type* it = map->LowerBound(type);
            if (it == map->_M_finish || interfaceType < it->first)
                it = map->_M_finish;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

template <class T> inline bool IsThingOfType(const CThing* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}

class CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPTypedItem() const
    {
        return (GetPItem() != 0 && IsThingOfType<T>((T*)GetPItem())) ? (T*)GetPItem() : 0;
    }
};

class CMessageEvent { public:
    unsigned char _pad[0x48];
    CIntelligentPointer<CThing> PTarget;            // +0x48
};
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) {}
};
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter);   // 0x004AB8B0
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
#include "engine/CCountedPointer.h"
class CWorld { public:
    virtual void w000();
    unsigned char _pad04[0x5c];
    CCountedPointer<CMessageEventManager> PMessageEventManager;    // +0x60
};

class CTCCameraPointScriptedSpline { public:
    bool HasMessage(const CCharString& message);    // 0x0075D2B0
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::CheckForCameraMessage(const CCharString& message) const
{
    CMessageEvent* event = World->PMessageEventManager->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x2b));
    if (event) {
        CThing* thing = event->PTarget.GetPTypedItem();
        CTCCameraPointScriptedSpline* spline;
        if (thing && !thing->m_dead && thing->GetInterface(&spline, 0x3f)) {
            if (spline->HasMessage(message) == true) {
                World->PMessageEventManager->RemoveMessage(event);
                return true;
            }
        }
    }
    return false;
}