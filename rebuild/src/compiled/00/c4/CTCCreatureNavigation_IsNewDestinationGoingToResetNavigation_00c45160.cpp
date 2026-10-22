struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;

struct Helper {
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4();
    virtual void m5();
    virtual bool m6(CTCCreatureNavigation* nav, C3DVector* v); // slot 6 = +0x18
};

struct CTCCreatureNavigation {
    void* f0;
    Helper* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* v);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* v)
{
    return ParentThing->m6(this, v);
}