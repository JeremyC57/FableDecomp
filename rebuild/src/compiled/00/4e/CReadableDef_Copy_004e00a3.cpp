// CReadableDef::Copy @ 004e00a3
class CDefClassBase { public: char pad[0x40]; };

struct CReadableBase {
    void Copy(CDefClassBase const* src); // __fastcall base at 0x431f10
};

class CReadableDef : public CReadableBase { public:
    unsigned char field25pad[0x25];
    unsigned char UseInfoDisplay; // 0x25
    void Copy(CDefClassBase const* src);
};

void __fastcall CReadableDef_Copy(CReadableDef* self, int /*edx*/, CDefClassBase const* src)
{
    self->CReadableBase::Copy(src);
    self->UseInfoDisplay = ((const CReadableDef*)src)->UseInfoDisplay;
}