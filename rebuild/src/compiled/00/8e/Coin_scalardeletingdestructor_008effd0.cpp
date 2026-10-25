// Coin::`scalar deleting destructor'(unsigned int) -- tavern coin-game coin: release the Thing intelligent pointer, the inlined Mass destructor stores the Mass vtable, flag-tested delete
// Leaf: ??_GCoin@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EFFD0, 39 bytes (+9 int3 padding), thiscall, `ret 4`.
// Identity: slot 0 of the Coin vtable 0x01288238 (rtti-vtables.tsv); the stored 0x01288230 is the Mass vtable (Mass's own
//   deleting destructor 0x008EE910 stores it too). The Coin copy constructor 0x008EFF40 (Mass words +0x04..+0x20, Coin
//   vtable, Owned +0x24, Thing +0x25, Radius/Owner/Sound +0x30..+0x38) and the vector<Coin> allocator 0x008EFF20
//   (`imul eax,eax,3Ch`) confirm the Ego_r.pdb layout: Coin 0x3c bytes, Mass 0x24. The catalog label
//   CLandscapeBackgroundSubNodeSplitBranch::`scalar_deleting_destructor' is propagated (engine header not applicable).
// Callees: ~CBaseIntelligentPointer 0x00A01C10, operator delete 0x00BFE9BC.
// Layout: CIntelligentPointer<CThing> sits at the odd offset +0x25 with Radius at +0x30, so the intelligent-pointer classes
//   are declared under pack(1) (alignment 1, 8 bytes) while Coin and Mass keep the default packing.
// Shape: Coin declares no destructor, so its compiler-generated one drops Coin's own vtable reset and the
//   CIntelligentPointer<CThing> vtable reset (24m/wave-25 rule); the member's base destructor runs out of line, then
//   Mass's inline `virtual ~Mass() {}` stores the Mass vtable. A `new Coin` factory pulls the vtable (and ??_G) into the TU.
class CThing;

#pragma pack(push, 1)
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
    void* PItem;
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
};
#pragma pack(pop)

struct C2DVector { float X, Y; };

class Mass {
public:
    virtual ~Mass() {}
    C2DVector Pos;                                  // +0x04
    C2DVector Vel;                                  // +0x0c
    float IMass;                                    // +0x14
    float APos;                                     // +0x18
    float AVel;                                     // +0x1c
    float AIMass;                                   // +0x20
};

class Coin : public Mass {
public:
    bool Owned;                                     // +0x24
    CIntelligentPointer<CThing> Thing;              // +0x25
    float Radius;                                   // +0x30
    int Owner;                                      // +0x34
    unsigned long Sound;                            // +0x38
};

Coin* CreateCoin()
{
    return new Coin;
}