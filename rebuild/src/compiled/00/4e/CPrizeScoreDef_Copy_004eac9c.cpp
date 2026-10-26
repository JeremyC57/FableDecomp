#pragma optimize("s",on)
// CPrizeScoreDef::Copy @ 0x004eac9c
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
class CPrizeScoreDef;

// base class Copy at 0x431f10 : void __fastcall CDefClassBase::Copy(CDefClassBase* self, CDefClassBase const* src)

class CPrizeScoreDef { public:
    char pad[0x28];
    int f28;
    int Mult;
};

void __fastcall CPrizeScoreDef_Copy(CPrizeScoreDef* self, void* edx, CDefClassBase const* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    const CPrizeScoreDef* s = (const CPrizeScoreDef*)src;
    self->f28 = s->f28;
    self->Mult = s->Mult;
}