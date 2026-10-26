// CReadableDef::Copy @ 004e00a3
class CDefClassBase { public: char pad[0x40]; 
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CReadableBase {
};

class CReadableDef : public CReadableBase { public:
    unsigned char field25pad[0x25];
    unsigned char UseInfoDisplay; // 0x25
    void Copy(CDefClassBase const* src);
};

void __fastcall CReadableDef_Copy(CReadableDef* self, int /*edx*/, CDefClassBase const* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    self->UseInfoDisplay = ((const CReadableDef*)src)->UseInfoDisplay;
}