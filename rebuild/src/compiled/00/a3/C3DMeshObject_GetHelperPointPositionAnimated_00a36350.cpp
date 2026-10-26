// C3DMeshObject::GetHelperPointPositionAnimated(const CCharString& name, long index, const C3DAnimationBlendState& state, const C3DAnimationBlendState& other, float blend, C3DAnimationManager& manager) const -- animate the named helper point through the mesh and transform it by the object's position matrix
// Leaf: GetHelperPointPositionAnimated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36350, 219 bytes (+ int3 padding), `ret 1Ch` (hidden result + six arguments).
// Identity: Ego_r.pdb `?GetHelperPointPositionAnimated@C3DMeshObject@@QBE?AVC3DVector@@ABVCCharString@@JABVC3DAnimationBlendState@@1MAAVC3DAnimationManager@@@Z`
//   (the two-blend-state overload); callers 0x004BF27B / 0x004C0835. The catalog's "C3DMesh_TransformPoint" is descriptive.
// Callees: CRC Calc @ 0x004014A0, C3DMeshStats::GetHelperPoint(unsigned long, unsigned long) const @ 0x00A2E8C0 (landed),
//   the virtual C3DMeshStats slot 4 (`call [edx+10h]`; pure in C3DMeshStats' vtable 0x0129CF04, overridden by
//   C3DMesh2::GetHelperPointPositionAnimated(const C3DHelperPoint&, state, other, float, manager) const, Ego_r.pdb).
// Layout (Ego_r.pdb C3DMeshObject): Mesh = CCountedPointer<const C3DMeshStats> +8 (Data read for each call),
//   PositionMatrix (CMatrix3x4, 3x3 rotation + translation, row-major E11..E43) +0x10.
// Shape: the helper point is a named reference fetched in its own statement (CRC, then GetHelperPoint, before any of the
//   virtual call's pushes; nesting the lookup in the argument list pushes the five arguments first), and Mesh's Data is
//   re-read for the virtual call. The same transform tail as the landed GetHelperPointPositionStatic 0x00A361A0: the animated position comes back
//   by hidden pointer into the local, x and y are copied to named float locals (homed in the dead name/index argument
//   slots) and each component is `x*Ei1 + y*Ei2 ... + z*Ei3 + E4i` evaluated through the FPU, stored x, y, z.
class CCharStringData { public:
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];       // 0x0129AAF4: ""
class CCharString { public:
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& p) : X(p.X), Y(p.Y), Z(p.Z) { }
};
class C3DHelperPoint { public:                            // 16 bytes
    C3DVector Position;                            // +0x00
    long      BoneIndex;                           // +0x0c
};
class C3DAnimationBlendState;
class C3DAnimationManager;

class C3DMeshStats { public:
    virtual void v00();                            // slot 0: scalar deleting destructor 0x00A353F0
    virtual void v01();                            // slot 1: RelocateData 0x00A2E820
    virtual void v02();                            // slot 2
    virtual void v03();                            // slot 3
    virtual C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint& helper, const C3DAnimationBlendState& state,
                                                     const C3DAnimationBlendState& other, float blend,
                                                     C3DAnimationManager& manager) const = 0;   // slot 4
    const C3DHelperPoint* GetHelperPoint(unsigned long nameHash, unsigned long index) const;     // 0x00A2E8C0
};

template <class T> class CCountedPointer { public:
    T*    Data;                                    // +0
    void* Info;                                    // +4
    T* operator->() const { return Data; }
};

class C3DMeshInfo;

class C3DMeshObject { public:
    virtual void v00();
    C3DMeshInfo*                         MeshInfo;        // +0x04
    CCountedPointer<const C3DMeshStats>  Mesh;            // +0x08
    float                                PositionMatrix[12];   // +0x10 CMatrix3x4
    C3DVector GetHelperPointPositionAnimated(const CCharString& name, long index, const C3DAnimationBlendState& state,
                                             const C3DAnimationBlendState& other, float blend,
                                             C3DAnimationManager& manager) const;
};

C3DVector C3DMeshObject::GetHelperPointPositionAnimated(const CCharString& name, long index, const C3DAnimationBlendState& state,
                                                        const C3DAnimationBlendState& other, float blend,
                                                        C3DAnimationManager& manager) const
{
    const C3DHelperPoint& helper = *Mesh->GetHelperPoint(CCRC::Calc(0, name.GetChars(), name.GetLength()), index);
    C3DVector point = Mesh->GetHelperPointPositionAnimated(helper, state, other, blend, manager);
    const float x = point.X;
    const float y = point.Y;
    point.X = x*PositionMatrix[0] + y*PositionMatrix[3] + point.Z*PositionMatrix[6] + PositionMatrix[9];
    point.Y = x*PositionMatrix[1] + y*PositionMatrix[4] + point.Z*PositionMatrix[7] + PositionMatrix[10];
    point.Z = x*PositionMatrix[2] + y*PositionMatrix[5] + point.Z*PositionMatrix[8] + PositionMatrix[11];
    return point;
}