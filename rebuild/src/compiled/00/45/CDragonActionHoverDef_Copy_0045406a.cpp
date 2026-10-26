class CDefClassBase { public:
    unsigned char _pad_0x0[0x28];
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
class CDragonActionHoverDef : public CDefClassBase { public:
    int field28; // +0x28
};
void __fastcall CDragonActionHoverDef_Copy(CDragonActionHoverDef* self, void* edx, const CDragonActionHoverDef* other)
{
    self->CDefClassBase::operator=(*(const CDefClassBase*)(other));
    self->field28 = other->field28;
}