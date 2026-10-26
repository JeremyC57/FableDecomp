// CScriptGameResourceObjectScriptedThing::MoveToThing(const CScriptThing& target, float proximity, EScriptEntityMoveType moveType, bool wait, bool avoidDynamicObstacles, bool ignorePathPreferability, bool faceMovement) -- queue a move-to action towards a live target for the live scripted thing, then optionally wait while it performs its tasks
// Leaf: MoveToThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903E20, length 258 bytes (`ret 1Ch`), vtable 0x0128D86C slot 5.
// PDB: ?MoveToThing@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@MW4EScriptEntityMoveType@@_N222@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CScriptThing::IsAlive (slot 75) and GetPThing
// (slot 11), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x1f =
// CTCScriptedControl), operator new @ 0x00BFEA1A (0x50 bytes), NTCScriptedControl::CActionMoveTo::CActionMoveTo(CThing*, long,
// float, bool x6) @ 0x009051F0, CTCScriptedControl::AddAction @ 0x007137D0, this->WaitWhilePerformingTasks (slot 11, +0x2c).
// Shape: the landed FollowThing 0x00904030 family guard; the presence bit is extracted as a byte (`shr ecx,1Fh ; and cl,1`)
// and stored to a frame byte in the dead `target` slot that is never reloaded: the HasTC result is bound to a const
// reference inside the inline GetInterface (a temporary with a frame home; a plain local folds into `test cl,1`). The script
// move type maps 0 -> 3, 1 -> 5, else 1 (a switch); the miss arm reads the never-written control from a dead argument slot.
// Layout: the engine header's offsets (ScriptInterface +0x10, PThing +0x14) on a local polymorphic model, because the
// slot-11 virtual call needs a real vptr (deriving virtuals from the flat header would add a second vptr).
#include <stddef.h>

void* __cdecl operator new(size_t size);           // 0x00BFEA1A

class CTCBase;

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
    unsigned char  _pad4c[0x45];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    unsigned char HasTC(long interfaceType) const { return (unsigned char)((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const unsigned char& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return present != 0;
    }
};
struct CIntelligentPointer_Retail {                // CIntelligentPointer<CThing>
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 CBaseIntelligentPointer::GetPItem
};

class CScriptThing {                               // vtable 0x01238C8C
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CThing_Retail* GetPThing() const;      // slot 11 (+0x2c) 0x004AAA60
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                  // slot 75 (+0x12c) 0x004AB130
};

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
    virtual void v00();
};
struct CActionMoveTo : CActionBase {               // vtable 0x0127091C, 0x50 bytes
    unsigned char _body[0x4c];
    CActionMoveTo(CThing_Retail* target, long movementType, float proximity, bool faceMovement,
                  bool avoidDynamicObstacles, bool useActionPoint, bool useBackupProximity,
                  bool cancelAllActionsOnFailure, bool ignorePathPreferability);   // 0x009051F0
    CActionMoveTo(CThing_Retail* routeThing, long movementType, bool faceMovement, bool avoidDynamicObstacles);   // 0x00905290
};
}

class CTCScriptedControl { public:                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

enum EScriptEntityMoveType { SCRIPT_MOVE_WALK = 0, SCRIPT_MOVE_RUN = 1 };

class CGameScriptInterface;
class CScriptGameResourceObjectScriptedThing { public:    // retail layout = engine/CScriptGameResourceObjectScriptedThing.h (vptr at +0)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void WaitWhilePerformingTasks();       // slot 11 (+0x2c) 0x00904380
    void*                      IntelligentPointer; // +0x04 (CBaseObject)
    void*                      PImp_Data;          // +0x08 (CScriptGameResourceObjectScriptedThingBase::PImp)
    void*                      PImp_Info;          // +0x0c
    CGameScriptInterface*      ScriptInterface;    // +0x10
    CIntelligentPointer_Retail PThing;             // +0x14 CIntelligentPointer<CThing>
    long                       ScriptID;           // +0x1c
    bool                       NullResource;       // +0x20
    long                       ActionPriority;     // +0x24
    void MoveToThing(const CScriptThing& target, float proximity, EScriptEntityMoveType moveType, bool wait,
                     bool avoidDynamicObstacles, bool ignorePathPreferability, bool faceMovement);
};

void CScriptGameResourceObjectScriptedThing::MoveToThing(const CScriptThing& target, float proximity, EScriptEntityMoveType moveType, bool wait,
                                                                bool avoidDynamicObstacles, bool ignorePathPreferability, bool faceMovement)
{
    CIntelligentPointer_Retail* thing = &PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (target.IsAlive()) {
                CThing_Retail* targetThing = target.GetPThing();
                CTCScriptedControl* control;
                thing->GetPItem()->GetInterface(&control, 0x1f);
                long movementType;
                switch (moveType) {
                case SCRIPT_MOVE_WALK: movementType = 3; break;
                case SCRIPT_MOVE_RUN:  movementType = 5; break;
                default:               movementType = 1; break;
                }
                control->AddAction(new NTCScriptedControl::CActionMoveTo(targetThing, movementType, proximity, faceMovement,
                                                                         avoidDynamicObstacles, false, false, false, ignorePathPreferability));
                if (wait)
                    WaitWhilePerformingTasks();
            }
        }
    }
}