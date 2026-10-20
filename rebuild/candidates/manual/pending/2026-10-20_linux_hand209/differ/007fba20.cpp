// Grade: DIFFER(222v222 ours v retail) -- register assignment: retail keeps `this` in ebp and re-reads the explosion argument from its slot for the driver test (three loads of [esp+1Ch]); the lane caches the argument in ebp and spills `this` to the frame; the push_back / insert(end(), x) / word-test forms all give this
// CTCKraken::AddStrikeExplosion(CThing* explosion) -- remembers a tentacle-strike explosion in the explosion list and tells its driver to leave the kraken and every living tentacle unharmed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FBA20, length 224 bytes (`ret 4`, + int3 padding); speed TU (no frame pointer).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90, GetPItem @ 0x00A01B50 and
// ~CBaseIntelligentPointer @ 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), malloc @ 0x00BFEA0E (the STLport list node),
// CExplosionDriver::AddExcludedThing(CThing*) @ 0x0080CBF0.
// Layout: retail CTCKraken ParentThing +0x04, ExplosionList +0x30 and Tentacles +0x44 (PDB +0x30/+0x4C: 4-byte list heads; node
// {next, prev, CIntelligentPointer}, 16 bytes). CThing: components bitset +0x20, PTCDriver +0x5C (PDB +0x6C), Dead = +0x91 bit 0.
// Shape: the explosion is wrapped in a local intelligent pointer (base ctor, vtable, SetPItem) that push_back copies into the
// malloc'd node (the placement-new null test, GetPItem on the source) and links before the header; the driver is the thing's
// own driver component when it carries interface 0x41 (the bitset test folds to `test [thing+28h],2`), else the thing itself; the
// tentacle walk re-reads the header every pass and skips empty or dead pointers (two GetPItem calls per live tentacle).
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);    // 0x00BFEA0E
inline void* __cdecl operator new(size_t, void* place) { return place; }
extern "C" int  __cdecl puts(const char* text);    // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
struct CPresenceBits {                             // std::bitset<274>
    unsigned long words[9];
    bool test(unsigned long position) const
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (words[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
struct CThing;
struct CExplosionDriver {                           // the explosion thing's driver component (interface 0x41)
    void AddExcludedThing(CThing* thing);          // 0x0080CBF0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits UsingComponents;                  // +0x20
    unsigned char _pad_0x44[0x5c - 0x44];
    CExplosionDriver* PTCDriver;                    // +0x5c
    unsigned char _pad_0x60[0x91 - 0x60];
    unsigned char Dead : 1;                         // +0x91 bit 0
    CExplosionDriver* GetExplosionDriver()          // the driver when the thing carries interface 0x41, else the thing stands in for it
    {
        if ((UsingComponents.words[2] & 2) != 0)
            return PTCDriver;
        return (CExplosionDriver*)this;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CThing* item);                    // 0x00A01B90
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct SThingPointerNode {                          // STLport _List_node<CIntelligentPointer<CThing>> (16 bytes)
    SThingPointerNode*  _M_next;
    SThingPointerNode*  _M_prev;
    CIntelligentPointer _M_data;
};
struct CThingPointerList {                          // std::list<CIntelligentPointer<CThing>>: the header node
    SThingPointerNode* _M_node;
    SThingPointerNode* begin() const { return _M_node->_M_next; }
    SThingPointerNode* end() const { return _M_node; }
    SThingPointerNode* insert(SThingPointerNode* position, const CIntelligentPointer& value)
    {
        SThingPointerNode* node = (SThingPointerNode*)malloc(sizeof(SThingPointerNode));
        new (&node->_M_data) CIntelligentPointer(value);
        SThingPointerNode* prev = position->_M_prev;
        node->_M_next = position;
        node->_M_prev = prev;
        prev->_M_next = node;
        position->_M_prev = node;
        return node;
    }
    void push_back(const CIntelligentPointer& value) { insert(end(), value); }
};

struct CTCKraken {
    unsigned char     _pad00[0x04];
    CThing*           ParentThing;                  // +0x04
    unsigned char     _pad08[0x30 - 0x08];
    CThingPointerList ExplosionList;                // +0x30
    unsigned char     _pad34[0x44 - 0x34];
    CThingPointerList Tentacles;                    // +0x44
    void AddStrikeExplosion(CThing* explosion);
};
void CTCKraken::AddStrikeExplosion(CThing* explosion)
{
    ExplosionList.push_back(CIntelligentPointer(explosion));
    CExplosionDriver* driver = explosion->GetExplosionDriver();
    driver->AddExcludedThing(ParentThing);
    for (SThingPointerNode* it = Tentacles.begin(); it != Tentacles.end(); it = it->_M_next) {
        CIntelligentPointer& tentacle = it->_M_data;
        if (tentacle.GetPItem() && !tentacle.GetPItem()->Dead)
            driver->AddExcludedThing(tentacle.GetPItem());
    }
}
