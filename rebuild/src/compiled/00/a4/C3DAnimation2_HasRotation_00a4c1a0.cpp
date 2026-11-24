// Leaf: ?HasRotation@C3DAnimation2@@UBE_NXZ
struct Inner {
    char pad[0xb];
    unsigned char flags;
};
struct Held {
    Inner* p;
};
class C3DAnimation2 { public:
    char pad[0x3c];
    Held* held;
    public: virtual bool HasRotation() const;
};

bool C3DAnimation2::HasRotation() const
{
    C3DAnimation2* animation2 = const_cast<C3DAnimation2*>(this);
    Held* h = animation2->held;
    if (h != 0) {
        if (h->p->flags & 3)
            return true;
    }
    return false;
}