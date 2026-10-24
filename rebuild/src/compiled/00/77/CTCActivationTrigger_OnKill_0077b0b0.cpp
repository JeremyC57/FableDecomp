struct CTCActivationTrigger;
// Leaf: OnKill

// helper at 0x7f7750: called with ecx = this->fc, one stack arg = this (__fastcall)
struct Sub {
    void destroy(CTCActivationTrigger* owner);
};

struct CTCActivationTrigger {
    char pad[0xc];
    Sub* PTCReceptor;   // +0xc
};

void __fastcall CTCActivationTrigger_OnKill(CTCActivationTrigger* self)
{
    if (self->PTCReceptor) {
        self->PTCReceptor->destroy(self);
        self->PTCReceptor = 0;
    }
}