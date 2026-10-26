class C3DVector { public: float x, y, z; };
class CTCCreatureNavigation;

struct Helper {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool v6(CTCCreatureNavigation* nav, C3DVector* dest);
};

class CTCCreatureNavigation { public:
    void* field_0;
    Helper* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return this->ParentThing->v6(this, dest);
}