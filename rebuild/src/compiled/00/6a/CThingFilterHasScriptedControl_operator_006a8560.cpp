// CThingFilter_HasScriptedControl::operator()(CThing* thing) const -- a script thing with a scripted-control component that, when CheckForActive is set, is also active (locked or with queued script tasks)
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A8560, 99 bytes (`ret 4`).
// Callees: GFHasScriptThing(CThing*) @ 0x004C7E70 (`__fastcall`; label "operator_new" propagated, landed),
//   CInterfaceMap::LowerBound @ 0x004365B0, CTCScriptedControl's task-list empty test @ 0x007122C0 (auto label
//   "CScriptTask_IsRunning"; landed callers call it IsTaskListEmpty / AreTasksEmpty).
// Layout: CThingFilter_HasScriptedControl (Ego_r.pdb 1 byte) = bool CheckForActive; retail CThing: interface bitset +0x20
//   (TC 0x1f = CTCScriptedControl: the sign bit of word +0x20), CVectorMap +0x44; CTCScriptedControl Locked +0x18.
// Shape: the agent160 GetTC form with the lookup key in the dead thing-argument slot (no frame: the function has no
//   named locals); CTCScriptedControl::IsActive inline (`Locked || !IsTaskListEmpty()`).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCScriptedControl { public:
    unsigned char _pad[0x18];
    bool Locked;                                // +0x18
    bool IsTaskListEmpty();                     // 0x007122C0
    bool IsActive() { return Locked || !IsTaskListEmpty(); }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};
bool __fastcall GFHasScriptThing(CThing* thing);   // 0x004C7E70

class CThingFilter_HasScriptedControl { public:
    bool CheckForActive;                        // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_HasScriptedControl::operator()(CThing* thing) const
{
    if (GFHasScriptThing(thing) && thing->HasTC(0x1f)) {
        if (!CheckForActive || thing->GetTC<CTCScriptedControl>(0x1f)->IsActive())
            return true;
    }
    return false;
}