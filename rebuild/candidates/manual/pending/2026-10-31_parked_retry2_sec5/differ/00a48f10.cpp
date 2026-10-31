// CLipSyncSampleData::GetFrameInterpolate(CArray<CLipSyncPhoneme>&, float) -- blend the phoneme weights of the two frames around a time
// Leaf: GetFrameInterpolate
// Grade: DIFFER(815v811) -- retail_asm candidate: the floor is now the landed __forceinline FloorToLong (inline asm fld
//   / fsub 0.5 / bare fistp / fild / fadd 1.0 / compare, as CThingPhysical::ModifyHealth 0x006A68F0), with the Retail-
//   asm evidence line; the instruction stream lines up, residue is the frame: one extra 4-byte slot (sub esp,30h v 2Ch)
//   and the helper's value/half/one/next words in a different order (local declaration order is inert), which shifts the
//   later frame offsets and the esi/edi choice
// Retail-asm: 0x00A48F33..0x00A48F4B `fld [ebp-4] ; fsub [ebp-8] ; fistp dword ptr [ebp+0Ch] ; fild [ebp+0Ch] ; fadd [ebp-0Ch] ; fstp [ebp-10h] ; mov eax,[ebp-4] ; cmp eax,[ebp-10h] ; jne` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f frame constants, a bare dword fistp; same block as GFFloatToLongFloor 0x0048D940 and the landed FloorToLong of CThingPhysical::ModifyHealth 0x006A68F0); VC7.1 emits no bare dword fistp from C++.
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
// The engine's inline-assembly FPU floor helper (standalone copy GFFloatToLongFloor 0x0048D940; the same __forceinline
// FloorToLong as the landed CThingPhysical::ModifyHealth 0x006A68F0 / CStatBar::DrawStatBarOnLeft 0x00646010).
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
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
    long frame0 = FloorToLong(frame);
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
