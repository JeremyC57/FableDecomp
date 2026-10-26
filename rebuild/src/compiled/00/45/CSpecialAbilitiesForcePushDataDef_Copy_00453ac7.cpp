struct CSubField { int v; };
class CSpecialAbilitiesDrainLifeDataDef { public:
    unsigned char _pad_0x0[0x28]; // to 0x28
    CSubField sub; // +0x28
};
class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

void __fastcall CSpecialAbilitiesDrainLifeDataDef_Copy(CSpecialAbilitiesDrainLifeDataDef* self, int _edx, const CSpecialAbilitiesDrainLifeDataDef* other)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)other);
    self->sub = other->sub;
}