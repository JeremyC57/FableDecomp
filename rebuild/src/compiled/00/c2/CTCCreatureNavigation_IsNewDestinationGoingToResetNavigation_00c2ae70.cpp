class C3DVector { public: float x, y, z; };
class CTCCreatureNavigation;

struct Inner {
    // vtable pointer at +0; slot at +0x18 is the 7th virtual (index 6)
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool doReset(CTCCreatureNavigation* self, C3DVector* v);
};

class CTCCreatureNavigation { public:
    void* f0;
    Inner* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return this->ParentThing->doReset(this, dest);
}