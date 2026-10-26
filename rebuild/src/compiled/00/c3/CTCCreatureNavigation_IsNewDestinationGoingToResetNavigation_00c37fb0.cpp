class C3DVector { public: float x, y, z; };
class CTCCreatureNavigation;
struct Helper {
    virtual void s0(); virtual void s1(); virtual void s2();
    virtual void s3(); virtual void s4(); virtual void s5();
    virtual bool s6(CTCCreatureNavigation* nav, C3DVector* v);
};
class CTCCreatureNavigation { public:
    void* field0;
    Helper* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return ParentThing->s6(this, dest);
}