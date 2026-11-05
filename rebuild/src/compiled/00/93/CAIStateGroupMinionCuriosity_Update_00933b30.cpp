// CAIStateGroup_MinionCuriosity::Update() -- walk to within the creature's extended sight radius of the
// Leaf: ?Update@CAIStateGroup_MinionCuriosity@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// curiosity position, then terminate.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933B30, length 95 bytes.
// Callees: CVectorMap::LowerBound @ 0x004365B0 (the inlined interface lookup, type 0x34 = bit 20 of the bitset
// dword at +0x24; this module links the second copy of LowerBound), CTCPerceiveThing::GetExtendedSightRadius
// @ 0x007448F0 (float in st0, stored straight into the pushed argument slot), MoveToCuriosityProximity(float)
// @ 0x00933A40 (own member, bool result dropped), CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: uninitialised component local, untested lookup, both paths merge (`jmp`) into the call. Base layout
// modelled locally (PDB names); no engine header for CAIStateGroup_MinionCuriosity.
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound -- the same shape the landed
// GetInterface_0x.. bodies use, here inlined into the caller through GetTC<T>().
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18 CThingAICreature*
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
public:
    void Terminate();   // 0x008FCF40
};

class CTCPerceiveThing { public:                       // interface type 0x34 component
    float GetExtendedSightRadius() const;             // 0x007448F0
};

struct CAIStateGroup_MinionCuriosity : CAIStateGroupBase {
    float CuriosityPos_X, CuriosityPos_Y, CuriosityPos_Z;   // +0x14 C3DVector
    bool MoveToCuriosityProximity(float radius);            // 0x00933A40
    virtual void Update();
};

void CAIStateGroup_MinionCuriosity::Update()
{
    const CTCPerceiveThing* pPerceive;
    PBrain->Creature->PeekTC(TCI_PERCEIVE_THING, &pPerceive);
    MoveToCuriosityProximity(pPerceive->GetExtendedSightRadius());
    Terminate();
}