// Grade: DIFFER(873v875) -- known lane gap: the weight byte uses an inline-asm fistp (fld/fmul 255/fstp/fld/fistp); also the lane's <map> is not retail's STLport map (header malloc 0x18 at 0x00BFEA0E, inline _M_erase 0x00630D90 + free 0x00BFEA14)
// CLipSyncSampleData::WriteToFile(CDataOutputStream&) const -- write the phoneme symbol table, frame rate and per-frame phoneme weights
// Leaf: WriteToFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A48BA0, 872 bytes (+ int3 padding), `ret 4`.
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
long GFRoundToLong(float value);                 // stand-in for retail's inline fistp (lane gap)
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
