#pragma optimize("s",on)
// Leaf: OnDie
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x0045a133

class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
struct Sub3c;
struct Owner48;

// callee at 0xbfea14: cdecl, takes a pointer, cleaned by caller (pop ecx)
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// member call on subobject at +0x3c (0x45180f), __fastcall -> __fastcall
extern void __fastcall Sub3c_Notify(Sub3c* self);

class CTCInventoryItem { public:
    char pad[0x3c];
    Sub3c* sub3c_marker; // subobject region begins at +0x3c
};

// base OnDie tail-called at 0x430300

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    Owner48* p = *(Owner48**)((char*)self + 0x48);
    if (p) {
        free(p);
    }
    Sub3c_Notify((Sub3c*)((char*)self + 0x3c));
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}