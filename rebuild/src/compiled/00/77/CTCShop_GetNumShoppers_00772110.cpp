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
// Identity (proven, chunk c675): Ego_r.pdb CTCShop::GetNumShoppers() const (long); FindASimplyDelightfulShop 0x00946F90 calls it
//   on the CTCShop component (TC 0x53). Shoppers = Ego_r vector<CIntelligentPointer<CThingCreatureBase> > (STLport 12 bytes at +0x18).
// Naming wave 17: landed as CShaderPreParser::GetConstantCount (shape label); respelled (name-only: the bytes are unchanged).
class CThingCreatureBase;
template <class T> class CIntelligentPointer {         // 8 bytes: vptr + PData
public:
    virtual ~CIntelligentPointer();
    T* PData;
};

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