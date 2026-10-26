#include "rebuild_abi.h"
// Leaf: CRegisteredMusicEntry>_>

// _Dest_val<std::allocator<...>,...> @ 0x008fe006
// Destructor forwarder: destroys the embedded member at +0x14, then tail-jumps
// to the base/next destructor with this in ecx. Shape:
//   push esi; mov esi,ecx; lea ecx,[esi+0x14]; call member_dtor;
//   mov ecx,esi; pop esi; jmp base_dtor.
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
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();   // 0x008FD288
};
struct FableDestMember_008fe006;
struct FableDestHost_008fe006
{
    char pad[0x14];
    FableDestMember_008fe006* member_at_14;
};


extern "C" void FABLE_FASTCALL FableDestVal_008fe006(FableDestHost_008fe006* self)
{
    ((CBaseIntelligentPointer*)((FableDestMember_008fe006*)&self->member_at_14))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((CAIStateGroupBase*)(self))->CAIStateGroupBase::~CAIStateGroupBase();
}