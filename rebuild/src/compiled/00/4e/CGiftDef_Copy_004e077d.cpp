#pragma optimize("s",on)
// CGiftDef::Copy @ 0x004e077d
class CDefClassBase;

class CDefClassBase { public:
    // base copy is a real thiscall member: this in ecx, src on stack
    void Copy(const CDefClassBase* src);
    char basepad[0x28];
};

class CGiftDef : public CDefClassBase { public:
    unsigned int Type;   // +0x28
    unsigned char IsWeddingRing;  // +0x2c
    void Copy(const CDefClassBase* src);
};

void CGiftDef::Copy(const CDefClassBase* src)
{
    CDefClassBase::Copy(src);
    const CGiftDef* s = (const CGiftDef*)src;
    this->Type = s->Type;
    this->IsWeddingRing = s->IsWeddingRing;
}