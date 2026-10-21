// CTCInventoryBase::UpdateInventoryAnim() -- advances the inventory preview thing's rotation: latches last angles and set, steps each axis angle by its gain over the def's per-axis seconds, rotates the right-handed set and wraps the angles past one turn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD699, length 405 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Leaf: UpdateInventoryAnim
// Grade: DIFFER(405v405) -- one scheduling swap in the Y step: retail `fmul [ecx+78Ch] ; dec [ecx+4] ; fdivr [1.0]` (the
// GetPDef() temp's release between the multiply and the divide), ours issues the `dec` before the fmul; all else identical.
// Layout: CTCInventoryBase retail = PDB minus 0x14 at the tail: RH sets +0xcc/+0xe4, angles +0xfc..+0x110, gains +0x114..+0x11c.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct C3DVector { float X, Y, Z; };
extern const C3DVector g_XAxis;                 // 0x0137CDFC
extern const C3DVector g_YAxis;                 // 0x0137CDF0
extern const C3DVector g_ZAxis;                 // 0x0137CE08
extern long g_FramesPerSecond;                  // 0x01375550

struct CRightHandedSet {
    C3DVector Forward;
    C3DVector Up;
    void RotateAboutAxis(const C3DVector& axis, float angle);   // 0x00A22F80
};

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};

struct CInventoryDef : CDefPointeeBase {
    unsigned char _pad08[0x780];
    float InventoryAnimXRotationSeconds;        // +0x788
    float InventoryAnimYRotationSeconds;        // +0x78c
    float InventoryAnimZRotationSeconds;        // +0x790
};

template<class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_count; }
    __forceinline ~CDefPointer()
    {
        if (Object)
            if (--Object->m_count == 0)
                Object->Destroy();
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
    InventoryAnimCurrentRHSet.RotateAboutAxis(g_XAxis, -x);
    InventoryAnimCurrentRHSet.RotateAboutAxis(g_YAxis, -y);
    InventoryAnimCurrentRHSet.RotateAboutAxis(g_ZAxis, -z);
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
