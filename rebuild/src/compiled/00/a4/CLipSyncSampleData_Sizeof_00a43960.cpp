// CLipSyncSampleData::Sizeof() const -- serialised size of the sample: the frame array (count word block + per-frame size) plus one long
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A43960, length 107 bytes.
// Leaf: ?Sizeof@CLipSyncSampleData@@QBEKXZ
// Callees: none (every Sizeof is inline; the array template's loop needs `__forceinline`).
// Shape: an array's size is `count * 4 + 12` plus the sum of its elements' sizes; a frame (CLipSyncFrameData, a
// vector of 12-byte phonemes) has the same form with a constant 12 per phoneme, so the inner loop only steps
// `add edx,0Ch`. The counts are the signed `/ 12` (0x2AAAAAAB multiply). Total + 4 for the trailing long.
class CLipSyncPhoneme { public:                           // 12 bytes
    void*         Name;
    unsigned long ID;
    long          Value;
    unsigned long Sizeof() const { return 12; }
};
template <class T> class CArray { public:
    T* m_first;
    T* m_last;
    T* m_end;
    unsigned long Size() const { return m_last - m_first; }
    __forceinline unsigned long Sizeof() const
    {
        unsigned long size = Size() * 4 + 12;
        for (const T* it = m_first; it != m_last; ++it)
            size += it->Sizeof();
        return size;
    }
};
class CLipSyncFrameData { public:                         // 12 bytes
    CArray<CLipSyncPhoneme> Phonemes;
    unsigned long Sizeof() const { return Phonemes.Sizeof(); }
};
class CLipSyncSampleData { public:
    CArray<CLipSyncFrameData> Frames;              // +0x00
    long                      Length;              // +0x0c
    unsigned long Sizeof() const;
};

unsigned long CLipSyncSampleData::Sizeof() const
{
    return Frames.Sizeof() + 4;
}