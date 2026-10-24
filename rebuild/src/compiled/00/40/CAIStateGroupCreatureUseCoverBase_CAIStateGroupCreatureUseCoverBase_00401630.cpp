// CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase() -- destructor: tear down the member at +0x18, then the base
// Leaf: ??1CAIStateGroup_CreatureUseCoverBase@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401630, 30 bytes. Callees: sub_5992f0, sub_599ee0, base destructor.
//
// Ordinary compiler-shaped destructor for a small polymorphic object. The vtable is pointed back at this
// class's table (0x0122D06C) so virtual calls made during teardown resolve here, a helper is run against
// the whole object, the member living at +0x18 is destroyed, and finally the base-class destructor runs.
// Nineteen call sites reach this body, and the vector-deleting destructor at 0x004016B0 shares the same
// vtable, so this appears to be a shared destructor body rather than one unique to the AI state group
// whose name is attached to it.
//
// Matching: `vtbl` is written as an immediate before any callee; all three callees take `this`
// (or `&member18`) in ecx; the base destructor is an ordinary call, not a tail jump.
struct Sub;
// Helper run over the whole object before members are destroyed.
extern void __fastcall sub_5992f0(void* self);
// Destructor of the member sub-object at +0x18.
extern void __fastcall sub_599ee0(void* member);
// Base-class destructor.
extern void __fastcall base_dtor_0099a300(void* self);

struct CAIStateGroup_CreatureUseCoverBase {
    unsigned int vtbl;      // vtable pointer, reset to this class's table on entry
    char pad[0x14];         // base-class state (untouched here)
    char member18[4];       // owned sub-object destroyed by sub_599ee0
};

void __fastcall CAIStateGroup_CreatureUseCoverBase_dtor(CAIStateGroup_CreatureUseCoverBase* self)
{
    // Virtual calls during teardown must land on this class, not a derived one.
    self->vtbl = 0x122d06c;
    sub_5992f0(self);
    // Destroy the owned member, then let the base clean up.
    sub_599ee0(&self->member18);
    base_dtor_0099a300(self);
}
