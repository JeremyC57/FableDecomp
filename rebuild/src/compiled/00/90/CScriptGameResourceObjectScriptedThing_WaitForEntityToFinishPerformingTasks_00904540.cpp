// CScriptGameResourceObjectScriptedThing::WaitForEntityToFinishPerformingTasks(const CScriptGameResourceObjectScriptedThingBase&) -- queue a wait-for-thing-to-finish-tasks action on the scripted thing for a live entity
// Leaf: ?WaitForEntityToFinishPerformingTasks@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptGameResourceObjectScriptedThingBase@@@Z
// Retail 0x00904540, 346 bytes, ret 4. Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PThing +0x14), the entity's
// CScriptGameResourceObjectScriptedThingBase::GetScriptThing (slot 12, +0x30, by value), CScriptThing::IsAlive (slot 75) /
// GetPThing (slot 11), CVectorMap::LowerBound 0x0040F020 (inline GetInterface 0x1F = CTCScriptedControl), operator new
// 0x00BFEA1A (0x18 bytes), CBase ctor 0x0099A2F0 / dtor 0x0099A2E0, CThing::GetScriptThing 0x004C7E50, operator delete
// 0x00BFE9BC, CTCScriptedControl::AddAction 0x007137D0, ~CScriptThing 0x004AA840. Vtables: CScriptThing 0x01238C8C,
// NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks 0x0128D7F0 (rtti-vtables.tsv).
// Layout: engine/CScriptGameResourceObjectScriptedThing.h (PThing +0x14) through a method view, as the landed
// DropGenericBox 0x00904E20; the entity parameter is a vtable view (engine/CScriptGameResourceObjectScriptedThingBase.h
// has no virtual slots).
// Shape: one `&&` condition whose third term is a by-value CScriptThing temporary (destroy flag bit 0, its dtor inline)
// and a new-expression whose constructor argument is a second temporary (flag bit 1, dtor out of line 0x004AA840) --
// VC7.1 makes that inline/out-of-line split by itself. GetInterface is the plain bitset test (`test ecx,ecx ; jns`
// for type 0x1F); the HasTC `const unsigned char&` form of MoveToAndPickUpGenericBox gives `shr/and/mov` here.
#include <stddef.h>
#include "engine/CScriptGameResourceObjectScriptedThing.h"

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

class CScriptThing;
class CThing_Retail { public:
    CScriptThing GetScriptThing() const;          // 0x004C7E50
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
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCPPointerInfo { public:                             // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
void operator delete(void* p);                     // 0x00BFE9BC
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBase {
    CBase();                                       // 0x0099A2F0
    virtual ~CBase();                              // 0x0099A2E0
};
class CScriptThing : public CBase {                // vtable 0x01238C8C, 12 bytes
public:
    CCountedPointer<CScriptThing> PImp;            // +0x04
    ~CScriptThing() { } virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
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
struct CActionBase : CBase {                       // vtable 0x01265504
    long m_actionState;                            // +0x04
    bool m_started;                                // +0x08
    bool m_finished;                               // +0x09
    CActionBase() : m_actionState(0), m_started(false), m_finished(false) { }
};
struct CActionWaitForThingToFinishPerformingTasks : CActionBase {   // vtable 0x0128D7F0, 0x18 bytes
    CScriptThing Target;                           // +0x0c
    CActionWaitForThingToFinishPerformingTasks(CThing_Retail* target) : Target(target->GetScriptThing()) { }
    virtual void v01();
};
}
class CTCScriptedControl { public:                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};
struct CScriptGameResourceObjectScriptedThingBase_View {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CScriptThing GetScriptThing() const;   // slot 12 (+0x30)
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void WaitForEntityToFinishPerformingTasks(const CScriptGameResourceObjectScriptedThingBase_View& entity);
};

void CScriptGameResourceObjectScriptedThing_Methods::WaitForEntityToFinishPerformingTasks(const CScriptGameResourceObjectScriptedThingBase_View& entity)
{
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
    if (thing->GetPItem() && !thing->GetPItem()->m_dead && entity.GetScriptThing().IsAlive()) {
        CTCScriptedControl* control;
        thing->GetPItem()->GetInterface(&control, 0x1f);
        control->AddAction(new NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks(entity.GetScriptThing().GetPThing()));
    }
}