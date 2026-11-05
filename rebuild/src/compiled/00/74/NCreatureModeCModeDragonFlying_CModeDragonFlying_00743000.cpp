// NCreatureMode::CModeDragonFlying::~CModeDragonFlying() -- release PEndAction, destroy the filter, free the flight spline's control points (CModeDragonFlyingBase), then the inlined CModeBase destructor
// Leaf: ??1CModeDragonFlying
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743000, 121 bytes (`ret`).
// Label: the catalog's "CModeIntangible::~CModeIntangible" is a byte-twin propagation; the offsets (counted pointer
// +0x80/+0x84, filter +0x38 with its string at +0x3c, the spline block at +0x1c freed with free) are
// CModeDragonFlying's (constructor 0x007437C0; the landed CModeDragonFlyingBase ??_G 0x00743780 frees the same +0x1c).
// Callees: operator delete @ 0x00BFE9BC, ~CCharString @ 0x0099EAE0 (two strings), free @ 0x00BFEA14.
// Vtables stored: 0x0122FFCC (CAnimFilterBase at +0x38 and +8), 0x012673CC (CModeBase); neither 0x0126868C
// (CModeDragonFlying) nor 0x01268658 (CModeDragonFlyingBase): both are `__declspec(novtable)` with non-virtual
// destructors. Layout: retail CArray is 0x0c, so PDB offsets after the spline sit 4 bytes lower.
void __cdecl operator delete(void* block);          // 0x00BFE9BC
extern "C" void __cdecl free(void* memory);         // 0x00BFEA14

class CCharString { public:
    void* m_pData;
    ~CCharString();                                  // 0x0099EAE0
};

struct CCountedItem { };
struct SCountBlock {
    long m_references;                               // +0x00
    void (CCountedItem::*m_pfnDestroy)();            // +0x04 (called with the item as `this`)
    CCountedItem* m_pItem;                           // +0x08
};

template <class T>
class CCountedPointer { public:
    T* m_pObject;                                    // +0x00
    SCountBlock* m_pBlock;                           // +0x04
    CCountedPointer() : m_pObject(0), m_pBlock(0) { }
    ~CCountedPointer()
    {
        if (m_pBlock) {
            --m_pBlock->m_references;
            if (m_pBlock->m_references == 0) {
                (m_pBlock->m_pItem->*m_pBlock->m_pfnDestroy)();
                operator delete(m_pBlock);
            }
        }
        m_pObject = 0;
        m_pBlock = 0;
    }
};

class CAnimFilterBase { public:                             // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }
};

class CAnimFilterOfGroup : public CAnimFilterBase { public:        // vtable 0x01267568 (not stored by the implicit destructor)
    CCharString   Group;                             // +4
    unsigned long GroupCRC;                          // +8
};

class CThing;
struct CModeBase {                                   // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual void v00();
    CThing*            ParentThing;                  // +0x04
    CAnimFilterOfGroup WeaponBasedFilter;            // +0x08
    long               FramesBeforeRemovingSelf;     // +0x14
    bool               Active;                       // +0x18
    ~CModeBase() { }
};

class CCreatureActionBase;
#include "engine/C3DVector.h"

#include "engine/CArray.h"

class CCardinalSpline { public:                             // retail 0x14
    CArray<C3DVector> ControlPoints;                 // +0
    float             Tension;                       // +0xc
    bool              EndPointsConnected;            // +0x10
};

namespace NCreatureMode {

struct __declspec(novtable) CModeDragonFlyingBase : CModeBase {   // vtable 0x01268658
    CCardinalSpline FlightSpline;                    // +0x1c
    float           SplinePercentageComplete;        // +0x30
    float           LegAnimAlpha;                    // +0x34
};

struct __declspec(novtable) CModeDragonFlying : CModeDragonFlyingBase {   // vtable 0x0126868C
    CAnimFilterOfGroup Filter;                       // +0x38
    C3DVector          Destination;                  // +0x44
    C3DVector          StartPos;                     // +0x50
    C3DVector          PreviousPos;                  // +0x5c
    float              DistanceToTravel;             // +0x68
    float              Speed;                        // +0x6c
    float              StartSpeed;                   // +0x70
    float              MovementDuringAnimationLoop;  // +0x74
    float              Acceleration;                 // +0x78
    float              AnimLength;                   // +0x7c
    CCountedPointer<CCreatureActionBase> PEndAction; // +0x80
    long               NumAnimLoops;                 // +0x88
    long               NumAnimLoopsToDo;             // +0x8c
    bool               FirstFrame;                   // +0x90
    bool               UsePreviousPos;               // +0x91
    ~CModeDragonFlying();
};

CModeDragonFlying::~CModeDragonFlying()
{
}

}