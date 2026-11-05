// CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ConvertPreviousUnknownVersionUp(void* data) -- build a fresh version-4 level-file header from an older record: convert the record up to version 3 first when it is not version 3, copy the four offsets across, zero NavigationSectionsOffset and hand the new header back in a counted pointer
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCFileFormatLevel@@VCOldFileFormatLevel3@@@@SI?AV?$CCountedPointer@VCFileFormatLevel@@@@PAX@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members object->Data, control->Info are the header CCountedPointer's (same offsets); the local CCPPointerInfo members refs->RefCount, destroy->DeleteFunc, object->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00517EF0, 257 bytes, static __fastcall (the hidden CCountedPointer return slot in ecx,
//   data in edx), plain `ret`; 0x14-byte frame. FableWin.pdb names the template
//   `?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCFileFormatLevel@@VCOldFileFormatLevel3@@@@SI?AV?$CCountedPointer@VCFileFormatLevel@@@@PAX@Z`.
//   Catalog "CEngineWorldMap_CopyConstructor_Type3" is wrong (propagated).
// Callees: 0x00BFEA1A operator new (0x19-byte header, 0xC-byte count block), 0x00515E90 CReplaceSerialise<COldFileFormatLevel3,COldFileFormatLevel2>::ConvertPreviousUnknownVersionUp (this chunk) (the previous link, same
//   convention), the count blocks' deleter (slot +4, object in ecx), 0x00BFE9BC operator delete; the new block's deleter
//   is CCountedPointer<CFileFormatLevel>::DeleteData @ 0x0050CDE0.
// Layout: Ego_r.pdb CFileFormatLevel (pack 1, 0x19 bytes): ByteLength +0, SerialiseVersionNumber +4, MapDataOffset +5, ThingDataOffset +9, BrushDataOffset +0xD, NavigationDataOffset +0x11, NavigationSectionsOffset +0x15.
// Shape: the landed 0x00518040 / 0x0051C2F0 template (`result(new T)`, a null `held`, `old = data`, the version byte vs
//   TOld::GetClassVersionNumber() = 3, `held = TOld::ConvertPreviousUnknownVersionUp(data); old = held.object;`,
//   `result.object->ConvertFromOld(old)` inline (four offsets copied, NavigationSectionsOffset = 0; the landed ConvertToThisVersion 0x00519510 calls this), return a copy; held and result released in reverse order).
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
void __cdecl operator delete(void* block);             // 0x00BFE9BC

#include "engine/CCountedPointer.h"

#pragma pack(push, 1)
template <class T, int VERSION> class CBaseSerialise {
public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(const void* data);
};
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                          // +0x00
    unsigned char SerialiseVersionNumber;              // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static CCountedPointer<T> __fastcall ConvertPreviousUnknownVersionUp(void* data);
};
class COldFileFormatLevel1 : public CBaseSerialise<COldFileFormatLevel1, 1> {
public:
    long MapDataOffset;                                // +0x05
    long ThingDataOffset;                              // +0x09 (13 bytes)
};
class COldFileFormatLevel2 : public CReplaceSerialise<COldFileFormatLevel2, COldFileFormatLevel1> {
public:
    enum { VERSION = 2 };
    long MapDataOffset;                                // +0x05
    long ThingDataOffset;                              // +0x09
    long BrushDataOffset;                              // +0x0D (17 bytes)
};
class COldFileFormatLevel3 : public CReplaceSerialise<COldFileFormatLevel3, COldFileFormatLevel2> {
public:
    enum { VERSION = 3 };
    long MapDataOffset;                                // +0x05
    long ThingDataOffset;                              // +0x09
    long BrushDataOffset;                              // +0x0D
    long NavigationDataOffset;                         // +0x11 (21 bytes)
};
class CFileFormatLevel : public CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3> {
public:
    enum { VERSION = 4 };
    long MapDataOffset;                                // +0x05
    long ThingDataOffset;                              // +0x09
    long BrushDataOffset;                              // +0x0D
    long NavigationDataOffset;                         // +0x11
    long NavigationSectionsOffset;                     // +0x15 (25 bytes)
    void ConvertFromOld(const COldFileFormatLevel3* old)
    {
        MapDataOffset = old->MapDataOffset;
        ThingDataOffset = old->ThingDataOffset;
        BrushDataOffset = old->BrushDataOffset;
        NavigationDataOffset = old->NavigationDataOffset;
        NavigationSectionsOffset = 0;
    }
};
#pragma pack(pop)

template <>
CCountedPointer<COldFileFormatLevel3> __fastcall
CReplaceSerialise<COldFileFormatLevel3, COldFileFormatLevel2>::ConvertPreviousUnknownVersionUp(void* data);   // 0x00515E90
template <class T, class TOld>
CCountedPointer<T> __fastcall CReplaceSerialise<T, TOld>::ConvertPreviousUnknownVersionUp(void* data)
{
    CCountedPointer<T> result(new T);
    CCountedPointer<TOld> held;
    TOld* old = (TOld*)data;
    if (((TOld*)data)->SerialiseVersionNumber != TOld::GetClassVersionNumber()) {
        held = TOld::ConvertPreviousUnknownVersionUp(data);
        old = held.Data;
    }
    result.Data->ConvertFromOld(old);
    return result;
}

template CCountedPointer<CFileFormatLevel> __fastcall
CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>::ConvertPreviousUnknownVersionUp(void* data);