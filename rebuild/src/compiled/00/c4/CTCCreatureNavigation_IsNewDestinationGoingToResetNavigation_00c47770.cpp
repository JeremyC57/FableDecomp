struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct IHelper {
    virtual int v0();
    virtual int v1();
    virtual int v2();
    virtual int v3();
    virtual int v4();
    virtual int v5();
    virtual bool v6(CTCCreatureNavigation* nav, C3DVector* dest);
};
struct CTCCreatureNavigation {
    void* f0;
    IHelper* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return this->ParentThing->v6(this, dest);
}