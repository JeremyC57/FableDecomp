class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
class CAbilityDef { public:
    unsigned char _tail[0x2c];
};

void __fastcall CAbilityDef_Copy(CAbilityDef* self, void* dummy, const CAbilityDef* other)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(other));
    *(unsigned long*)(self->_tail + 0x28) = *(const unsigned long*)(other->_tail + 0x28);
}