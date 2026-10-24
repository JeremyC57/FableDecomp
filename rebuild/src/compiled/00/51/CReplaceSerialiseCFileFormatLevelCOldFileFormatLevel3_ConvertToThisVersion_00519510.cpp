// CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ConvertToThisVersion(void* data) -- upgrade a level-file header record in place: anything older than version 4 is converted up through the chain and the version-4 result copied over the buffer
// Leaf: ?ConvertToThisVersion@?$CReplaceSerialise@VCFileFormatLevel@@VCOldFileFormatLevel3@@
//
// Retail 0x00519510, 80 bytes, static __fastcall (data in ecx), plain `ret`.  FableWin.pdb
//   ?ConvertToThisVersion@?$CReplaceSerialise@VCFileFormatLevel@@VCOldFileFormatLevel3@@@@SIXPAX@Z.
// Callees: 0x00517EF0 CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ConvertPreviousUnknownVersionUp(void*)
//   (static __fastcall returning CCountedPointer<CFileFormatLevel> through the hidden pointer in ecx, data in edx), the
//   count block's deleter (its slot +4, object in ecx), 0x00BFE9BC operator delete.
// Caller: CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ReadFromFile 0x0051A240 (landed).
// Layout: Ego_r.pdb CFileFormatLevel (pack 1, 25 bytes): ByteLength +0, SerialiseVersionNumber +4, MapDataOffset +5,
//   ThingDataOffset +9, BrushDataOffset +0xD, NavigationDataOffset +0x11, NavigationSectionsOffset +0x15.  CCountedPointer
//   = { T* object; count block* } with the block { refs, deleter, object } (landed 0x0051C2F0 model).
// Shape: `if (version byte != GetClassVersionNumber()) *(T*)data = *ConvertPreviousUnknownVersionUp(data);` -- the
//   returned temporary is dereferenced through operator* straight off eax (`mov eax,[eax] ; mov esi,eax`; a named local
//   reads `mov esi,[esp+8]` instead); the 25-byte implicit operator= is `rep movsd` x6 + `movsb`; the temporary's
//   destructor at the end of the full expression decrements the count, re-reads the block, and on zero runs the
//   deleter and frees it.
// Label: catalog "CEngineWorldMap_CopyFields_Type3" (propagated).
void __cdecl operator delete(void* block);

struct CCPPointerInfo {
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

#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                  // +0x00
    unsigned char SerialiseVersionNumber;      // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
    static void __fastcall ConvertToThisVersion(void* data);
};
class COldFileFormatLevel2;
class COldFileFormatLevel3 : public CReplaceSerialise<COldFileFormatLevel3, COldFileFormatLevel2> {
public:
    enum { VERSION = 3 };
    long MapDataOffset;                        // +0x05
    long ThingDataOffset;                      // +0x09
    long BrushDataOffset;                      // +0x0D
    long NavigationDataOffset;                 // +0x11
};
class CFileFormatLevel : public CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3> {
public:
    enum { VERSION = 4 };
    long MapDataOffset;                        // +0x05
    long ThingDataOffset;                      // +0x09
    long BrushDataOffset;                      // +0x0D
    long NavigationDataOffset;                 // +0x11
    long NavigationSectionsOffset;             // +0x15
};
#pragma pack(pop)

template <class T, class TOld> void __fastcall CReplaceSerialise<T, TOld>::ConvertToThisVersion(void* data)
{
    if (((CReplaceSerialise<T, TOld>*)data)->SerialiseVersionNumber != GetClassVersionNumber()) {
        *(T*)data = *ConvertPreviousUnknownVersionUp(data);
    }
}

template void __fastcall CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>::ConvertToThisVersion(void* data);