// CEngineSceneMap::CalculateLevel(float extent) -- the highest grid level (from levels-1 down to 1) whose cell size (the map span (+0xc minus +4) over 2^level, scaled by the scene's factor at +0x50 of the object at +0x24) is at least the extent; 0 when none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B91920, length 84 bytes.
// Callees: none. Shape: the span is computed once into the frame (`fsub ; fstp [esp+4]`), each level builds `1 << level` in the other frame word for `fild`, `fdivr`
// then `fmul` by the scale and `fcomp` against the argument (`test ah,1 ; je` = not below: return); the level runs in ecx (`dec ; test ; jg`); the loop head is
// aligned with `lea ebx,[ebx]`.
struct CSceneScale_00b91920 {
    unsigned char _pad[0x50];
    float         m_scale;                         // +0x50
};

class CEngineSceneMap { public:
    unsigned char        _pad0[4];
    float                m_origin;                 // +0x04
    unsigned char        _pad8[4];
    float                m_end;                    // +0x0c
    unsigned char        _pad10[4];
    long                 m_levels;                 // +0x14
    unsigned char        _pad18[0xc];
    CSceneScale_00b91920* m_pScale;                // +0x24
    long CalculateLevel(float extent);
};

long CEngineSceneMap::CalculateLevel(float extent)
{
    for (long level = m_levels - 1; level > 0; --level) {
        if ((m_end - m_origin) / (float)(1 << level) * m_pScale->m_scale >= extent)   // the span is hoisted into the loop preheader (after the level test)
            return level;
    }
    return 0;
}