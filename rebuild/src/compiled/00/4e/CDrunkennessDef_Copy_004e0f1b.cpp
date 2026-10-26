class CDefClassBase { public:
    char pad[0x28];
    unsigned long field28;

    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

class CDrunkennessDef : public CDefClassBase { public:
};

// Base-class copy — real target of the rel32 call. this in ECX, src on stack.

void __fastcall CDrunkennessDef_Copy(CDrunkennessDef* self, int edx, CDefClassBase const* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    self->field28 = src->field28;
}