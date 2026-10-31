// NParticleEngine::CPSCSingleSprite::GetComponentCost(SCost& cost, CCostTable& table, long a, long b) -- add the sprite's fill cost (quantised width x height) and the trail cost to the running totals
// Leaf: GetComponentCost
// Grade: DIFFER(430v429) -- retail_asm candidate (Retail-asm line cites 0x00B05CDF / 0x00B05D39 bare fistp dword): the
//   inline Quantise is __forceinline and rounds through a __forceinline FloatToLong(float) helper with `__asm fld value
//   / __asm fistp result` (retail's store/reload + bare fistp next to __ftol2); the whole instruction stream now lines
//   up, residue is the frame: we add one `push ecx` local slot for the step where retail homes every temporary in the
//   dead argument slots
// Retail-asm: 0x00B05CDB `fld dword ptr [esp + 0x14]` ; 0x00B05CDF `fistp dword ptr [esp + 0x10]` and 0x00B05D35 `fld dword ptr [esp + 0x10]` ; 0x00B05D39 `fistp dword ptr [esp + 0x1c]` -- the engine's inline-assembly float-to-long rounding (bare dword fistp, current rounding mode) inside the inline Quantise: the product is stored to a frame word, reloaded by the asm fld and rounded into a local
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B05C60, 0x1AD bytes (padded to 432), `ret 10h`.
// Callees: the base cost 0x00B202C0 (when the +0x96 flag is set), __ftol2 0x00BFEA70, the table lookup 0x00AF8F80.
struct SCost { float Pixels; float Sprites; float Unused; float Total; };
struct CCostTable { void Lookup(float* inOut); };   // 0x00AF8F80
extern const float g_spriteScale;                  // 0x0129F65C
extern const float g_spriteScale2;                 // 0x0122F160
extern const float g_quantStep;                    // 0x01237F0C
extern const float g_noTrailCost;                  // 0x0122DCDC
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
__forceinline float Quantise(float size)
{
    float v = (float)(unsigned long)(long)size * g_spriteScale * g_spriteScale2;
    float step = 1.0f;
    for (int i = 2; i != 0; --i)
        step *= g_quantStep;
    return (float)FloatToLong(v * step) / step;
}
namespace NParticleEngine {
struct CPSCSingleSprite {
    unsigned char _pad00[0x34];
    void*         Sprite;                          // +0x34
    unsigned char _pad38[0x10];
    float         Width;                           // +0x48
    float         Height;                          // +0x4c
    unsigned char _pad50[4];
    long          TrailLength;                     // +0x54
    unsigned char _pad58[0x80 - 0x58];
    void*         Trail;                           // +0x80
    long          TrailCost;                       // +0x84
    long          TrailSegments;                   // +0x88
    unsigned char _pad8c[0x93 - 0x8c];
    unsigned char Flags93;                         // +0x93
    unsigned char _pad94[2];
    bool          HasBase;                         // +0x96
    void BaseCost(SCost& cost, CCostTable& table, long a, long b);   // 0x00B202C0
    void GetComponentCost(SCost& cost, CCostTable& table, long a, long b);
};
void CPSCSingleSprite::GetComponentCost(SCost& cost, CCostTable& table, long a, long b)
{
    if (HasBase)
        BaseCost(cost, table, a, b);
    if (Sprite) {
        float w = Quantise(Width);
        float h = Quantise(Height);
        float area = w > h ? w : h;
        table.Lookup(&area);
        float count = (Flags93 & 0x20) ? (float)(TrailLength * 2) : g_noTrailCost;
        float fill = area * area;
        cost.Pixels += count;
        cost.Sprites += fill;
        cost.Total += fill + count;
    }
    if (Trail) {
        float len = (float)(TrailSegments * 16 - 16);
        float value = *(float*)&TrailCost;
        table.Lookup(&value);
        float fill = value * len;
        cost.Pixels += len;
        cost.Sprites += fill;
        cost.Total += fill + len;
    }
}
}
