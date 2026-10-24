// CTCVillage::OnInitialActivate  0x0086e090
// Leaf: OnInitialActivate
struct CTCVillage;

// callee 0x86dd80: __fastcall(ecx, edx, stack byte)
extern int __fastcall sub_86dd80(void* ecx, void* edx, unsigned char b);

struct CTCVillage {
    void* m0;     // +0x00
    void* ParentThing;     // +0x04
    unsigned char ToBeRemoved;   // +0x08
    unsigned char IsInGlobalUpdateTCs;
    unsigned char ma;
    unsigned char mb;
    unsigned char mc;   // +0x0c
    unsigned char md;   // +0x0d
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->md) {
        sub_86dd80(self->m0, self->ParentThing, self->mc);
        self->md = 0;
    }
}