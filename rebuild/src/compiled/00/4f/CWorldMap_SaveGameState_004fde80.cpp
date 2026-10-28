// CWorldMap::SaveGameState(CPersistContext& context) -- persists the region game state: in the binary modes it transfers the region count and, for every region after the first, its name and region state; in the text modes it writes each region (from the second) between a `REGION "name"` header and an END_REGION line; each region ticks the loading-progress display
// Leaf: SaveGameState
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FDE80, 429 bytes (+ int3 padding), `ret 4`. PDB ?SaveGameState@CWorldMap@@QAEXAAVCPersistContext@@@Z.
// Callees: CPersistContext::CheckCRC 0x00404500, CPersistTraits<unsigned long>::TransferBinaryIn 0x0040F8A0 (label
//   <float> is a byte twin), CDataOutputStream::WriteSLONG 0x00993EB0, CCharString copy ctor 0x0099EC30,
//   CPersistContext::Transfer(CCharString&) 0x00431143, CRegion game-state transfer 0x006BC290, CCharString(const
//   char*, long) 0x0099EBF0, DisplayProgress 0x009E9F40 (`__fastcall`), ~CCharString 0x0099EAE0, operator+(const
//   char*, const CCharString&) 0x0099F690 and operator+(const CCharString&, const char*) 0x0099F600 (`__fastcall`),
//   the text-mode string writer 0x009BAB50; literals "" 0x0122D70E, "REGION \"" 0x012445FC, "\"\r\n" 0x01244608,
//   "END_REGION\r\n\r\n" 0x012445EC.
// Layout: retail CWorldMap Regions vector<CRegion> at +0x2C (0x58-byte CRegion, Name +0x18), as the landed
//   SetRegionAsLoaded 0x004FC8A0 (the PDB header rebuild/include/engine/CWorldMap.h has 16-byte containers: +0x34);
//   CPersistContext Mode +0x18, load stream +0x24, save stream +0x28.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString&, float, bool, bool); }   // 0x009E9F40   // 0x009E9F40

class CDataOutputStream { public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};
class CMemoryDataInputStream;
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* value);   // 0x0040F8A0
};

template <class T> struct CPersistTraits;
class CMemoryDataInputStream;
extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
template <> void CPersistTraits<unsigned long>::TransferBinaryIn(CMemoryDataInputStream* stream, unsigned long* value)
{
    *value = (unsigned long)StandInSink((int)stream, (int)DefaultVal);
}
#pragma auto_inline(on)
class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    Mode;                        // +0x18
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;                 // +0x24
    CDataOutputStream*      PSaveStream;                 // +0x28
    protected: void CheckCRC(const char* name); public:                     // 0x00404500
    template <class T> void Transfer(T& value);                   // 0x00431143
    void WriteText(const CCharString& text);             // 0x009BAB50
    bool IsBinary() const { return Mode == 2 || Mode == 3; }
    __forceinline void TransferBinary(unsigned long& value)
    {
        CheckCRC("");
        CPersistTraits<unsigned long> traits;
        switch (Mode) {
        case 3:
            PSaveStream->WriteSLONG(value);
            break;
        case 2:
            traits.TransferBinaryIn(PLoadStream, &value);
            break;
        }
    }
};

class CRegion { public:                                         // 0x58 bytes
    unsigned char _pad00[0x18];
    CCharString   Name;                                  // +0x18
    unsigned char _rest[0x58 - 0x1c];
    void TransferGameState(CPersistContext& context);    // 0x006BC290
};
template <class T> struct CRetailVector {
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    unsigned long size() const { return Finish - Start; }
    T* begin() { return Start; }
    T& operator[](unsigned long i) { return *(begin() + i); }
};

class CWorldMap { public:
    unsigned char          _pad[0x2c];
    CRetailVector<CRegion> Regions;             // +0x2C
    void SaveGameState(CPersistContext& context);
};

void CWorldMap::SaveGameState(CPersistContext& context)
{
    unsigned long count = Regions.size();
    if (context.IsBinary()) {
        context.TransferBinary(count);
        for (unsigned long i = 1; i < count; ++i) {
            CCharString name(Regions[i].Name);
            context.Transfer(name);
            Regions[i].TransferGameState(context);
            {
                CCharString text("");
                NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
            }
        }
    } else {
        for (unsigned long i = 1; i < count; ++i) {
            context.WriteText("REGION \"" + Regions[i].Name + "\"\r\n");
            Regions[i].TransferGameState(context);
            {
                CCharString end("END_REGION\r\n\r\n");
                context.WriteText(end);
            }
            {
                CCharString text("");
                NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
            }
        }
    }
}