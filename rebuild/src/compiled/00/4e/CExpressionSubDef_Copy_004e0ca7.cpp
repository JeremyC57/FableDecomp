class CDefClassBase { public: int base[10]; int f28; 
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

class CExpressionSubDef { public:
    int base[10];
    int f28;
    // base class copy: real __fastcall member (this in ECX, src pushed on stack)
};

void __fastcall CExpressionSubDef_Copy(CExpressionSubDef* self, int /*edx*/, const CDefClassBase* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    self->f28 = ((const CExpressionSubDef*)src)->f28;
}