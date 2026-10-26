class C3DVector { public: float x, y, z; };
// Leaf: IsNewDestinationGoingToResetNavigation
class CTCCreatureNavigation;

struct Delegate {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool Check(CTCCreatureNavigation* orig, C3DVector* pos);
};

class CTCCreatureNavigation { public:
    void* f0;
    Delegate* ParentThing;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int edx, C3DVector* pos)
{
    return self->ParentThing->Check(self, pos);
}