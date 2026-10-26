// CTCHeroAttachableAppearanceModifiers::OnAppearanceClear() -- the appearance was cleared: if the modifiers were loaded, mark them unloaded, rebuild hidden modifiers / materials / graphics and flag the appearance changed; then empty the primitive-handle vector and mark it for reload
// Leaf: ?OnAppearanceClear@CTCHeroAttachableAppearanceModifiers@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00707970, length 106 bytes (+6 int3 padding).
// Callees: CTCHeroAttachableAppearanceModifiers::UpdateHiddenModifiers @ 0x00706040, UpdateHiddenMaterials @ 0x00706510,
// UpdateGraphics @ 0x007070D0, NGraphicAppearance::CTCGraphicAppearance::SetAsChanged(unsigned long) @ 0x004BF9E0 (mask 0xF),
// memmove @ 0x00BFEAE6 (the STLport __copy_trivial path of PrimHandles.clear()).
// Shape: OnAppearanceClear is a virtual of the secondary CAGraphicAppearanceComponent base at +0xc, so `this` arrives at that
// subobject (`lea esi,[edi-0Ch]` for the CTCHeroAttachableAppearanceModifiers member calls; member offsets 0xc lower).
// The loaded-arm accesses go through a `self = this` local: VC then keeps the FULL-object pointer in esi for that arm
// (`mov [esi+14h],al`, `mov eax,[esi+4]`) while the clear uses the subobject pointer in edi, as retail does.
// The clear is the landed STLport form `_M_finish = CopyTrivial(_M_finish, _M_finish, _M_start)` (retail's `cmp eax,eax`).
// Layout: retail FrameLastChanged +0x10, flag byte +0x14 (bit 0 AppearanceModifiersLoaded, bit 1 the primitive reload flag),
// PrimHandles +0x18 (12-byte vector); the parent thing's CTCGraphicAppearance pointer CThing +0x64.
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
namespace NGraphicAppearance {
class CPrimitive;
class CTCGraphicAppearance {
public:
    void SetAsChanged(unsigned long mask);      // 0x004BF9E0
};
class CAGraphicAppearanceComponent {
public:
    virtual void OnAppearanceClear();
};
}
struct CPrimitiveVector {                       // STLport vector<NGraphicAppearance::CPrimitive*>
    NGraphicAppearance::CPrimitive** _M_start;
    NGraphicAppearance::CPrimitive** _M_finish;
    NGraphicAppearance::CPrimitive** _M_end_of_storage;
    void clear()
    {
        NGraphicAppearance::CPrimitive** i = (NGraphicAppearance::CPrimitive**)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};
class CThing {
public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCHeroAttachableAppearanceModifiers : public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent {
public:
    unsigned long    FrameLastChanged;          // +0x10
    unsigned char    AppearanceModifiersLoaded : 1;   // +0x14 bit 0
    unsigned char    PrimHandlesNeedReload : 1;       // +0x14 bit 1
    CPrimitiveVector PrimHandles;               // +0x18
    void UpdateHiddenModifiers();               // 0x00706040
    void UpdateHiddenMaterials();               // 0x00706510
    void UpdateGraphics();                      // 0x007070D0
    virtual void OnAppearanceClear();
};

void CTCHeroAttachableAppearanceModifiers::OnAppearanceClear()
{
    CTCHeroAttachableAppearanceModifiers* self = this;
    if (self->AppearanceModifiersLoaded) {
        self->AppearanceModifiersLoaded = false;
        self->UpdateHiddenModifiers();
        self->UpdateHiddenMaterials();
        self->UpdateGraphics();
        self->ParentThing->PTCGraphicAppearance->SetAsChanged(0xf);
    }
    PrimHandles.clear();
    PrimHandlesNeedReload = true;
}