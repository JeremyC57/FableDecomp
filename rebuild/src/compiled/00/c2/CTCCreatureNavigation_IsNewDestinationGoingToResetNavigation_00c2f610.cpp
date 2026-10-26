class C3DVector { public: float x, y, z; };
class CTCCreatureNavigation;

struct Helper {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual bool check(CTCCreatureNavigation* nav, C3DVector* v);
};

class CTCCreatureNavigation { public:
    void* field_0;
    Helper* helper;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* v);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* v) {
    return this->helper->check(this, v);
}