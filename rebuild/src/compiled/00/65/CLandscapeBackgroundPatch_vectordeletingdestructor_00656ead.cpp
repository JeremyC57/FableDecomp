#pragma optimize("s",on)
// Leaf: `vector_deleting_destructor'
// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector_deleting_destructor'
// retail 0x00656ead
//
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x10]; call 0x440c40        -> Sub::destroy on this+0x10
//   test byte ptr [esp+8],1                  -> deleting flag (stack arg)
//   je skip; push esi; call 0xbfe9bc; pop ecx-> operator delete(this)
//   mov eax,esi; pop esi; ret 4

struct Sub {
    void destroy();
};

class CLandscapeBackgroundPatch { public:
    char pad[0x10];
    Sub  sub;      // at +0x10
};

// operator delete @ 0xbfe9bc

// Sub::destroy @ 0x440c40 : __fastcall member, model as __fastcall(self)
void __fastcall Sub_destroy(Sub* self);

// __fastcall(self, uint flags) -> void*. this in ecx, flags on stack.
// Rewritten by harness to __fastcall; add a dummy edx so flags stays on stack.
void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    Sub_destroy(&self->sub);
    if (flags & 1)
        operator delete(self);
    return self;
}