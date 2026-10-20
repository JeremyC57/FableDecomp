// CTCObjectAugmentations::GetHealthToAddThisFrame() const -- sum, over the attached augmentations whose health frequency is positive, the def's health increase on every frame that is a multiple of the frequency in frames (frequency * frames-per-second, rounded)
// Grade: DIFFER(210v199) -- lane gap: retail rounds with an inline-asm helper (frame pointer `push ebp ; mov ebp,esp`, bare dword `fld ; fadd 0.5 ; fistp ; fild ; fsub 1.0` with the float/int bit compare and `dec`), not expressible without __asm
// Leaf: GetHealthToAddThisFrame
// Retail: Fable.exe (TLC, VC7.1) @ 0x00766260, 199 bytes (plain `ret`; PDB ?GetHealthToAddThisFrame@CTCObjectAugmentations@@QBEJXZ).
// Callees: CDefClassBase::GetSubDef<CAugmentationDef> @ 0x0042B248 (landed), CWorld::GetWorldFrame() @ 0x0049D870 (static), the def
//   release through vtable slot 1 (`dec [esi+4] ; jne`). Globals: g_framesPerSecond @ 0x01375550, g_epsilon_0129ba3c @ 0x0129BA3C.
// Layout: Ego_r.pdb CTCObjectAugmentations Augmentations +0x14 (vector<CDefPointer<CThingBaseDef>>, 12-byte retail vector);
//   CAugmentationDef (PDB = retail) frequency/increase +0x3c/+0x40.
// Tried: plain C++ rounding helper (one grade). Next: lane gap: inline-asm fistp rounding (the PackVertexNormal family, GOTCHAS
//   chunk 145).
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                    // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (--def->m_refCount == 0)
        def->Destroy();
}
struct CAugmentationDef : CDefBase {
    unsigned char _pad08[0x3c - 0x08];
    float FrequencyToAddHealthInSeconds;                // +0x3c
    long  HealthToAddPerIncrease;                       // +0x40
    float FrequencyToAddStaminaInSeconds;               // +0x44
    long  StaminaToAddPerIncrease;                      // +0x48
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x0042B248 (T = CAugmentationDef)
};
struct CThingBaseDef : CDefClassBase { };
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CWorld { static long GetWorldFrame(); };          // 0x0049D870
extern long g_framesPerSecond;                          // 0x01375550
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C
inline long RoundToLong(float x)
{
    long r = (long)(x + 0.5f);
    if (x == (float)r - 1.0f)
        --r;
    return r;
}
struct CTCObjectAugmentations {
    virtual void v00();
    unsigned char _pad04[0x14 - 0x04];
    vector< CDefPointer<CThingBaseDef> > Augmentations; // +0x14
    long GetHealthToAddThisFrame() const;
};

long CTCObjectAugmentations::GetHealthToAddThisFrame() const
{
    long total = 0;
    for (unsigned long i = 0; i < Augmentations.size(); ++i) {
        if (Augmentations[i].Object) {
            CDefPointer<CAugmentationDef> slot;
            Augmentations[i].Object->GetSubDef(slot);
            CAugmentationDef* aug = slot.Object;
            if (aug->FrequencyToAddHealthInSeconds > g_epsilon_0129ba3c) {
                long period = RoundToLong(g_framesPerSecond * aug->FrequencyToAddHealthInSeconds);
                if (CWorld::GetWorldFrame() % period == 0)
                    total += aug->HealthToAddPerIncrease;
            }
            ReleaseDef(aug);
        }
    }
    return total;
}
