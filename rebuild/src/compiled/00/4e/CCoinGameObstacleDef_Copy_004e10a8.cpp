
class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other);   // __fastcall: this in ecx, src on stack
};

class CCoinGameObstacleDef { public:
    char pad[0x28];
    unsigned long f28;
    void __fastcall Copy(int /*edx*/, CDefClassBase const *src);
};

void __fastcall CCoinGameObstacleDef::Copy(int /*edx*/, CDefClassBase const *src)
{
    ((CDefClassBase *)this)->operator=(*(const CDefClassBase*)(src));
    this->f28 = ((CCoinGameObstacleDef const *)src)->f28;
}