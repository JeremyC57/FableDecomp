#pragma optimize("s",on)
// Leaf: OnDie
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
struct SubA { char x; };
struct SubB { char x; };
#pragma pack(1)
class CTCInventoryItem { public:
    unsigned char _pad_0x0[0x3c];
    SubB sub3c;             // +0x3c
    unsigned char _pad_0x3d[0x13];
    SubA sub50;             // +0x50
    unsigned char _pad_0x51[0x1b];
    void* ptr6c;            // +0x6c
};
#pragma pack()
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
extern void __fastcall SubA_dtor(SubA* self);
extern void __fastcall SubB_dtor(SubB* self);

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr6c)
        free(self->ptr6c);
    SubA_dtor(&self->sub50);
    SubB_dtor(&self->sub3c);
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}