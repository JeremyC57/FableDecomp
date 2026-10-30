// CReverbDiffuser::UpdateGain(unsigned int index, const SDiffuserParams* params, const CRingBuffer* history, unsigned int count) -- libpk reverb diffuser: run gain `index` over one block of the shared delay line, reading at the block start minus the gain's tap delay and writing at the block start minus the gain's aligned line offset ((index + 1) * spacing / 4 + base, rounded up to 4 floats); a block that would cross the ring's end is processed in chunks that stop at whichever position wraps first
// Leaf: ?UpdateGain@CReverbDiffuser@@QAEXIPBUSDiffuserParams@@PBUCRingBuffer@@I@Z
// Grades: 1 (RELOCATION_MATCH first grade; one compile-only sweep fixed the chunk-length initialisation order)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C756C0, 341 bytes, thiscall `ret 10h`. Callers: CReverbDiffuser::Process 0x00C75A42
//   and CReverbDiffuser::ProcessRamped 0x00C75AFE (both landed), once per gain (index 0..2).
// Names: DESCRIPTIVE (libpk: no PDB, no RTTI). The class, the method and the SDiffuserParams / CDspGain / CReverbConfig
//   models are the landed CReverbDiffuser rows' (Process 0x00C75A20 binds this address as UpdateGain). The third
//   parameter is the shared delay line, not a parameter block: it is read as {data +0, size +4 (floats), write position
//   +8} and every position wraps against its size, which is the landed CRingBuffer (CDspOps::ReadRing 0x00C763E0,
//   CReverbTapBank::Process 0x00C75E90 `const CRingBuffer* history`). The landed callers' spelling
//   `?UpdateGain@CReverbDiffuser@@QAEXIPBUSDiffuserParams@@0I@Z` types it as SDiffuserParams (NAME_FIXES.tsv).
// Callee: CDspGain::Apply 0x00C74CE0 (thiscall on m_gains[index]: &params->m_gains[index], source, block, mode 1,
//   count; the landed definition's spelling).
// Layout: SDiffuserParams = three tap delays (+0), three 0x1C-byte SGainParams (+0xC), the two 0x3C-byte stage records
//   (+0x60); CReverbConfig +0x10 / +0x14 are the line-offset base and spacing (descriptive names).
// Shape: both chunk lengths start at `remaining` before either wrap test (one-at-a-time initialisation moves the second
//   `mov esi,ebp` below the first test); the shorter of the two is the chunk.
struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
};

struct CDspOps {                           // x87 vtable 0x012BF02C (SIMD build: ctor 0x00C88A30)
    virtual long GetKind();                                                                         // 0x00C73FB0
    virtual void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count);   // 0x00C74130
    virtual void ScaleRamp(float* buffer, float gain, float step, unsigned int count);             // 0x00C740B0
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned int count);          // 0x00C74030
    virtual void Scale(float* buffer, float gain, unsigned int count);                             // 0x00C73FC0
    virtual void Add(float* dst, const float* src, unsigned int count);                            // 0x00C741C0
    virtual void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count); // 0x00C742C0
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count);                // 0x00C74230
    void WriteRing(CRingBuffer* ring, unsigned int pos, const float* src, unsigned int count);     // 0x00C76460
    void ReadRingScaledRamp(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step,
        unsigned int count);                                                                        // 0x00C76560
    void MixRampFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step,
        unsigned int count);                                                                        // 0x00C767E0
};

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
    static CDspOps* s_pOps;                // 0x0143CCDC
};

struct SGainParams {
    float m_master;                        // +0x00
    float m_current[3];                    // +0x04
    float m_target[3];                     // +0x10
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Reset();                          // 0x00C744E0
    void Apply(const SGainParams* params, const float* source, float* block, int mode, unsigned int count);   // 0x00C74CE0
    void Crossfade(SGainParams* oldParams, const SGainParams* newParams, const float* source, float* block, int mode,
        unsigned int count, unsigned int remaining);                                                         // 0x00C74D30
};

struct SDiffuserStageParams {
    unsigned char _body[0x3c];
};

struct SDiffuserParams {
    unsigned int m_taps[3];                // +0x00 per-gain read delay (samples)
    SGainParams m_gains[3];                // +0x0C
    SDiffuserStageParams m_stages[2];      // +0x60
};

struct CReverbDiffuserStage : CDspObject { // vtable 0x012BF07C
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08
    unsigned int m_pos;                    // +0x0C
    CDspGain m_gain;                       // +0x10
    unsigned int m_index;                  // +0x24
};

struct CReverbConfig {
    unsigned int m_00;
    unsigned int m_channels;               // +0x04
    unsigned int m_rate;                   // +0x08
    unsigned int m_0C;
    unsigned int m_tapBase;                // +0x10
    unsigned int m_tapSpacing;             // +0x14
};

struct CReverbDiffuser : CDspObject {      // vtable 0x012BF080
    CDspGain m_gains[3];                   // +0x04
    CReverbDiffuserStage m_stages[2];      // +0x40
    CReverbConfig* m_pConfig;              // +0x90
    unsigned int m_firstIndex;             // +0x94
    unsigned int m_endIndex;               // +0x98
    void UpdateGain(unsigned int index, const SDiffuserParams* params, const CRingBuffer* history, unsigned int count);
    void UpdateGainRamped(unsigned int index, SDiffuserParams* params, const SDiffuserParams* next, CRingBuffer* history,
        unsigned int count, unsigned int rampLength);
};
void CReverbDiffuser::UpdateGain(unsigned int index, const SDiffuserParams* params, const CRingBuffer* history, unsigned int count)
{
    float* data = history->m_data;
    unsigned int start = history->m_pos >= count ? history->m_pos - count : history->m_size - count + history->m_pos;
    unsigned int tap = params->m_taps[index];
    unsigned int source = start >= tap ? start - tap : history->m_size - tap + start;
    unsigned int offset = ((index + 1) * m_pConfig->m_tapSpacing / 4 + m_pConfig->m_tapBase + 3) & ~3u;
    unsigned int block = start >= offset ? start - offset : history->m_size - offset + start;
    if (block + count < history->m_size && source + count < history->m_size) {
        m_gains[index].Apply(&params->m_gains[index], data + source, data + block, 1, count);
        return;
    }
    unsigned int remaining = count;
    do {
        unsigned int n1 = remaining;
        unsigned int n = remaining;
        if (source + remaining > history->m_size) n1 = history->m_size - source;
        if (block + remaining > history->m_size) n = history->m_size - block;
        if (n >= n1) n = n1;
        m_gains[index].Apply(&params->m_gains[index], data + source, data + block, 1, n);
        source = source + n < history->m_size ? source + n : source + (n - history->m_size);
        block = block + n < history->m_size ? block + n : block + (n - history->m_size);
        remaining -= n;
    } while (remaining);
}