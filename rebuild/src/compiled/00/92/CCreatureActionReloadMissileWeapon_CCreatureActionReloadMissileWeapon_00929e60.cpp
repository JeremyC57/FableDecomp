struct Sub { char pad[4]; };
struct Obj {
    char head[0xa8];
    Sub  member;
};

extern void __fastcall SubCleanup(Sub* s);
extern void __fastcall FinalDtor(Obj* o);

void __fastcall DestVal_A8_E(Obj* action)
{
    SubCleanup(&action->member);
    FinalDtor(action);
}