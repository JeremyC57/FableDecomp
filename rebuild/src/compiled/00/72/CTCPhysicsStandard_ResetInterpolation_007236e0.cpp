// CTCPhysicsStandard::ResetInterpolation @ 007236e0
struct Xform6 { unsigned int m[6]; };
struct CTCPhysicsStandard {
    char pad0[0x50];
    Xform6 cur;         // +0x50
    Xform6 interp;      // +0x68
    unsigned int flag;  // +0x80
};

extern void __fastcall CTCPhysicsStandard_helper(CTCPhysicsStandard* standard);
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};   // 0x0049D870
extern void __fastcall CTCPhysicsStandard_tail(CTCPhysicsStandard* standard);

void __fastcall CTCPhysicsStandard_ResetInterpolation(CTCPhysicsStandard* standard)
{
    standard->interp = standard->cur;
    CTCPhysicsStandard_helper(standard);
    standard->flag = CWorld::GetFrame();
    CTCPhysicsStandard_tail(standard);
}