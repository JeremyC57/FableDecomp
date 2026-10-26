#pragma optimize("s",on)
class CDefClassBase { public:
    int pad[10];   // 0x00..0x27
    int m28;       // 0x28
    int m2c;       // 0x2c
    CDefClassBase& operator=(const CDefClassBase& other);  // thiscall, at 0x431f10
};

class CHeroDef : public CDefClassBase { public:
};

void __fastcall CHeroDef_Copy(CHeroDef* self, int edx_ignored, CDefClassBase const* src)
{
    self->CDefClassBase::operator=(*(const CDefClassBase*)(src));
    self->m28 = src->m28;
    self->m2c = src->m2c;
}