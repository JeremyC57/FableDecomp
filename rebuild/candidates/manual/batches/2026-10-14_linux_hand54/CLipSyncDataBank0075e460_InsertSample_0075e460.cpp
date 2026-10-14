// CLipSyncDataBank_0075e460::InsertSample(const SSampleSource* source) -- when the insertion iterator (+0x74) has not reached the end of the sample vector (+0x68..: last at +0x6c), a 0x38-byte sample is built on the stack from the source (0x0075F180 with (source, &source->+0x18, the dwords +0x1c and +0x24 of the sample at the iterator, 1.0f, 2.0f, 0)), inserted before the iterator (0x0075FAE0, vector insert returning the new iterator), its CCharString (+0x30) destroyed (the temporary's inlined destructor); then the bank is refreshed (0x0075DDF0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075E460, length 96 bytes.
// Callees: 0x0075F180 (sample constructor, seven arguments), 0x0075FAE0 (`insert(where, value)` on the vector at +0x68), CCharString::~CCharString @ 0x0099EAE0, 0x0075DDF0 (thiscall on `this`). The chunk labels are propagated. Shape: a 0x38-byte frame holding the sample temporary, `this` in esi, `ret 4`.
struct C3DVector { float x, y, z; };
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct SSampleSource_0075e460 {
    C3DVector     m_position;                      // +0x00
    unsigned char _pad0c[0xc];
    C3DVector     m_direction;                     // +0x18
};
struct SLipSyncSample_0075e460 {                   // 0x38 bytes: the CCharString at +0x30 is the only member with a destructor
    unsigned char m_body[0x1c];
    long          m_1c;                            // +0x1c
    unsigned char m_pad20[4];
    long          m_24;                            // +0x24
    unsigned char m_pad28[8];
    CCharString   m_name;                          // +0x30
    long          m_34;                            // +0x34
    SLipSyncSample_0075e460(const SSampleSource_0075e460* source, const C3DVector* direction, long a, long b, float scale, float range, long flags);   // 0x0075F180
};
struct CRetailVector_0075e460 {                    // VC7.1 Dinkumware vector: first / last / end
    SLipSyncSample_0075e460* m_first;
    SLipSyncSample_0075e460* m_last;
    SLipSyncSample_0075e460* m_end;
    SLipSyncSample_0075e460* insert(SLipSyncSample_0075e460* where, const SLipSyncSample_0075e460& value);   // 0x0075FAE0
};

struct CLipSyncDataBank_0075e460 {
    unsigned char            _pad00[0x68];
    CRetailVector_0075e460   m_samples;            // +0x68
    SLipSyncSample_0075e460* m_pInsertAt;          // +0x74
    void Refresh();                                // 0x0075DDF0
    void InsertSample(const SSampleSource_0075e460* source);
};

void CLipSyncDataBank_0075e460::InsertSample(const SSampleSource_0075e460* source)
{
    if (m_pInsertAt != m_samples.m_last) {
        m_pInsertAt = m_samples.insert(m_pInsertAt, SLipSyncSample_0075e460(source, &source->m_direction, m_pInsertAt->m_1c, m_pInsertAt->m_24, 1.0f, 2.0f, 0));
    }
    Refresh();
}
