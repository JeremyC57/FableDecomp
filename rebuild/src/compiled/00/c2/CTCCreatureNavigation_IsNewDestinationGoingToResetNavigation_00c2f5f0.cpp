class C3DVector { public: float x, y, z; };

class CTCCreatureNavigation;

struct NavDelegate {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual bool v7(CTCCreatureNavigation* nav, C3DVector* dest);
};

class CTCCreatureNavigation { public:
    void* pad0;
    NavDelegate* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return ParentThing->v7(this, dest);
}