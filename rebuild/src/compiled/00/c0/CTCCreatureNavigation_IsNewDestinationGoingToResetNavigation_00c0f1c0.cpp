class C3DVector { public: float x, y, z; };
class CTCCreatureNavigation;

struct Helper {
    virtual void m00();
    virtual void m01();
    virtual void m02();
    virtual void m03();
    virtual void m04();
    virtual void m05();
    virtual void m06();
    virtual bool m07(CTCCreatureNavigation* nav, C3DVector* dest);
};

class CTCCreatureNavigation { public:
    void* field_0;
    Helper* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    Helper* h = this->ParentThing;
    return h->m07(this, dest);
}