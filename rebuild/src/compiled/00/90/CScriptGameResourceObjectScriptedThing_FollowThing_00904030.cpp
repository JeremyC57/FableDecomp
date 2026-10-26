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

struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};

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
};

class CScriptThing {                               // vtable 0x01238C8C
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CThing* GetPThing() const;      // slot 11 (+0x2c) 0x004AAA60
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