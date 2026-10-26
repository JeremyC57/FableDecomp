#include <new>
// Leaf: OnDie

extern void sink(void*);

class CTCMapwho { public:
    int dummy;
    __declspec(noinline) ~CTCMapwho();
};

__declspec(noinline) CTCMapwho::~CTCMapwho() { sink(this); }

void __fastcall OnDie(CTCMapwho* self)
{
    if (self) {
        self->~CTCMapwho();
        ::operator delete(self);
    }
}