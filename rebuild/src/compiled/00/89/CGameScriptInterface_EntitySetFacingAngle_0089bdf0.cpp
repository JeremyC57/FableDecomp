// CGameScriptInterface::EntitySetFacingAngle(const CScriptThing& entity, float angle, bool immediate) const -- turn a valid entity to the angle (a fraction of a full turn): reset TC 7, stop its looking (TC 0x43); unless immediate, make it look at a point one unit away along the new heading, else set the physics facing directly and reset its perception (TC 0x34)
// Retry 10b: cos(pitch) held as a FLOAT local (a double local keeps it on the x87 stack past the Y product: `fmul st(1)` + a late `fstp st(0)`; the float is consumed by retail's `fld st(2) ; fmulp` / `fmulp st(1)`).
// Leaf: ?EntitySetFacingAngle@CGameScriptInterface@@UBEXABVCScriptThing@@M_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089BDF0, length 385 bytes (`ret 0Ch`).
// Callees: the script thing's slots 75 (+0x12C IsValid), 11 (+0x2C GetThing) and 6 (+0x18, the position), the thing
//   physics (+0x60) slots 63 (+0xFC, current facing angle) and 66 (+0x108, set facing angle), the component-map
//   LowerBound @ 0x0040F020, the TC 7 reset @ 0x006CB6B0 (manifest "CScriptThing::operator==" is propagated),
//   CTCLook::StopLooking(long) @ 0x00751B80 and LookAtPos(const C3DVector&, long) @ 0x00753E90 (TC 0x43),
//   CTCPerceiveThing::ResetPerceptionVariables @ 0x00744DB0 (TC 0x34).
// Shape: the look direction is the intrinsic cos/sin pair of `angle * (double)2pi` and a zero pitch read from a double
//   in memory (0x0122ED70, not folded), X = sin(yaw)*cos(pitch), Y = cos(yaw)*cos(pitch), Z = sin(pitch).
#include <math.h>
#pragma intrinsic(sin, cos)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
extern const double g_zeroPitch;                        // 0x0122ED70 (0.0)
extern const double g_twoPi;                            // 0x0124F2B8 ((double)6.2831855f)
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap_Retail;
struct CThingPhysics_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngle();                     // slot 63 (+0xFC)
    virtual void v64(); virtual void v65();
    virtual void SetFacingAngle(float angle);           // slot 66 (+0x108)
};
struct CThing_Retail {
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];         // +0x20
    CInterfaceMap_Retail  m_interfaces;             // +0x44
    unsigned char         _pad4c[0x14];
    CThingPhysics_Retail* m_pPhysics;               // +0x60
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase_Retail* GetInterface(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(type);
        return ((CTCBase_Retail*)((it == map->last || interfaceType < it->first) ? map->last : it)->second);
    }
};
struct CTCType7 { void Reset(); };                          // 0x006CB6B0
class CTCLook { public:
    void StopLooking(long reason);                          // 0x00751B80
    void LookAtPos(const C3DVector& pos, long priority);    // 0x00753E90
};
class CTCPerceiveThing { public: void ResetPerceptionVariables(); };   // 0x00744DB0
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetFacingAngle(const CScriptThing& entity, float angle, bool immediate) const
{
    if (!entity.IsAlive())
        return;
    CThing_Retail* thing = ((CThing_Retail*)entity.GetPThing());
    if (thing->m_pPhysics->GetFacingAngle() == angle)
        return;
    if (thing->HasInterface(7))
        ((CTCType7*)thing->GetInterface(7))->Reset();
    CTCLook* look;
    if (thing->HasInterface(0x43))
        look = (CTCLook*)thing->GetInterface(0x43);
    else
        look = 0;
    if (look)
        look->StopLooking(0);
    if (!immediate && look) {
        float cosPitch = (float)cos(g_zeroPitch);
        double yaw = angle * g_twoPi;
        C3DVector direction((float)(sin(yaw) * cosPitch), (float)(cos(yaw) * cosPitch), (float)sin(g_zeroPitch));
        look->LookAtPos(entity.GetPos() + direction, 0x21);
        return;
    }
    thing->m_pPhysics->SetFacingAngle(angle);
    if (thing->HasInterface(0x34))
        ((CTCPerceiveThing*)thing->GetInterface(0x34))->ResetPerceptionVariables();
}