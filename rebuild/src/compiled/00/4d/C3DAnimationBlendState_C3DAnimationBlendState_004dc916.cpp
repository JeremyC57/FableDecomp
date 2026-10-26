#pragma optimize("s",on)
// Leaf: ??1C3DAnimationBlendState@
// Byte-exact reconstruction of C3DAnimationBlendState::~C3DAnimationBlendState @ 0x004dc916

class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
struct RefCounted {
    void** vtable;   // +0
    int    refCount; // +4
};

struct BaseBlendState;
// base destructor tail-called at 0x686830

class C3DAnimationBlendState { public:
    char        pad0[0xc];   // +0
    RefCounted* handle;      // +0xc
};

void __fastcall dtor(C3DAnimationBlendState* self)
{
    RefCounted* h = self->handle;
    if (h) {
        if (--h->refCount == 0) {
            // virtual slot at [vtable+4]
            typedef void (__fastcall *Fn)(RefCounted*);
            ((Fn)h->vtable[1])(h);
        }
        self->handle = 0;
    }
    ((CTCBase*)((BaseBlendState*)self))->CTCBase::~CTCBase();
}