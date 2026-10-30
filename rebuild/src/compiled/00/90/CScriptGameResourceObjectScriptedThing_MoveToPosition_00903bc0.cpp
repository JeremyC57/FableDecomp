// CScriptGameResourceObjectScriptedThing::MoveToPosition(const C3DVector& position, float proximity, EScriptEntityMoveType moveType, bool waitUntilDone, bool flag) -- queue a move-to-position action for the live scripted thing and optionally wait for its tasks
// Grade: RELOCATION_MATCH -- the presence bit is stored to a never-reloaded frame byte (`shr ecx,1Fh ; and cl,1 ; mov [esp+0Fh],cl`) because, as in the landed sibling MoveToThing 0x00903E20, the inline GetInterface binds the byte HasTC result to a const reference (a temporary with a frame home); a plain bool local folds into `test cl,1`
// Leaf: MoveToPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903BC0, length 220 bytes (`ret 14h`), vtable 0x0128D86C slot 4.
// PDB: ?MoveToPosition@CScriptGameResourceObjectScriptedThing@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x1f = CTCScriptedControl),
// operator new @ 0x00BFEA1A (0x50 bytes), NTCScriptedControl::CActionMoveTo::CActionMoveTo(const C3DVector&,
// EMovementType, float, bool, bool) @ 0x007E71D0 (vtable 0x0127091C), CTCScriptedControl::AddAction @ 0x007137D0,
// this's virtual WaitWhilePerformingTasks (slot 11, +0x2c).
// Shape: the family guard and the unchecked GetInterface; the script move type maps to the engine movement
// type through a switch (0 -> 3, 1 -> 5, default 1; default arm laid out first); the action is always built
// with FaceMovement = true. A virtual call on `this` needs the polymorphic model: engine/
// CScriptGameResourceObjectScriptedThing.h keeps the vptr as a plain word, so this file models the class
// locally with the header's offsets (ScriptInterface +0x10, PThing +0x14).
#include <stddef.h>



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
    unsigned char HasTC(long interfaceType) const { return (unsigned char)((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1); }
    template<class T> __forceinline bool GetInterfaceBit(T** ppOut, long interfaceType)
    {
        const unsigned char& present = HasTC(interfaceType);
        if (present) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
        }
        return present != 0;
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

class CTCScriptedControl { public:                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

class C3DVector { public: float X, Y, Z; };

enum EScriptEntityMoveType { SCRIPT_MOVE_TYPE_0 = 0, SCRIPT_MOVE_TYPE_1 = 1 };
enum EMovementType { MOVEMENT_TYPE_1 = 1, MOVEMENT_TYPE_3 = 3, MOVEMENT_TYPE_5 = 5 };

namespace NTCScriptedControl {
struct CActionMoveTo : CActionBase {                           // vtable 0x0127091C, 0x50 bytes
    unsigned char _body[0x4c];
    CActionMoveTo(const C3DVector& position, EMovementType movementType, float proximity, bool faceMovement, bool flag);   // 0x007E71D0
};
}

class CScriptGameResourceObjectScriptedThingLocal {            // vtable 0x0128D86C (engine header offsets)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void WaitWhilePerformingTasks();                    // slot 11 (+0x2c)
    void MoveToPosition(const C3DVector& position, float proximity, EScriptEntityMoveType moveType, bool waitUntilDone, bool flag);

    void*                      IntelligentPointer;              // +0x04 (CBaseObject)
    void*                      PImp_Data;                       // +0x08
    void*                      PImp_Info;                       // +0x0c
    void*                      ScriptInterface;                 // +0x10
    CIntelligentPointer<CThing_Retail> PThing;                          // +0x14
    long                       ScriptID;                        // +0x1c
    bool                       NullResource;                    // +0x20
    long                       ActionPriority;                  // +0x24
};

void CScriptGameResourceObjectScriptedThingLocal::MoveToPosition(const C3DVector& position, float proximity, EScriptEntityMoveType moveType, bool waitUntilDone, bool flag)
{
    CIntelligentPointer<CThing_Retail>* thing = &PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterfaceBit(&control, 0x1f);
            EMovementType movementType;
            switch (moveType) {
            case SCRIPT_MOVE_TYPE_0: movementType = MOVEMENT_TYPE_3; break;
            case SCRIPT_MOVE_TYPE_1: movementType = MOVEMENT_TYPE_5; break;
            default:                 movementType = MOVEMENT_TYPE_1; break;
            }
            control->AddAction(new NTCScriptedControl::CActionMoveTo(position, movementType, proximity, true, flag));
            if (waitUntilDone)
                WaitWhilePerformingTasks();
        }
    }
}