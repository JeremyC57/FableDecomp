// CScriptGameResourceObjectScriptedThing::MoveToPosition(const C3DVector& position, float proximity, EScriptEntityMoveType moveType, bool waitUntilDone, bool flag) -- queue a move-to-position action for the live scripted thing and optionally wait for its tasks
// Grade: DIFFER(ours v retail) -- 212v220: retail stores the interface-present bit as a bool frame byte (`shr ecx,1Fh ; and cl,1 ; mov [esp+0Fh],cl ; je`, never reloaded, sub esp,8 frame) -- also in MoveToThing 0x903E20; our `bool present = (w >> n) & 1` gives `test cl,1` without the store
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


void* __cdecl operator new(size_t size);           // 0x00BFEA1A

struct CTCBase;

struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};

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
    template<class T> __forceinline bool GetInterfaceBit(T** ppOut, long interfaceType)
    {
        bool present = (m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return present;
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

struct CTCScriptedControl {                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

struct C3DVector { float X, Y, Z; };

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
    CIntelligentPointer_Retail PThing;                          // +0x14
    long                       ScriptID;                        // +0x1c
    bool                       NullResource;                    // +0x20
    long                       ActionPriority;                  // +0x24
};

void CScriptGameResourceObjectScriptedThingLocal::MoveToPosition(const C3DVector& position, float proximity, EScriptEntityMoveType moveType, bool waitUntilDone, bool flag)
{
    CIntelligentPointer_Retail* thing = &PThing;
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
