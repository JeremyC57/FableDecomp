// CTCNymph::IsDecentAttackRange(const C3DVector& target) -- the nymph is a decent attack position when it is more than 0.5 above the target and its horizontal distance lies strictly between MinAttackRange and CircleRange
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080F190, length 122 bytes (`ret 4`).
// Callees: CThing::GetPos @ 0x004C73D0 (catalog "CVertexBufferWin32::DoSizeof" is a byte twin).
// Layout: PDB CTCNymph PDef +0x0C (CNymphDef: MinAttackRange +0x28, CircleRange +0x34); 0.5f @ 0x0122F59C.
// Shape: the delta `pos - target` is built on the x87 stack (all three differences kept), the squared ranges compared with
// `!(a <= b)` style tests (`test ah,41h ; jne`) and the upper bound strict (`test ah,5 ; jp`); nested ifs, one false exit.
#include "engine/C3DVector.h"
class CThing { public:
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CNymphDef { public:
    unsigned char _pad00[0x28];
    float MinAttackRange;                       // +0x28
    float MaxHeight;                            // +0x2c
    float MinHeight;                            // +0x30
    float CircleRange;                          // +0x34
};
template<class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CTCNymph { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    CDefPointer<const CNymphDef> PDef;          // +0x0c
    bool IsDecentAttackRange(const C3DVector& target);
};

bool CTCNymph::IsDecentAttackRange(const C3DVector& target)
{
    C3DVector delta = ParentThing->GetPos() - target;
    if (delta.Z > 0.5f) {
        float distanceSq = delta.X * delta.X + delta.Y * delta.Y;
        if (distanceSq > PDef->MinAttackRange * PDef->MinAttackRange) {
            if (distanceSq < PDef->CircleRange * PDef->CircleRange)
                return true;
        }
    }
    return false;
}