struct Inner {
    char pad[0xb];
    unsigned char flags;
};
struct Held {
    Inner* p;
};
class C3DAnimation2 { public:
    char pad[0x40];
    Held* held;
};

bool __fastcall C3DAnimation2_HasRotation(C3DAnimation2* animation2)
{
    Held* h = animation2->held;
    if (h != 0) {
        if (h->p->flags & 3)
            return true;
    }
    return false;
}