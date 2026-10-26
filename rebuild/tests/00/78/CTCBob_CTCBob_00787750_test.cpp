// Behaviour test for 00787750 CTCBob::CTCBob(CThing*): the owner is stored and told to use cutscene behaviour 2.
#include <cstdio>

class CThing
{ public:
    long behaviour;
    void SetCutscenePauseBehaviour(long b) { behaviour = b; }
};

class CTCBase
{ public:
    virtual void v00() {}
    CThing* ParentThing;
    CTCBase(CThing* owner) : ParentThing(owner) {}
    virtual ~CTCBase() {}
};

class CTCBob : public CTCBase
{ public:
    float Rhset[6];
    float Pos[3];
    bool  ForceVisible;
    virtual void v00() {}
    CTCBob(CThing* owner) : CTCBase(owner) { ParentThing->SetCutscenePauseBehaviour(2); }
};

int main()
{
    CThing thing; thing.behaviour = 0;
    CTCBob bob(&thing);
    if (bob.ParentThing != &thing || thing.behaviour != 2) { std::printf("00787750_TEST FAIL\n"); return 1; }
    std::printf("00787750_TEST PASS\n");
    return 0;
}