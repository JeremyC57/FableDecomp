// CScriptGameResourceObjectScriptedThing::MoveToAndPickUpGenericBox(const CScriptThing&, long, bool) -- queue move-to, turn-to-face and pick-up actions on the scripted thing for a live box
// Leaf: MoveToAndPickUpGenericBox
// Retail 0x00904CD0, 327 bytes, ret 0xC. Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PThing +0x14), the box's
// CScriptThing::GetPThing (slot 11, +0x2C), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline
// GetInterface 0x1F = CTCScriptedControl), operator new 0x00BFEA1A, CActionMoveTo ctor 0x009051F0 (0x50 bytes),
// CThing::GetPosition 0x004C73D0, CActionTurnToFacePosition ctor 0x007185D0 (0x1C bytes, speed 1/72),
// CActionPickUpGenericBox ctor 0x00713200 (0x20 bytes), CTCScriptedControl::AddAction 0x007137D0.
// Layout: engine/CScriptGameResourceObjectScriptedThing.h (PThing +0x14) through a method view, as the landed
// DropGenericBox 0x00904E20.
// Shape: the landed MoveToThing 0x00903E20 model -- the presence bit bound to a `const unsigned char&` HasTC result
// (stored to the shared key/control slot), the unchecked control read on a miss, the 0->3 / 1->5 / else 1 movement
// switch; each action is a new-expression straight into AddAction (VC's null test of new).
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

struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x20];
    unsigned long  ThingTypeFlags;                 // +0x6c
    const class C3DVector& GetPosition() const;   // 0x004C73D0
    unsigned char  _pad70[0x21];
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


class C3DVector;
class CTCScriptedControl;
namespace NTCScriptedControl {
struct CActionBase;
struct CActionMoveTo {                             // 0x50 bytes
    unsigned char _body[0x50];
    CActionMoveTo(CThing_Retail* target, long movementType, float distance, bool a, bool b, bool c, bool d, bool e, bool f);   // 0x009051F0
};
struct CActionTurnToFacePosition {                 // 0x1c bytes
    unsigned char _body[0x1c];
    CActionTurnToFacePosition(const C3DVector& position, float speed);   // 0x007185D0
};
struct CActionPickUpGenericBox {                   // 0x20 bytes
    unsigned char _body[0x20];
    CActionPickUpGenericBox(CThing_Retail* box);   // 0x00713200
};
}
class CTCScriptedControl { public:                        // interface 0x1f component
    void AddAction(void* action);                  // 0x007137D0
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void MoveToAndPickUpGenericBox(const CScriptThing& box, long approach, bool run);
};

void CScriptGameResourceObjectScriptedThing_Methods::MoveToAndPickUpGenericBox(const CScriptThing& box, long approach, bool run)
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CThing_Retail* boxThing = box.GetPThing();
            if (boxThing && !boxThing->m_dead) {
                CTCScriptedControl* control;
                thing->GetPItem()->GetInterface(&control, 0x1f);
                long movement;
                switch (approach) {
                case 0: movement = 3; break;
                case 1: movement = 5; break;
                default: movement = 1; break;
                }
                control->AddAction(new NTCScriptedControl::CActionMoveTo(boxThing, movement, 1.5f, true, run, true, true, true, false));
                control->AddAction(new NTCScriptedControl::CActionTurnToFacePosition(boxThing->GetPosition(), 0.013888889f));
                control->AddAction(new NTCScriptedControl::CActionPickUpGenericBox(boxThing));
            }
        }
    }
}