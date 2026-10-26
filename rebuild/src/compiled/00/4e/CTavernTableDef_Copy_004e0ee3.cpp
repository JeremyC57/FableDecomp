#pragma optimize("s",on)
// CTavernTableDef::Copy @ 0x004e0ee3
// void __fastcall Copy(CTavernTableDef* this, CDefClassBase const* src)

class CDefClassBase;

// base class with a __fastcall Copy at 0x431f10 (this in ecx, src pushed; no edx use)
struct CDefBase {
    unsigned char pad[0x25];               // bytes 0..0x24
    void Copy(CDefClassBase const* src);   // out-of-line, real __fastcall
};

class CTavernTableDef : public CDefBase { public:
    unsigned char AddTankards;   // +0x25
    unsigned char HighQualityTankards;   // +0x26
};

void __fastcall Copy(CTavernTableDef* self, void* /*edx*/, CDefClassBase const* src)
{
    self->CDefBase::Copy(src);
    CTavernTableDef* s = (CTavernTableDef*)src;
    self->AddTankards = s->AddTankards;
    self->HighQualityTankards = s->HighQualityTankards;
}