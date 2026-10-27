// CTCInventory::ProcessDPadRightReleased() -- D-pad-right release on the inventory screen: confirm a pending quick-access assignment into slot 3, else cancel a pending quick-access removal (clear slot 3), else the base behaviour
// Leaf: ?ProcessDPadRightReleased@CTCInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00587807, 64 bytes, thiscall, plain `ret` (base arm: tail `jmp`).
// Identity (proven): slot 65 of vtable 0x01250DE4 (RTTI .?AVCTCInventory@@); Ego_r.pdb ?ProcessDPadRightReleased@CTCInventory@@UAEXXZ;
//   the base arm jumps to NInventory::CTCInventoryBase::ProcessDPadRightReleased 0x005BC6AB (landed, base vtable slot 65).
//   Up / Down / Left / Right use quick-access slots 0 / 1 / 2 / 3 (landed ProcessDPadUpReleased 0x00587749 uses 0).
//   The manifest's vfunc_65 label is a placeholder.
// Callees: CTCInventory::SetHighlightedItemAsQuickAccessItem(long) 0x005850DF, CTCInventory::SetQuickAccessAssign(bool) 0x00581A14.
// Shape (family M146, representative 0x00587787): the landed ProcessDPadUpReleased 0x00587749 with slot 3; the base
//   call is qualified, so VC7.1 tail-jumps to it. `optimize("s", on)` (shared zero register).
//   Layout: retail QuickAccessItems at +0x148 and EnableQuickAccessAssign / Remove at +0x1CC / +0x1CD (PDB +0x168 / +0x204 / +0x205).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
namespace NInventory {
class CTCInventoryBase {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void ProcessDPadUpReleased();                 // slot 62
    virtual void ProcessDPadDownReleased();               // slot 63 (0x005BC69B)
    virtual void ProcessDPadLeftReleased();               // slot 64 (0x005BC6A3)
    virtual void ProcessDPadRightReleased();              // slot 65 (0x005BC6AB)
};
}
class CTCInventory : public NInventory::CTCInventoryBase {
public:
    unsigned char     _pad004[0x148 - 0x004];
    std::vector<long> QuickAccessItems;                   // +0x148 (PDB +0x168)
    unsigned char     _pad154[0x1CC - 0x154];
    bool              EnableQuickAccessAssign;            // +0x1CC (PDB +0x204)
    bool              EnableQuickAccessRemove;            // +0x1CD (PDB +0x205)
    void SetHighlightedItemAsQuickAccessItem(long slot);  // 0x005850DF
    void SetQuickAccessAssign(bool enable);               // 0x00581A14
    virtual void ProcessDPadRightReleased();
};
void CTCInventory::ProcessDPadRightReleased()
{
    if (EnableQuickAccessAssign) {
        SetHighlightedItemAsQuickAccessItem(3);
        SetQuickAccessAssign(false);
    } else if (EnableQuickAccessRemove) {
        QuickAccessItems[3] = 0;
        EnableQuickAccessRemove = false;
    } else {
        NInventory::CTCInventoryBase::ProcessDPadRightReleased();
    }
}