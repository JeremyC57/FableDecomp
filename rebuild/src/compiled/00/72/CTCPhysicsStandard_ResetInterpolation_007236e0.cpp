// CTCPhysicsStandard::ResetInterpolation @ 007236e0
struct Xform6 { unsigned int m[6]; };
struct CTCPhysicsStandard {
    char pad0[0x50];
    Xform6 cur;         // +0x50
    Xform6 interp;      // +0x68
    unsigned int flag;  // +0x80
};

extern void __fastcall CTCPhysicsStandard_helper(CTCPhysicsStandard* standard);
extern unsigned int __cdecl CTCPhysicsStandard_getFlag(void);
extern void __fastcall CTCPhysicsStandard_tail(CTCPhysicsStandard* standard);

void __fastcall CTCPhysicsStandard_ResetInterpolation(CTCPhysicsStandard* standard)
{
    standard->interp = standard->cur;
    CTCPhysicsStandard_helper(standard);
    standard->flag = CTCPhysicsStandard_getFlag();
    CTCPhysicsStandard_tail(standard);
}