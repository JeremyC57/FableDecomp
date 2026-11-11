// CTCCarriedActionUseTrophy::OnActionUse(CThing& user) [Ego_r.pdb; was vfunc_28(CThing*)] -- runs the base carried-action hook, then the trophy's shared use handler
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EDFB0, length 27 bytes. CTCCarriedActionUseTrophy vtable 0x0123FEF4 slot 28.
// Callees: the base hook @ 0x007EAB90 (an empty `ret 4` body, called on `this` because it is defined in another
// translation unit; its manifest label "$E2" is meaningless), CTCCarriedActionUseTrophy::OnActionUseCommon @ 0x007EDEE0.
// Shape: the user argument is loaded once into edi and pushed twice, `this` kept in esi; both calls are
// `__fastcall` members on `this`. The body is byte-identical to slot 32 (0x007EDFD0): the linker did not
// fold them.
class CThing;

class CThing;
class CTCActionUseBase { public:                 // Ego_r.pdb base of CTCCarriedActionUseBase (row-local view: the one member called)
    virtual void OnActionUse(CThing& user);   // 0x007EAB90 Ego_r.pdb ?OnActionUse@CTCActionUseBase@@UAEXAAVCThing@@@Z (empty `ret 4`, RTTI slot 28; was OnUseBase(CThing*))
};
class CTCCarriedActionUseBase : public CTCActionUseBase { public:
    virtual void v00();
};

struct CTCCarriedActionUseTrophy : CTCCarriedActionUseBase {
    void OnActionUseCommon(CThing* user);           // 0x007EDEE0
    virtual void OnActionUse(CThing& user);   // Ego_r.pdb (UAE; RTTI slot 28, was vfunc_28(CThing*))
};

void CTCCarriedActionUseTrophy::OnActionUse(CThing& thing)
{
    CThing* user = (CThing*)&thing;
    CTCCarriedActionUseBase::OnActionUse(thing);
    OnActionUseCommon(user);
}