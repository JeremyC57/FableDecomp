// Grade: DIFFER(845v811) -- known lane gap: retail inlines an inline-asm fistp floor helper (fld/fsub 0.5/fistp/fild/fadd 1.0 with frame constants, ebp frame); the rest of the blend is modelled
// CLipSyncSampleData::GetFrameInterpolate(CArray<CLipSyncPhoneme>&, float) -- blend the phoneme weights of the two frames around a time
// Leaf: GetFrameInterpolate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A48F10, 811 bytes (+ int3 padding), `ret 8`, ebp frame.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);       // 0x0099EC30
    ~CCharString();                              // 0x0099EAE0
};
struct CLipSyncPhoneme {                         // 12 bytes
    CCharString   Symbol;                        // +0
    unsigned long ID;                            // +4
    float         Weight;                        // +8
};
struct CPhonemeArray {                           // CArray<CLipSyncPhoneme> (retail STLport vector, 12 bytes)
    CLipSyncPhoneme* _M_start;
    CLipSyncPhoneme* _M_finish;
    CLipSyncPhoneme* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    CLipSyncPhoneme& operator[](unsigned int i) const { return *(_M_start + i); }
    CLipSyncPhoneme* erase(CLipSyncPhoneme* first, CLipSyncPhoneme* last);   // 0x00A4A320
    void clear() { erase(_M_start, _M_finish); }
    void _M_insert_overflow(CLipSyncPhoneme* pos, const CLipSyncPhoneme& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00A4A710
    void push_back(const CLipSyncPhoneme& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CLipSyncPhoneme(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    CLipSyncPhoneme GetPhoneme(unsigned long id) const;   // 0x00A48A40 (descriptive name)
};
long GFFloor(float value);                       // stand-in for retail's inline fistp floor (lane gap)
struct CLipSyncSampleData {
    struct CFrameArray {                         // CArray<CLipSyncFrameData> (12 bytes)
        CPhonemeArray* _M_start;
        CPhonemeArray* _M_finish;
        CPhonemeArray* _M_end_of_storage;
        unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    } FrameData;                                 // +0x00
    float FramesPerSecond;                       // +0x0c
    void GetFrameInterpolate(CPhonemeArray& out, float time);
};

void CLipSyncSampleData::GetFrameInterpolate(CPhonemeArray& out, float time)
{
    float frame = time * FramesPerSecond;
    long frame0 = GFFloor(frame);
    float t = frame - (float)frame0;
    long frame1 = frame0 + 1;
    if (frame0 < 0)
        frame0 = 0;
    else if ((unsigned long)frame0 > FrameData.size() - 1)
        frame0 = FrameData.size() - 1;
    if (frame1 < 0)
        frame1 = 0;
    else if ((unsigned long)frame1 > FrameData.size() - 1)
        frame1 = FrameData.size() - 1;
    CPhonemeArray& a = FrameData._M_start[frame0];
    CPhonemeArray& b = FrameData._M_start[frame1];
    out.clear();
    for (unsigned int i = 0; i < a.size(); ++i) {
        CLipSyncPhoneme phoneme(a[i]);
        unsigned int j;
        for (j = 0; j < b.size(); ++j)
            if (b[j].ID == phoneme.ID)
                break;
        if (j == b.size()) {
            phoneme.Weight = (1.0f - t) * phoneme.Weight;
            out.push_back(phoneme);
        } else {
            CLipSyncPhoneme other = b.GetPhoneme(phoneme.ID);
            phoneme.Weight = (other.Weight - phoneme.Weight) * t + phoneme.Weight;
            out.push_back(phoneme);
        }
    }
    for (unsigned int i = 0; i < b.size(); ++i) {
        CLipSyncPhoneme phoneme(b[i]);
        unsigned int j;
        for (j = 0; j < a.size(); ++j)
            if (a[j].ID == phoneme.ID)
                break;
        if (j == a.size()) {
            phoneme.Weight = phoneme.Weight * t;
            out.push_back(phoneme);
        }
    }
}
