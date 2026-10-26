// CTCGraphicAppearance::SetAlpha  @ 004bfac0
class CThing { public:
public:
    void AddToRenderList();   // 0x004C7AC0
};

struct CTCGraphicAppearance {
    char pad0[4];        // +0x00
    CThing* ParentThing;             // +0x04
    char pad8[0x58 - 8]; // +0x08 .. +0x57
    unsigned char flags; // +0x58
    char pad59;          // +0x59
    unsigned char alpha; // +0x5a
    void SetAlpha(unsigned char a);
};

void CTCGraphicAppearance::SetAlpha(unsigned char a)
{
    this->alpha = a;
    this->ParentThing->AddToRenderList();
    this->flags |= 4;
}