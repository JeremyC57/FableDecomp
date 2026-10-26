#pragma optimize("g",on)
// CBuyHouseDef::Copy @ 004e01a7
class CDefClassBase { public: char pad[0x40]; 
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

// Base class whose Copy is __fastcall (this=ecx, src on stack, EDX untouched).

class CBuyHouseDef { public:
    char pad[0x25];
    unsigned char Main; // 0x25
    void Copy(CDefClassBase const* src);
};

void __fastcall CBuyHouseDef_Copy(CBuyHouseDef* self, int /*edx*/, CDefClassBase const* src)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)(src));
    self->Main = ((const CBuyHouseDef*)src)->Main;
}