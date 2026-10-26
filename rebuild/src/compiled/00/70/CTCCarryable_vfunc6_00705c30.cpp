// CTCCarryable::vfunc_6() -- take the carry offset from the thing's carryable def and normalise it when it is not (nearly) zero
//
// Retail 0x00705C30, 137 bytes (plain `ret`). Vtable 0x0123B35C (CTCCarryable) slot 6.
// Callees: CDefClassBase::GetSubDef<CCarryableDef>(CDefPointer<const CCarryableDef>*) @ 0x005CC10E
// (manifest "GetSubDef<CAbilityDef>", one of the propagated GetSubDef labels), C3DVector::Normalise
// @ 0x00A14440 (manifest "GetScaled", propagated), the def's virtual slot 1 (release at zero).
// 0x0129BA3C is the extern 0.0001f.
// Layout: CTCCarryable (PDB): OffsetCoordRelativeToAttachToDummy +0x14; CCarryableDef (PDB):
// OffsetCoordRelativeToAttachToDummy +0x38; the parent thing's def +0x70.
// Shape: the parent's def is a CDefPointer member read through its inline operator-> into a local
// BEFORE the out pointer is declared (a direct raw `->PDef_Object` cast loads through ecx and rotates
// eax/edx in the copy; declaring the out pointer first stores its zero before the push). The out
// pointer starts null in the `push ecx` frame word; its object is read into edi
// right after the call and released by hand at the end (null test, `dec [edi+4] ; jne`). The
// vector is the implicit C3DVector assignment, then the landed GetLengthSquared > eps*eps test
// (0x008059F0 form).
class C3DVector { public:
    float X, Y, Z;
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    C3DVector& AccurateNormalise();                        // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
extern const float g_minLength_0129ba3c;           // 0x0129BA3C (0.0001f)

class CDefPointeeBase { public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};

template <class T>
class CDefPointer { public:                            // GetSubDef's out pointer (released by hand)
    CDefPointeeBase* Object;                    // +0x00
    CDefPointer() : Object(0) {}
    T* operator->() const { return (T*)Object; }
};

class CCarryableDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x38 - 0x08];
    C3DVector     OffsetCoordRelativeToAttachToDummy;   // +0x38
};

class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<const CCarryableDef>* out);   // 0x005CC10E GetSubDef<CCarryableDef>
};

class CThing { public:
    unsigned char    _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;            // +0x70 CDefPointer<const CThingDef>
};

class CTCCarryable { public:
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad08[4];
    bool          ForceFragility;               // +0x0c
    unsigned char _pad0d[3];
    void*         PCarryingTC;                  // +0x10
    C3DVector     OffsetCoordRelativeToAttachToDummy;   // +0x14
    float         OffsetAmount;                 // +0x20
    void vfunc_6();
};

void CTCCarryable::vfunc_6()
{
    CDefClassBase* thingDef = ParentThing->PDef.operator->();
    CDefPointer<const CCarryableDef> out;
    bool found = thingDef->GetSubDef(&out);
    CCarryableDef* def = (CCarryableDef*)out.Object;
    if (found)
    {
        OffsetCoordRelativeToAttachToDummy = def->OffsetCoordRelativeToAttachToDummy;
        if (OffsetCoordRelativeToAttachToDummy.GetLengthSquared() > g_minLength_0129ba3c * g_minLength_0129ba3c)
            OffsetCoordRelativeToAttachToDummy.AccurateNormalise();
    }
    if (def && --def->m_refCount == 0)
        def->Delete();
}