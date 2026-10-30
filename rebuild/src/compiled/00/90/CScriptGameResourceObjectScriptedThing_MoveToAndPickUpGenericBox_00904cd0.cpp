// CScriptGameResourceObjectScriptedThing::MoveToAndPickUpGenericBox(const CScriptThing&, long, bool) -- queue move-to, turn-to-face and pick-up actions on the scripted thing for a live box
// Leaf: MoveToAndPickUpGenericBox
// Retail 0x00904CD0, 327 bytes, ret 0xC. Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PThing +0x14), the box's
// CScriptThing::GetPThing (slot 11, +0x2C), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline
// GetInterface 0x1F = CTCScriptedControl), operator new 0x00BFEA1A, CActionMoveTo ctor 0x009051F0 (0x50 bytes),
// CThing::GetPos 0x004C73D0, CActionTurnToFacePosition ctor 0x007185D0 (0x1C bytes, speed 1/72),
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

class C3DVector;
class CThing { public:
    const C3DVector& GetPos() const;   // 0x004C73D0
};
struct CThing_Retail : public CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x20];
    unsigned long  ThingTypeFlags;                 // +0x6c
    unsigned char  _pad70[0x21];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    unsigned char HasTC(long interfaceType) const { return (unsigned char)((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CIntelligentPointer_Retail {                // CIntelligentPointer<CThing>
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 CBaseIntelligentPointer::GetPItem
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"


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
            CThing_Retail* boxThing = ((CThing_Retail*)box.GetPThing());
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
                control->AddAction(new NTCScriptedControl::CActionTurnToFacePosition(boxThing->GetPos(), 0.013888889f));
                control->AddAction(new NTCScriptedControl::CActionPickUpGenericBox(boxThing));
            }
        }
    }
}