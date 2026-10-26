class C3DVector { public: float x,y,z; };
// Leaf: IsNewDestinationGoingToResetNavigation
class CTCCreatureNavigation;

struct Sub {
    // virtual with slot7 (+0x1c) taking (CTCCreatureNavigation*, C3DVector*)
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual bool s7(CTCCreatureNavigation* thisNav, C3DVector* dest);
};

class CTCCreatureNavigation { public:
    void* pad0;
    Sub* sub;   // +4
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest)
{
    return self->sub->s7(self, dest);
}