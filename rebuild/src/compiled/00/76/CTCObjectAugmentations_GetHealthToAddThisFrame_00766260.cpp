// CTCObjectAugmentations::GetHealthToAddThisFrame() const -- sum, over the attached augmentations whose health frequency is positive, the def's health increase on every frame that is a multiple of the frequency in frames (frequency * frames-per-second, rounded)
// Leaf: GetHealthToAddThisFrame
// Retail: Fable.exe (TLC, VC7.1) @ 0x00766260, 199 bytes (plain `ret`; PDB ?GetHealthToAddThisFrame@CTCObjectAugmentations@@QBEJXZ).
// Callees: CDefClassBase::GetSubDef<CAugmentationDef> @ 0x0042B248 (landed), CWorld::GetWorldFrame() @ 0x0049D870 (static), the def
//   release through vtable slot 1 (`dec [esi+4] ; jne`). Globals: g_framesPerSecond @ 0x01375550, g_epsilon_0129ba3c @ 0x0129BA3C.
// Layout: Ego_r.pdb CTCObjectAugmentations Augmentations +0x14 (vector<CDefPointer<CThingBaseDef>>, 12-byte retail vector);
//   CAugmentationDef (PDB = retail) frequency/increase +0x3c/+0x40.
#pragma optimize("y", off)
// Retail-asm: 0x007662CA..0x007662E4 `fld [ebp-8] ; fadd [ebp-14h] ; fistp dword ptr [ebp-0Ch] ; fild [ebp-0Ch] ; fsub [ebp-10h] ; fstp [ebp-18h] ; mov eax,[ebp-8] ; cmp eax,[ebp-18h] ; jne ; dec dword ptr [ebp-0Ch]` -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                    // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (--def->m_refCount == 0)
        def->Destroy();
}
class CAugmentationDef : public CDefBase { public:
    unsigned char _pad08[0x3c - 0x08];
    float FrequencyToAddHealthInSeconds;                // +0x3c
    long  HealthToAddPerIncrease;                       // +0x40
    float FrequencyToAddStaminaInSeconds;               // +0x44
    long  StaminaToAddPerIncrease;                      // +0x48
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x0042B248 (T = CAugmentationDef)
};
class CThingBaseDef : public CDefClassBase { public: };
void* FindRef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body of the same-TU template instantiation (the real one is the landed 0x0042B248): it only writes a word through
// the reference parameter inline, so VC7.1 knows the caller's address-taken CDefPointer does not escape the call and can
// reuse its frame word for the rounding helper's by-value copy.
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CAugmentationDef>(CDefPointer<CAugmentationDef>& out) const
{
    void* p = FindRef_CDefClassBase_GetSubDef_0();
    if (!p)
        return false;
    *(void**)&out = p;
    return true;
}
#pragma auto_inline(on)
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CWorld { public: static long GetWorldFrame(); };          // 0x0049D870
extern long g_framesPerSecond;                          // 0x01375550
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}
class CTCObjectAugmentations { public:
    virtual void v00();
    unsigned char _pad04[0x14 - 0x04];
    vector< CDefPointer<CThingBaseDef> > Augmentations; // +0x14
    long GetHealthToAddThisFrame() const;
};

long CTCObjectAugmentations::GetHealthToAddThisFrame() const
{
    long total = 0;
    for (unsigned long i = 0; i < Augmentations.size(); ++i) {
        CThingBaseDef* def = Augmentations[i].Object;
        if (def) {
            CDefPointer<CAugmentationDef> slot;
            def->GetSubDef(slot);
            CAugmentationDef* aug = slot.Object;
            if (aug->FrequencyToAddHealthInSeconds > g_epsilon_0129ba3c) {
                long period = CeilToLong(g_framesPerSecond * aug->FrequencyToAddHealthInSeconds);
                if (CWorld::GetWorldFrame() % period == 0)
                    total += aug->HealthToAddPerIncrease;
            }
            ReleaseDef(aug);
        }
    }
    return total;
}