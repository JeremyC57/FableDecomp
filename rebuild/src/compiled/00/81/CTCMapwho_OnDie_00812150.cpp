struct CTCMapwho { void Cleanup(); };
// Leaf: OnDie
extern "C" void __fastcall Helper_die(CTCMapwho* self);
void operator delete(void*);
void __fastcall OnDie(CTCMapwho* self)
{
    if (self) {
        Helper_die(self);
        operator delete(self);
    }
}