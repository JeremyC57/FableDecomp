#pragma optimize("s",on)
// Leaf: pair<unsigned_long,CAnimationEntry>_>_>
// _Destroy_range<std::allocator<std::pair<unsigned_long,CAnimationEntry>>>
// retail 004dbe73, __fastcall(first, last, allocator&)
// loop over [first,last) stride 0xc calling element dtor via thiscall.

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
struct CAnimEntryPair { unsigned long k; unsigned long a; unsigned long b; }; // 12 bytes

struct AllocDummy {};

void __fastcall _Destroy_range_pair(CAnimEntryPair* first, CAnimEntryPair* last, AllocDummy* al)
{
    (void)al;
    while (first != last)
    {
        ((CBaseIntelligentPointer*)(first))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
        ++first;
    }
}