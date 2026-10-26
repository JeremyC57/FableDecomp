// CThingSwitch::`vector_deleting_destructor' @ 0x007692A0 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: `vector_deleting_destructor'
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x10 CBaseIntelligentPointer (~ @ 0x00A01C10), +0x4 CCharString (~ @ 0x0099EAE0), then +0x0 CCharString (~ @ 0x0099EAE0), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
class CCharString { public:                        // one word: CCharStringData*
    void* PStringData;
    ~CCharString();                         // 0x0099EAE0 (release @ 0x0099E9B0, instance count 0x013BD800)
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_007692a0 {
    CCharString Member0;
    CCharString Member4;                        // +0x4
    unsigned char _pad10[0x8];
    CBaseIntelligentPointer Member10;           // +0x10
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_007692a0* object = static_cast<CObject_007692a0*>(self);
    object->Member10.CBaseIntelligentPointer::~CBaseIntelligentPointer();
    object->Member4.~CCharString();
    object->Member0.~CCharString();
    if (flags & 1)
        operator delete(object);
    return self;
}
