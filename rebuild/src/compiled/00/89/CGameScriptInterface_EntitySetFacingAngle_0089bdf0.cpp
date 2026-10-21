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
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
extern const double g_zeroPitch;                        // 0x0122ED70 (0.0)
extern const double g_twoPi;                            // 0x0124F2B8 ((double)6.2831855f)
struct CTCBase_Retail;
struct CInterfaceMap_Retail {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
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
        long type = interfaceType;
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CTCType7 { void Reset(); };                          // 0x006CB6B0
struct CTCLook {
    void StopLooking(long reason);                          // 0x00751B80
    void LookAtPos(const C3DVector& pos, long priority);    // 0x00753E90
};
struct CTCPerceiveThing { void ResetPerceptionVariables(); };   // 0x00744DB0
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPosition() const;   // slot 6 (+0x18)
    virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;        // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};
struct CGameScriptInterface {
    virtual void EntitySetFacingAngle(const CScriptThing& entity, float angle, bool immediate) const;
};

void CGameScriptInterface::EntitySetFacingAngle(const CScriptThing& entity, float angle, bool immediate) const
{
    if (!entity.IsValid())
        return;
    CThing_Retail* thing = entity.GetThing();
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
        look->LookAtPos(entity.GetPosition() + direction, 0x21);
        return;
    }
    thing->m_pPhysics->SetFacingAngle(angle);
    if (thing->HasInterface(0x34))
        ((CTCPerceiveThing*)thing->GetInterface(0x34))->ResetPerceptionVariables();
}