// Grade: DIFFER(277v429) -- the two size terms round through the inline-asm `fld ; fistp` helper (store/reload, bare dword fistp next to __ftol2 calls): the documented asm_bake lane gap (GOTCHAS chunks 150/164); one cast-form attempt, structure only
// NParticleEngine::CPSCSingleSprite::GetComponentCost(SCost& cost, CCostTable& table, long a, long b) -- add the sprite's fill cost (quantised width x height) and the trail cost to the running totals
// Leaf: GetComponentCost
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B05C60, 0x1AD bytes (padded to 432), `ret 10h`.
// Callees: the base cost 0x00B202C0 (when the +0x96 flag is set), __ftol2 0x00BFEA70, the table lookup 0x00AF8F80.
struct SCost { float Pixels; float Sprites; float Unused; float Total; };
struct CCostTable { void Lookup(float* inOut); };   // 0x00AF8F80
extern const float g_spriteScale;                  // 0x0129F65C
extern const float g_spriteScale2;                 // 0x0122F160
extern const float g_quantStep;                    // 0x01237F0C
extern const float g_noTrailCost;                  // 0x0122DCDC
inline float Quantise(float size)
{
    float v = (float)(unsigned long)(long)size * g_spriteScale * g_spriteScale2;
    float step = 1.0f;
    for (int i = 2; i != 0; --i)
        step *= g_quantStep;
    return (float)(long)(v * step) / step;
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
