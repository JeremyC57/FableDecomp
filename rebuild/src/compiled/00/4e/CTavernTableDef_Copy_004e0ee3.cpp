#pragma optimize("s",on)
// CTavernTableDef::Copy @ 0x004e0ee3
// void __fastcall Copy(CTavernTableDef* this, CDefClassBase const* src)


// base class with a __fastcall Copy at 0x431f10 (this in ecx, src pushed; no edx use)
class CDefClassBase { public:
    unsigned char pad[0x25];               // bytes 0..0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // out-of-line, real __fastcall
};

class CTavernTableDef : public CDefClassBase { public:
    unsigned char AddTankards;   // +0x25
    unsigned char HighQualityTankards;   // +0x26
};

void __fastcall Copy(CTavernTableDef* self, void* /*edx*/, CDefClassBase const* src)
{
    self->CDefClassBase::operator=(*(const CDefClassBase*)(src));
    CTavernTableDef* s = (CTavernTableDef*)src;
    self->AddTankards = s->AddTankards;
    self->HighQualityTankards = s->HighQualityTankards;
}