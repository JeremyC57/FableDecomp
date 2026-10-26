class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

class CWifeDef { public:
    char pad[0x28];
    int f28;
    // base-class copy at 0x431f10; member => thiscall(this, src)
};

void __fastcall CWifeDef_Copy(CWifeDef* self, int /*edx*/, const CDefClassBase* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    self->f28 = ((const CWifeDef*)src)->f28;
}