#include <stdio.h>

class C3DVector { public:
    float x;
    float y;
    float z;
};

class CCombatAction_GenericStrikeResponseBase { public:
    char pad[0x118];                    // +0x04 (the vptr takes +0x00)
    C3DVector hitPos;

    virtual void SetHitPosition(C3DVector pos);   // the row's PDB UAE spelling
};

int main()
{
    CCombatAction_GenericStrikeResponseBase obj;
    for (int i = 0; i < (int)sizeof(obj); ++i) ((char*)&obj)[i] = 0;

    C3DVector v;
    v.x = 1.5f; v.y = -2.25f; v.z = 42.0f;

    obj.SetHitPosition(v);

    if (obj.hitPos.x == 1.5f && obj.hitPos.y == -2.25f && obj.hitPos.z == 42.0f) {
        printf("SETHITPOS_OK\n");
        return 0;
    }
    printf("SETHITPOS_FAIL\n");
    return 1;
}