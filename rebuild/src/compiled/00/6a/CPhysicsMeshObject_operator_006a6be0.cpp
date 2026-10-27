// CPhysicsMeshObject::operator=(const CPhysicsMeshObject&) -- memberwise assignment: re-point the counted mesh, copy the position matrix, scale and 1/scale
// Leaf: ??4CPhysicsMeshObject@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe @ 0x006A6BE0, 88 bytes, thiscall `ret 4`.
// Identity (proven): Ego_r.pdb `??4CPhysicsMeshObject@@QAEAAV0@ABV0@@Z`; the caller 0x006A5C87 (in a CThingPhysical
// method) builds a CPhysicsMeshObject local (CBaseClass ctor 0x0099A2D0, vtable 0x0125D084, Mesh = 0), assigns the
// temporary returned by CTCPhysicsBase::GetPhysicsMeshObject() 0x006B0850 to it through this function, then runs
// ~CPhysicsMeshObject 0x00A127D0 on the temporary.
// Layout (Ego_r = retail, 0x40): vptr +0 (CBaseClass), Mesh +4 (CIVCountedPointer<const CPhysicsMesh>: the mesh's
// ref count +4, virtual slot 1 deletes it), PositionMatrix +8 (CMatrix3x4, 0x30 bytes: `rep movsd` x12), Scale +0x38,
// OO_Scale +0x3C. The vptr is not assigned (the implicit operator= of a polymorphic class).
// Shape: the inline CIVCountedPointer::operator= (`if (m_pObject != other.m_pObject) { release; take; addref }`), the
// release through a local copy of the object pointer (as the landed ~CIVCountedPointer 0x00A127D0) so `dec` sets the flags.
class CPhysicsMesh { public:
    virtual void v00();
    virtual void DeleteThis();                     // slot 1
    long m_refCount;                               // +0x04
};

template <class T> class CIVCountedPointer { public:
    T* m_pObject;
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            T* object = m_pObject;
            if (object) {
                if (--object->m_refCount == 0)
                    object->DeleteThis();
                m_pObject = 0;
            }
            m_pObject = other.m_pObject;
            if (m_pObject)
                ++m_pObject->m_refCount;
        }
        return *this;
    }
};

class CMatrix3x4 { public:                         // 0x30 bytes
    float M[12];
};

class CBaseClass { public:                         // vtable 0x01231710
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
};

class CPhysicsMeshObject : public CBaseClass { public:   // vtable 0x0125D084
    CIVCountedPointer<CPhysicsMesh> Mesh;          // +0x04
    CMatrix3x4 PositionMatrix;                     // +0x08
    float Scale;                                   // +0x38
    float OO_Scale;                                // +0x3c
    CPhysicsMeshObject& operator=(const CPhysicsMeshObject& other);
};

CPhysicsMeshObject& CPhysicsMeshObject::operator=(const CPhysicsMeshObject& other)
{
    Mesh = other.Mesh;
    PositionMatrix = other.PositionMatrix;
    Scale = other.Scale;
    OO_Scale = other.OO_Scale;
    return *this;
}