class C3DVector { public: float x, y, z; };

struct Callee {
    virtual bool s0();
    virtual bool s1();
    virtual bool s2();
    virtual bool s3();
    virtual bool s4();
    virtual bool s5();
    virtual bool Query(void* nav, C3DVector* v);
};

class CTCCreatureNavigation { public:
    void* f0;
    Callee* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return ParentThing->Query(this, dest);
}