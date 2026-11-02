// test: CTCFlashOnHit::~CTCFlashOnHit -- remove this component's tint from the parent's graphic appearance only if the parent exists and is not dead
#include <stdio.h>
struct CTCGraphicAppearance { void* removed; int count; void RemoveTintColour(void* id) { removed = id; ++count; } };
class CThing { public:
    bool Dead; CTCGraphicAppearance* PTCGraphicAppearance;
    bool IsDead() const { return Dead; }
    CTCGraphicAppearance* GetTCGraphicAppearance() const { return PTCGraphicAppearance; }
};
static int baseDtors = 0;
class CTCBase { public: CThing* ParentThing; virtual ~CTCBase() { ++baseDtors; } };
class CTCFlashOnHit : public CTCBase { public:
    ~CTCFlashOnHit() {
        if (ParentThing && !ParentThing->IsDead()) ParentThing->GetTCGraphicAppearance()->RemoveTintColour(this);
    }
};
int main() {
    CTCGraphicAppearance ga = { 0, 0 };
    CThing alive = { false, &ga }, dead = { true, &ga };
    void* id;
    { CTCFlashOnHit f; f.ParentThing = &alive; id = &f; }
    if (ga.count != 1 || ga.removed != id || baseDtors != 1) { printf("007e3420_TEST FAIL\n"); return 1; }
    { CTCFlashOnHit f; f.ParentThing = &dead; }
    { CTCFlashOnHit f; f.ParentThing = 0; }
    if (ga.count != 1 || baseDtors != 3) { printf("007e3420_TEST FAIL\n"); return 1; }
    printf("007e3420_TEST PASS\n");
    return 0;
}