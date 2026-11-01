// MixChannels_00c77d90(const float* position, float* gains, unsigned long count, unsigned long channels) -- libpk panner: from a source position's x and z fill a gain block (stereo: a linear left/right pan of x; otherwise four quad speaker gains at (+-0.707, +-0.707) weighted by inverse squared distance, a speaker exactly hit taking everything), repeated over `count` samples in strides of min(channels, count)
// Leaf: ?MixChannels_00c77d90@@YAXPBMPAMKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C77D90, 557 bytes, `__cdecl`, plain `ret`. Audio library (libpk) code: no PDB, no RTTI;
//   the name is the landed callers' binding (CChannelMixer_00c783b0::Mix 0x00C783B0 calls it with (set->m_channels, m_quad, 4,
//   limit) and (.., m_pair, 2, limit); CEffectPart260::Configure 0x00C783F0 with (parameters->Mix, Values, 4, Context->Channels)),
//   kept under the one-address-one-name rule (`rebuild/oracles/reloc_bindings.tsv` ?MixChannels_00c77d90@@YAXPAM0JJ@Z; the
//   unsigned compares `jae`/`jb`/`jbe` of retail make the counts unsigned here).
// Callees: none. Constants 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 0.5f 0x0122F59C, 0.707f 0x0125D044.
// Shape: `if (count < channels) channels = count;` then the stereo branch on `channels == 2`; the four squared distances are
//   named locals built from the four signed offsets (x +- 0.707, z +- 0.707), the gains are four locals zeroed up front, the
//   exact-hit chain `if (d0 == 0.0f) g0 = 1.0f; else if ...` ends in the inverse-distance normalisation; both fill loops are
//   `for (i = 0; i < count; i += stride)`.
void __cdecl MixChannels_00c77d90(const float* position, float* gains, unsigned long count, unsigned long channels)
{
    if (count < channels)
        channels = count;
    float x = position[0];
    float z = position[2];
    if (channels == 2) {
        float left = (1.0f - x) * 0.5f;
        float right = (x + 1.0f) * 0.5f;
        for (unsigned long i = 0; i < count; i += 2) {
            gains[i] = left;
            gains[i + 1] = right;
        }
        return;
    }
    float xm = 0.707f - x;
    float xp = x + 0.707f;
    float zm = 0.707f - z;
    float zp = z + 0.707f;
    float d0 = zm * zm + xp * xp;
    float d1 = xm * xm + zm * zm;
    float d2 = zp * zp + xp * xp;
    float d3 = zp * zp + xm * xm;
    float g0 = 0.0f;
    float g1 = 0.0f;
    float g2 = 0.0f;
    float g3 = 0.0f;
    if (d0 == 0.0f)
        g0 = 1.0f;
    else if (d1 == 0.0f)
        g1 = 1.0f;
    else if (d2 == 0.0f)
        g2 = 1.0f;
    else if (d3 == 0.0f)
        g3 = 1.0f;
    else {
        float w0 = 1.0f / d0;
        float w1 = 1.0f / d1;
        float w2 = 1.0f / d2;
        float w3 = 1.0f / d3;
        float norm = 1.0f / (w3 + w2 + w1 + w0);
        g0 = norm * w0;
        g1 = norm * w1;
        g2 = norm * w2;
        g3 = norm * w3;
    }
    for (unsigned long i = 0; i < count; i += channels) {
        gains[i] = g0;
        gains[i + 1] = g1;
        gains[i + 2] = g2;
        gains[i + 3] = g3;
    }
}