// Retail 0x008fd56a: _Dest_val<pair<long,CRegisteredMusicEntry>> style destroy helper.
// Leaf: CRegisteredMusicEntry>_>
// push esi; mov esi,ecx; lea ecx,[esi+0x34]; call 0xa01c10; mov ecx,esi; pop esi; jmp 0x8fd544
// VOID member: destroy the sub-object at this+0x34, then tail-call the base/other destroy on this.

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
extern "C" void __fastcall tail_8fd544(void* self);      // jmp  0x8fd544 (ecx = this)

extern "C" void __fastcall _Dest_val_pair_long_CRegisteredMusicEntry(void* self)
{
    ((CBaseIntelligentPointer*)((char*)self + 0x34))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    tail_8fd544(self);
}