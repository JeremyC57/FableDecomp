// CTCCarriedActionUseTrophy::vfunc_28(CThing* user) -- runs the base carried-action hook, then the trophy's shared use handler
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EDFB0, length 27 bytes. CTCCarriedActionUseTrophy vtable 0x0123FEF4 slot 28.
// Callees: the base hook @ 0x007EAB90 (an empty `ret 4` body, called on `this` because it is defined in another
// translation unit; its manifest label "$E2" is meaningless), CTCCarriedActionUseTrophy::OnActionUseCommon @ 0x007EDEE0.
// Shape: the user argument is loaded once into edi and pushed twice, `this` kept in esi; both calls are
// `__thiscall` members on `this`. The body is byte-identical to slot 32 (0x007EDFD0): the linker did not
// fold them.
struct CThing;

struct CTCCarriedActionUseBase {
    virtual void v00();
    void OnUseBase(CThing* user);                   // 0x007EAB90 (empty base implementation)
};

struct CTCCarriedActionUseTrophy : CTCCarriedActionUseBase {
    void OnActionUseCommon(CThing* user);           // 0x007EDEE0
    void vfunc_28(CThing* user);
};

void CTCCarriedActionUseTrophy::vfunc_28(CThing* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    OnActionUseCommon(user);
}
