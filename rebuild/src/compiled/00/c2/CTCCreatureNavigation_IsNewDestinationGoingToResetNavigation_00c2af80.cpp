class C3DVector { public: float x, y, z; };
// Leaf: IsNewDestinationGoingToResetNavigation

struct Sub {
    virtual bool m00();
    virtual bool m04();
    virtual bool m08();
    virtual bool m0c();
    virtual bool m10();
    virtual bool m14();
    virtual bool Check(void* origThis, C3DVector* pos);   // slot 6 -> +0x18
};

class CTCCreatureNavigation { public:
    void* f0;
    Sub*  ParentThing;
};

bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* pos)
{
    return self->ParentThing->Check(self, pos);
}