class C3DVector { public: float x,y,z; };

struct Inner {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual bool v7(void* a, C3DVector* v);  // slot 7 -> +0x1c
};

class CTCCreatureNavigation { public:
    int f0;
    Inner* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* vec);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* vec)
{
    Inner* n = this->ParentThing;
    return n->v7(this, vec);
}