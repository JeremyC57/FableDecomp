// CVtable_012bf1dc::CVtable_012bf1dc(void* source, const SStreamSegment& segment) -- constructor of libpk's null audio stream (the fallback the stream factory builds when a sound cannot be decoded): only its vtable; both arguments, the same as COggVorbisStream's, are unused
// Leaf: ??0CVtable_012bf1dc@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C888F0, 11 bytes, thiscall, ret 8. No callees.
// Identity: it stores vtable 0x012BF1DC, whose slots are the deleting destructor 0x00C88900 (-> the IAudioStream base
//   destructor 0x00C88890) and the IAudioStream slots as constants: GetLength 0x00C888A0 = 0, Seek 0x00C888B0 = -1
//   (two arguments), Read 0x00C888C0 = -1 (two arguments), GetRate 0x00C888D0 = -1, GetState 0x00C888E0 = -1; its only
//   caller 0x00C73B97 allocates 4 bytes (operator new 0x00BFEA1A) and passes the two arguments it gives the Ogg stream.
//   The catalog label "CBase::CBase" is propagated; libpk has no PDB/RTTI, so the class is named by its vtable.
// Shape: the inline IAudioStream constructor's vtable store is dead and dropped; only the derived store remains.
struct SStreamSegment {
    unsigned long m_word0;
    unsigned long m_word4;
    unsigned long m_word8;
};

class IAudioStream {                                    // vtable 0x012BEFFC
public:
    virtual ~IAudioStream() {}
    virtual long GetLength() = 0;
    virtual long Seek(unsigned long position, bool exact) = 0;
    virtual long Read(void* buffer, unsigned long size) = 0;
    virtual long GetRate() = 0;
    virtual long GetState() = 0;
};

class CVtable_012bf1dc : public IAudioStream {          // vtable 0x012BF1DC: the null stream, 4 bytes
public:
    CVtable_012bf1dc(void* source, const SStreamSegment& segment);
    virtual long GetLength();                           // 0x00C888A0
    virtual long Seek(unsigned long position, bool exact);   // 0x00C888B0
    virtual long Read(void* buffer, unsigned long size);     // 0x00C888C0
    virtual long GetRate();                             // 0x00C888D0
    virtual long GetState();                            // 0x00C888E0
};

CVtable_012bf1dc::CVtable_012bf1dc(void* source, const SStreamSegment& segment)
{
}