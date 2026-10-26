struct Foo { long Method(long arg); };
struct Sub { };
__declspec(noinline) long __fastcall Sub_Helper(Sub* s);

class CCreatureActionBase { public:
    char pad0[8];
    Sub  sub;
    char pad1[0x74 - 8 - sizeof(Sub)];
    Foo* f74;
};

long __fastcall GetAnimationTransitionInTime(CCreatureActionBase* action)
{
    Foo* f = action->f74;
    long r = Sub_Helper(&action->sub);
    return f->Method(r);
}