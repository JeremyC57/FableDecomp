class C3DVector { public: float x, y, z; };
class CTCCreatureNavigation;

struct Sub {
    virtual int v0();
    virtual int v1();
    virtual int v2();
    virtual int v3();
    virtual int v4();
    virtual int v5();
    virtual int v6();
    virtual bool v7(CTCCreatureNavigation* nav, C3DVector* dest); // slot 0x1c
};

class CTCCreatureNavigation { public:
    void* f0;
    Sub* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
    return ParentThing->v7(this, dest);
}