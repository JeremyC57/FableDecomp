struct RefTarget {
    char pad0[0x10];
    unsigned long refs; // +0x10
};
class CPixelShader { public:
    char pad0[8];
    RefTarget* inner; // +0x08
};
unsigned long __fastcall CountReferences(CPixelShader* self) {
    return self->inner->refs;
}