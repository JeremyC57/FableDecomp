// CReplaceSerialise<CFileFormatBrush,COldFileFormatBrush>::ConvertToThisVersion(void* data) -- upgrade a scripted-brush record in place: an older record is converted up through the chain and the current-version result copied over the buffer
// Leaf: ?ConvertToThisVersion@?$CReplaceSerialise@VCFileFormatBrush@@VCOldFileFormatBrush@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00825D40, 80 bytes, static __fastcall (data in ecx), plain `ret`. FableWin.pdb
//   ?ConvertToThisVersion@?$CReplaceSerialise@VCFileFormatBrush@@VCOldFileFormatBrush@@@@SIXPAX@Z (name set).
// Callees: CReplaceSerialise<CFileFormatBrush,COldFileFormatBrush>::ConvertPreviousUnknownVersionUp @ 0x00825590 (static
//   __fastcall returning CCountedPointer<CFileFormatBrush> through the hidden pointer in ecx, data in edx), the count
//   block's deleter (its slot +4, object in ecx), operator delete @ 0x00BFE9BC.
// Label: the catalog's "CAFile_ReadRigidBody" is descriptive and wrong.
// Layout: Ego_r.pdb CFileFormatBrush (pack 1, 73 bytes, version 2: the `cmp byte ptr [edi+4],2`).
// Shape: the landed 0x00519510 template: `if (version byte != GetClassVersionNumber()) *(T*)data =
//   *ConvertPreviousUnknownVersionUp(data);` -- the 73-byte implicit operator= is `rep movsd` x18 + `movsb`, the
//   temporary's destructor decrements the count, re-reads the block and on zero runs the deleter and frees it.
void __cdecl operator delete(void* block);             // 0x00BFE9BC

class CCPPointerInfo { public:
    unsigned long refs;                        // +0x0
    void (__fastcall* destroy)(void*);         // +0x4
    void* object;                              // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              object;                    // +0x0
    CCPPointerInfo* control;                   // +0x4
    ~CCountedPointer()
    {
        if (control) {
            --control->refs;
            if (control->refs == 0) {
                control->destroy(control->object);
                operator delete(control);
            }
        }
    }
    T& operator*() const { return *object; }
};

class C2DBoxI { public: long m_x0, m_y0, m_x1, m_y1; };

#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                  // +0x00
    unsigned char SerialiseVersionNumber;      // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
    static void __fastcall ConvertToThisVersion(void* data);
};
class COldFileFormatBrush;
class CFileFormatBrush : public CReplaceSerialise<CFileFormatBrush, COldFileFormatBrush> {
public:
    enum { VERSION = 2 };
    C2DBoxI GameArea;                          // +0x05
    char    ScriptName[32];                    // +0x15
    float   CreationTimeSeconds;               // +0x35
    long    CreationPattern;                   // +0x39
    long    NumberOfThingsToMove;              // +0x3D
    long    NumberOfThingsToDelete;            // +0x41
    long    NumberOfThingsToAdd;               // +0x45 (73 bytes)
};
#pragma pack(pop)

template <class T, class TOld> void __fastcall CReplaceSerialise<T, TOld>::ConvertToThisVersion(void* data)
{
    if (((CReplaceSerialise<T, TOld>*)data)->SerialiseVersionNumber != GetClassVersionNumber()) {
        *(T*)data = *ConvertPreviousUnknownVersionUp(data);
    }
}

template void __fastcall CReplaceSerialise<CFileFormatBrush, COldFileFormatBrush>::ConvertToThisVersion(void* data);