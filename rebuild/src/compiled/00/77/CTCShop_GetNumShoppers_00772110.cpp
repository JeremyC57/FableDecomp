// CTCShop::GetNumShoppers() const -- the number of shoppers: Shoppers.size(), the 8-byte CIntelligentPointer elements at +0x18/+0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00772110, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x1c] ; sub eax, dword ptr [ecx + 0x18] ; sar eax, 3 ; ret 
// Tiny-body shape `GetCount` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
// Leaf: ?GetNumShoppers@CTCShop@@QBEJXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Identity (proven, chunk c675): Ego_r.pdb CTCShop::GetNumShoppers() const (long); FindASimplyDelightfulShop 0x00946F90 calls it
//   on the CTCShop component (TC 0x53). Shoppers = Ego_r vector<CIntelligentPointer<CThingCreatureBase> > (STLport 12 bytes at +0x18).
// Naming wave 17: landed as CShaderPreParser::GetConstantCount (shape label); respelled (name-only: the bytes are unchanged).
class CThingCreatureBase;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCShop { public:
    unsigned char _pad[0x18];
    CIntelligentPointer<CThingCreatureBase>* ShoppersStart;     // +0x18 Shoppers (STLport vector _M_start)
    CIntelligentPointer<CThingCreatureBase>* ShoppersFinish;    // +0x1c Shoppers._M_finish
    long GetNumShoppers() const;
};

long CTCShop::GetNumShoppers() const
{
    return ShoppersFinish - ShoppersStart;
}