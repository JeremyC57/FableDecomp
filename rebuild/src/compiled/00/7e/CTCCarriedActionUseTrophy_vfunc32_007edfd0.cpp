// CTCCarriedActionUseTrophy::vfunc_32(CThing* user) -- runs the base carried-action hook, then the trophy's shared use handler
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EDFD0, length 27 bytes. CTCCarriedActionUseTrophy vtable 0x0123FEF4 slot 32.
// Callees: the base hook @ 0x007EAB90 (an empty `ret 4` body defined in another translation unit),
// CTCCarriedActionUseTrophy::OnActionUseCommon @ 0x007EDEE0.
// Shape: byte-identical to slot 28 (0x007EDFB0): user argument in edi pushed twice, `this` in esi, two
// `__fastcall` calls on `this`.
struct CThing;

struct CTCCarriedActionUseBase {
    virtual void v00();
    void OnUseBase(CThing* user);                   // 0x007EAB90 (empty base implementation)
};

struct CTCCarriedActionUseTrophy : CTCCarriedActionUseBase {
    void OnActionUseCommon(CThing* user);           // 0x007EDEE0
    void vfunc_32(CThing* user);
};

void CTCCarriedActionUseTrophy::vfunc_32(CThing* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    OnActionUseCommon(user);
}