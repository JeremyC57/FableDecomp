struct C3DVector { float x, y, z; };
// Leaf: IsNewDestinationGoingToResetNavigation
struct CTCCreatureNavigation;

struct Delegate {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual bool v7(CTCCreatureNavigation* nav, C3DVector* vec);
};

struct CTCCreatureNavigation {
    void* f0;
    Delegate* ParentThing;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int edx, C3DVector* vec)
{
    return self->ParentThing->v7(self, vec);
}