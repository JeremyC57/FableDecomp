// NThingShot::CTCShotDefinitionDrainLife::CTCShotDefinitionDrainLife(CThing* parent) -- CTCShotDefinitionBase(parent), AmountToHeal = 0.0f, own vtable, null soul-effect pointer
// Leaf: CTCShotDefinitionDrainLife
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB15E, length 45 bytes.
// Callees: NThingShot::CTCShotDefinitionBase::CTCShotDefinitionBase(CThing*) @ 0x007BF460 (stores vtable
// 0x0126E1A4 = NThingShot::CTCShotDefinitionBase); CIntelligentPointer<CThing>::CIntelligentPointer(CThing*)
// @ 0x004A96F0 (out of line, called with 0).
// Vtable 0x012403F4 = NThingShot::CTCShotDefinitionDrainLife (rtti-vtables.tsv): the chunk label
// "CTCDiceGame" is propagated. PDB CTCShotDefinitionDrainLife (0xb8 bytes): MaxCosAngleToTurn +0xa0,
// MaxAngleToTurn +0xa4, AmountToHeal +0xa8, Speed +0xac, PSoulEffect +0xb0 -- the retail base is 0x9c
// bytes (its HitThingsList vector is 12 bytes, not the PDB's 16), so every member sits 4 lower:
// AmountToHeal +0xa4 (the `fstp`), PSoulEffect +0xac (the `lea ecx`).
// Shape: /Os pass-through base argument; the float initialiser (`fldz ; fstp`) is stored before the
// vtable store with the pointer constructor's `push 0` scheduled between the fld and the fstp.
#pragma optimize("s", on)
struct CThing;

namespace NThingShot {

struct CTCShotDefinitionBase_Calls {
    virtual void v00();
    unsigned char _body[0x9c - 4];                          // CTCBase + the shot fields (retail 0x9c bytes)
    CTCShotDefinitionBase_Calls(CThing* parent);            // 0x007BF460
};

struct CIntelligentPointer_CThing {                         // vptr + PData
    void* __vftable;
    void* PData;
    CIntelligentPointer_CThing(CThing* thing);              // 0x004A96F0
};

struct CTCShotDefinitionDrainLife : CTCShotDefinitionBase_Calls {
    float                      MaxCosAngleToTurn;           // +0x9c
    float                      MaxAngleToTurn;              // +0xa0
    float                      AmountToHeal;                // +0xa4
    float                      Speed;                       // +0xa8
    CIntelligentPointer_CThing PSoulEffect;                 // +0xac
    virtual void v00();                                     // own vtable 0x012403F4
    CTCShotDefinitionDrainLife(CThing* parent);
};

CTCShotDefinitionDrainLife::CTCShotDefinitionDrainLife(CThing* parent)
    : CTCShotDefinitionBase_Calls(parent)
    , AmountToHeal(0.0f)
    , PSoulEffect(0)
{
}

}
