// CInterestingToVillagersDef::Copy @ 004e0e0a
// void __fastcall Copy(CInterestingToVillagersDef* this, CDefClassBase const* src)
// Calls base Copy(src) [this=ecx, src pushed on stack, edx untouched],
// then copies DWORD field at +0x28.

class CDefClassBase { public:
    char pad[0x28];
    unsigned long field28;

    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

// Base class with a plain (implicit __fastcall under /Gd) member:
// this -> ecx, src -> pushed. Leaf name avoids "Copy".
struct BaseDef {
};

class CInterestingToVillagersDef : public BaseDef { public:
    char pad[0x28 - sizeof(BaseDef)];
    unsigned long field28;
};

void __fastcall CInterestingToVillagersDef__Copy(CInterestingToVillagersDef* self, void* /*edx*/, CDefClassBase const* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    self->field28 = src->field28;
}