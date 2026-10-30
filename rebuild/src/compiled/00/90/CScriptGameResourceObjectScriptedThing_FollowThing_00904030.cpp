// CScriptGameResourceObjectScriptedThing::FollowThing(const CScriptThing& target, float distance, bool run) -- queue a follow-creature action on a live target for the live scripted thing
// Leaf: ?FollowThing@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@M_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904030, length 190 bytes (`ret 0Ch`), vtable 0x0128D86C slot 7.
// PDB: ?FollowThing@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@M_N@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CScriptThing::IsAlive (slot 75)
// and GetPThing (slot 11), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined
// GetInterface, interface 0x1f = CTCScriptedControl), operator new @ 0x00BFEA1A (0x28 bytes),
// NTCScriptedControl::CActionFollowCreature::CActionFollowCreature(CThing*, float, bool) @ 0x00905180 (vtable
// 0x0128D838), CTCScriptedControl::AddAction @ 0x007137D0.
// Shape: the family guard, then the unchecked GetInterface BEFORE the target thing is fetched (the
// control lands in ebx, the miss arm reads the dead `target` slot); the target thing is a named local
// read after the lookup and before operator new.
// Layout: engine/CScriptGameResourceObjectScriptedThing.h models the vptr as a data word, so it cannot declare this
// virtual member; the class is modelled locally with the header's offsets (ScriptInterface +0x10, PThing +0x14
// CIntelligentPointer<CThing>, ScriptID +0x1c, NullResource +0x20, ActionPriority +0x24; 0x28 bytes).
#include <stddef.h>

void* __cdecl operator new(size_t size);           // 0x00BFEA1A

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
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

class CTCScriptedControl { public:                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

namespace NTCScriptedControl {
struct CActionFollowCreature : CActionBase {                   // vtable 0x0128D838, 0x28 bytes
    unsigned char _body[0x24];
    CActionFollowCreature(CThing& creature, float distance, bool run);   // 0x00905180 (Ego_r ??0CActionFollowCreature@NTCScriptedControl@@QAE@AAVCThing@@M_N@Z)
};
}

class CGameScriptInterface;
class CScriptGameResourceObjectScriptedThing {     // vtable 0x0128D86C, 0x28 bytes
public:
    virtual ~CScriptGameResourceObjectScriptedThing();
    unsigned char               _pad_0x04[0xc];    // +0x04 CBaseObject / CScriptGameResourceObjectBase members
    CGameScriptInterface*       ScriptInterface;   // +0x10
    CIntelligentPointer<CThing> PThing;            // +0x14
    long                        ScriptID;          // +0x1c
    bool                        NullResource;      // +0x20
    long                        ActionPriority;    // +0x24
    virtual void FollowThing(const CScriptThing& target, float distance, bool run);   // slot 7
};

void CScriptGameResourceObjectScriptedThing::FollowThing(const CScriptThing& target, float distance, bool run)
{
    if (PThing.GetPItem()) {
        if (!PThing.GetPItem()->m_dead) {
            if (target.IsAlive()) {
                CTCScriptedControl* control;
                PThing.GetPItem()->GetInterface(&control, 0x1f);
                CThing* targetThing = target.GetPThing();
                control->AddAction(new NTCScriptedControl::CActionFollowCreature(*targetThing, distance, run));
            }
        }
    }
}