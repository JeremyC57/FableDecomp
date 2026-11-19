// CLipSyncSampleData::WriteToFile(CDataOutputStream&) const -- write the phoneme symbol table, frame rate and per-frame phoneme weights
// Leaf: WriteToFile
// Retail-asm: 0x00A48E25 `fld dword ptr [esp + 0x20]` ; 0x00A48E29 `fistp dword ptr [esp + 0x10]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the value is stored to the helper's by-value parameter slot and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything around it is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A48BA0; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. The weight byte is rounded by the helper; the frame rate stays a __ftol2 cast.
#include <map>
struct CCharString {
    void* PStringData;
    CCharString();                               // 0x0099E4B0
    CCharString(const CCharString& other);       // 0x0099EC30
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    ~CCharString();                              // 0x0099EAE0
};
struct CDataOutputStream {
    void WriteUChar(unsigned char value);        // 0x00993E50
    void WriteULong(unsigned long value);        // 0x00993EA0
    void WriteString(const CCharString& value);  // 0x00995AE0
};
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long GFRoundToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

struct CLipSyncPhoneme {                         // 12 bytes
    CCharString   Symbol;                        // +0
    unsigned long ID;                            // +4
    float         Weight;                        // +8
};
struct CPhonemeArray {
    CLipSyncPhoneme* _M_start;
    CLipSyncPhoneme* _M_finish;
    CLipSyncPhoneme* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
};
struct CFrameArray {
    CPhonemeArray* _M_start;
    CPhonemeArray* _M_finish;
    CPhonemeArray* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
};
struct CLipSyncSampleData {
    CFrameArray FrameData;                       // +0x00
    float       FramesPerSecond;                 // +0x0c
    void WriteToFile(CDataOutputStream& out) const;
};

void CLipSyncSampleData::WriteToFile(CDataOutputStream& out) const
{
    std::map<unsigned long, CCharString> symbols;
    for (unsigned int i = 0; i < FrameData.size(); ++i) {
        for (unsigned int j = 0; j < FrameData._M_start[i].size(); ++j) {
            CLipSyncPhoneme phoneme = FrameData._M_start[i]._M_start[j];
            CCharString symbol(phoneme.Symbol);
            symbols[phoneme.ID] = symbol;
        }
    }
    out.WriteULong(symbols.size());
    for (std::map<unsigned long, CCharString>::const_iterator it = symbols.begin(); it != symbols.end(); ++it) {
        out.WriteUChar((unsigned char)it->first);
        out.WriteString(it->second);
    }
    out.WriteULong((long)FramesPerSecond);
    out.WriteULong(FrameData.size());
    for (unsigned int i = 0; i < FrameData.size(); ++i) {
        unsigned int count = FrameData._M_start[i].size();
        out.WriteUChar((unsigned char)count);
        for (unsigned int j = 0; j < count; ++j) {
            CLipSyncPhoneme phoneme = FrameData._M_start[i]._M_start[j];
            out.WriteUChar((unsigned char)phoneme.ID);
            long weight = GFRoundToLong(phoneme.Weight * 255.0f);
            if (weight < 0)
                weight = 0;
            else if (weight > 0xff)
                weight = 0xff;
            out.WriteUChar((unsigned char)weight);
        }
    }
}
