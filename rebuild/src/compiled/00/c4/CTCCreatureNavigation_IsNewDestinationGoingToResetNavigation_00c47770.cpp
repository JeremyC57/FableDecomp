class C3DVector { public: float x, y, z; };
class CTCCreatureNavigation;
struct IHelper {
    virtual int v0();
    virtual int v1();
    virtual int v2();
    virtual int v3();
    virtual int v4();
    virtual int v5();
    virtual bool v6(CTCCreatureNavigation* nav, C3DVector* dest);
};
class CTCCreatureNavigation { public:
    void* f0;
    IHelper* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return this->ParentThing->v6(this, dest);
}