// CThingCreatureBase::GetStunTimeSeconds() -- a random stun duration in seconds, uniform in the creature def's [StunTimeMin, StunTimeMax] range
// Leaf: GetStunTimeSeconds
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662040, 59 bytes.
// Callees: CThing::GetWorld @ 0x004C7AB0 (propagated label "CViewBase::GetDisplayEngine"), CWorld::GetPRandomSeed
//          @ 0x0049D810 (propagated "CVertexBufferWin32::DoSizeof"; landed as GetPRandomSeed in 0x007BEFB0),
//          GFFloatRandom(float range, unsigned long* seed) @ 0x004978A0 (__fastcall: seed in ecx, range on the stack).
// Layout: PDef (CDefPointer<const CThingCreatureDef>, its object word) at retail +0xe0 (PDB +0xfc). PDB CThingCreatureDef
//   has StunTimeMin/Max +0x74/+0x78 and DazedTimeMin/Max +0x80/+0x84; retail reads +0x70/+0x74 and +0x7c/+0x80 (4 bytes
//   lower, like the landed GetDazedHealthThreshold 0x00662090 at +0x84 = PDB +0x88). The manifest names BOTH rows
//   "GetDazedTimeSeconds" (propagated); the +0x70 one is the stun range.
// Shape: the min/max pair is copied into an 8-byte frame record first (mov edx/eax, then [esp] and [esp+4]):
//   a by-value CFloatRange local; then the seed (right-most argument) is evaluated, then the difference
//   max - min is pushed, and the trailing + min reads the record after the call (fadd [esp]).
struct CWorld {
    unsigned long* GetPRandomSeed();        // 0x0049D810
};

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CFloatRange {
    float Min;
    float Max;
};

struct CThingCreatureDef {
    unsigned char _pad00[0x70];
    CFloatRange StunTime;                   // +0x70 StunTimeMin, StunTimeMax
    unsigned char _pad78[0x4];              // +0x78 StunHealthThreshold
    CFloatRange DazedTime;                  // +0x7c DazedTimeMin, DazedTimeMax
};

struct CThingCreatureBase {
    unsigned char _pad000[0xe0];
    const CThingCreatureDef* PDef;          // +0xe0 CDefPointer<const CThingCreatureDef>::Object
    CWorld* GetWorld();                     // 0x004C7AB0 (CThing)
    float GetStunTimeSeconds();
};

float CThingCreatureBase::GetStunTimeSeconds()
{
    CFloatRange range = PDef->StunTime;
    return GFFloatRandom(range.Max - range.Min, GetWorld()->GetPRandomSeed()) + range.Min;
}