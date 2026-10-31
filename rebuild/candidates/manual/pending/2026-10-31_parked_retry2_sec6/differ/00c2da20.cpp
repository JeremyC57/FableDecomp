// CLHSegmentReader::CLHSegmentReader(void* source, long size) -- open a Lionhead segment file over the buffered reader: check the "LiOnHeAd" signature, then walk the 0x24-byte segment headers (32-byte name, 4-byte size), registering each segment's name, data offset and size; any truncation bumps the error count; finally clear the read range
// Leaf: ??0CLHSegmentReader
// Grade: DIFFER(443v448) -- needs --flags /EHsc /GS; `pos = segment->Offset + segment->Size` gives retail's Size-then-
//   Offset loads; left: retail hoists the `mov ecx,ebp` this-setup above the strcmp / Read failure branches (two error
//   tails), ours sets it after; tried this pass an inverted strcmp test (same)
// Flags: /EHsc /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2DA20, 448 bytes, `ret 8`, EH frame and /GS cookie. Catalog label
//   CGuiWindowManager::LoadDrawEnvironment is propagated; named descriptively.
// Callees: CBufferedReader(void*, long) 0x00C3AAC0, the segment array ctor 0x00C194F0, SetRange 0x00C3AA50,
//   Read 0x00C3A9E0, GetCount 0x00C3AA30, operator new 0x00BFEA1A, CCharString assign-from-text 0x00C0C200; the
//   segment list's virtual slot 9 (add); the empty-string data global 0x0139C900.
// Layout: local projection -- error count +0x7c, the segment list +0x80 (vtable 0x012B47D0, array pointer +0x84,
//   array +0x88).
#include <string.h>
struct SReaderParams { long Size; long Offset; long Mode; };
struct CBufferedReader {
    virtual void v00();
    unsigned char Body[0x78];
    CBufferedReader(void* source, long size);            // 0x00C3AAC0
    ~CBufferedReader();
    long SetRange(unsigned long first, unsigned long count);   // 0x00C3AA50
    long Read(void* buffer, long count);                 // 0x00C3A9E0
    unsigned long GetCount() const;                      // 0x00C3AA30
};
struct CStringData;
extern CStringData* g_EmptyStringData;                   // 0x0139C900
struct CSegmentName {
    CStringData* Data;
    CSegmentName() : Data(g_EmptyStringData) { }
    CSegmentName& operator=(const char* text);           // 0x00C0C200
};
struct CSegment {
    unsigned long Offset;
    long          Size;
    CSegmentName  Name;
};
struct CSegmentArray {
    unsigned char Data[0x14];
    CSegmentArray(long reserve);                         // 0x00C194F0
    ~CSegmentArray();
};
struct CSegmentListBase {
    virtual ~CSegmentListBase();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void Add(CSegment* segment);   // slot 9 (+0x24)
    CSegmentArray* PArray;
    CSegmentListBase(CSegmentArray* array) : PArray(array) { }
};
struct CSegmentList : CSegmentListBase {
    CSegmentArray Array;
    CSegmentList() : CSegmentListBase(&Array), Array(0) { }
};
struct __declspec(novtable) CLHSegmentReader : CBufferedReader {
    long         ErrorCount;                             // +0x7c
    CSegmentList Segments;                               // +0x80
    CLHSegmentReader(void* source, long size);
};

CLHSegmentReader::CLHSegmentReader(void* source, long size)
    : CBufferedReader(source, size), ErrorCount(0)
{
    if (SetRange(0, 8) < 0) {
        ++ErrorCount;
        SetRange(0, 0);
        return;
    }
    char signature[9];
    signature[8] = 0;
    if (Read(signature, 8) < 8) {
        ++ErrorCount;
        SetRange(0, 0);
        return;
    }
    if (strcmp(signature, "LiOnHeAd") == 0) {
        unsigned long pos = 8;
        if (GetCount() > pos) {
            do {
                if (SetRange(pos, 0x24) < 0) {
                    ++ErrorCount;
                    break;
                }
                char name[0x21];
                name[0x20] = 0;
                if (Read(name, 0x20) < 0x20) {
                    ++ErrorCount;
                    break;
                }
                long segmentSize;
                if (Read(&segmentSize, 4) < 4) {
                    ++ErrorCount;
                    break;
                }
                if (pos + 0x24 + segmentSize > GetCount()) {
                    ++ErrorCount;
                    break;
                }
                CSegment* segment = new CSegment;
                segment->Offset = pos + 0x24;
                segment->Size = segmentSize;
                segment->Name = name;
                Segments.Add(segment);
                pos = segment->Offset + segment->Size;
            } while (pos < GetCount());
        }
    } else {
        ++ErrorCount;
    }
    SetRange(0, 0);
}
