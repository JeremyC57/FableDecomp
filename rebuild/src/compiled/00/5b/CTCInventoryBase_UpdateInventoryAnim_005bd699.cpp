// CTCInventoryBase::UpdateInventoryAnim() -- advances the inventory preview thing's rotation: latches last angles and set, steps each axis angle by its gain over the def's per-axis seconds, rotates the right-handed set and wraps the angles past one turn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD699, length 405 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Leaf: UpdateInventoryAnim
// Shape: the three per-axis steps read the def through a by-value GetPDef() temporary (inc / dec of the def count around each
// read). The temporary's release must be spelled `~CDefPointer() { if (Object) Object->ReleaseReference(); }` with an inline
// `ReleaseReference() { if (--m_count == 0) Destroy(); }` on the pointee: that keeps the Y step's `dec [ecx+4]` between
// `fmul [ecx+78Ch]` and `fdivr [1.0f]` as retail; the direct `if (--Object->m_count == 0) Object->Destroy();` spelling lets the
// lane hoist the dec above the fmul (405v405, one swap). The X/Z steps (gain numerator read through `this`) order the same way
// under both spellings.
// Layout: CTCInventoryBase retail = PDB minus 0x14 at the tail: RH sets +0xcc/+0xe4, angles +0xfc..+0x110, gains +0x114..+0x11c.
#pragma optimize("y", off)
#pragma optimize("s", on)

class C3DVector { public: float X, Y, Z; };
extern const C3DVector g_XAxis;                 // 0x0137CDFC
extern const C3DVector g_YAxis;                 // 0x0137CDF0
extern const C3DVector g_ZAxis;                 // 0x0137CE08
extern long g_FramesPerSecond;                  // 0x01375550

class CRightHandedSet { public:
    C3DVector Forward;
    C3DVector Up;
    void Rotate(const C3DVector& axis, float angle);   // 0x00A22F80
};

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
    __forceinline void ReleaseReference() { if (--m_count == 0) Destroy(); }
};

class CInventoryDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x780];
    float InventoryAnimXRotationSeconds;        // +0x788
    float InventoryAnimYRotationSeconds;        // +0x78c
    float InventoryAnimZRotationSeconds;        // +0x790
};

template<class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_count; }
    __forceinline ~CDefPointer()
    {
        if (Object)
            Object->ReleaseReference();
    }
    T* operator->() const { return Object; }
};

struct CTCInventoryBase {
    unsigned char   _pad_0x00[0x68];
    CDefPointer<CInventoryDef> PDef;            // +0x68
    unsigned char   _pad_0x6c[0x60];
    CRightHandedSet InventoryAnimCurrentRHSet;  // +0xcc
    CRightHandedSet InventoryAnimLastRHSet;     // +0xe4
    float InventoryAnimCurrentXRotationAngle;   // +0xfc
    float InventoryAnimLastXRotationAngle;      // +0x100
    float InventoryAnimCurrentYRotationAngle;   // +0x104
    float InventoryAnimLastYRotationAngle;      // +0x108
    float InventoryAnimCurrentZRotationAngle;   // +0x10c
    float InventoryAnimLastZRotationAngle;      // +0x110
    float InventoryAnimXRotationAngleGain;      // +0x114
    float InventoryAnimYRotationAngleGain;      // +0x118
    float InventoryAnimZRotationAngleGain;      // +0x11c

    __forceinline CDefPointer<CInventoryDef> GetPDef() const { return PDef; }
    void UpdateInventoryAnimXAxis();            // 0x005BCEA4
    void UpdateInventoryAnimZAxis();            // 0x005BCEF8
    void UpdateInventoryAnim();
};

void CTCInventoryBase::UpdateInventoryAnim()
{
    UpdateInventoryAnimXAxis();
    UpdateInventoryAnimZAxis();
    InventoryAnimLastXRotationAngle = InventoryAnimCurrentXRotationAngle;
    InventoryAnimLastRHSet = InventoryAnimCurrentRHSet;
    InventoryAnimLastYRotationAngle = InventoryAnimCurrentYRotationAngle;
    InventoryAnimLastZRotationAngle = InventoryAnimCurrentZRotationAngle;
    float x = InventoryAnimXRotationAngleGain / (g_FramesPerSecond * GetPDef()->InventoryAnimXRotationSeconds);
    float y = 1.0f / (g_FramesPerSecond * GetPDef()->InventoryAnimYRotationSeconds);
    float z = InventoryAnimZRotationAngleGain / (g_FramesPerSecond * GetPDef()->InventoryAnimZRotationSeconds);
    InventoryAnimCurrentRHSet.Rotate(g_XAxis, -x);
    InventoryAnimCurrentRHSet.Rotate(g_YAxis, -y);
    InventoryAnimCurrentRHSet.Rotate(g_ZAxis, -z);
    x += InventoryAnimCurrentXRotationAngle;
    InventoryAnimCurrentXRotationAngle = x;
    y += InventoryAnimCurrentYRotationAngle;
    InventoryAnimCurrentYRotationAngle = y;
    z += InventoryAnimCurrentZRotationAngle;
    InventoryAnimCurrentZRotationAngle = z;
    if (x > 1.0f)
        InventoryAnimCurrentXRotationAngle = x - 1.0f;
    if (y > 1.0f)
        InventoryAnimCurrentYRotationAngle = y - 1.0f;
    if (z > 1.0f)
        InventoryAnimCurrentZRotationAngle = z - 1.0f;
}