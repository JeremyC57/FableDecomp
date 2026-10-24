// LHNetworkLib::LHHttpDocumentParts::~LHHttpDocumentParts() -- frees the part's byte buffer (array delete) and clears its pointer and size
// Leaf: ??1LHHttpDocumentParts
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083EA10, 33 bytes. Ego_r.pdb names ??1LHHttpDocumentParts@LHNetworkLib@@QAE@XZ.
// Callee: operator delete[] @ 0x00BFEB1C (MSVCR71 ??_V import thunk; declared globally so `delete[]` binds it, not ??3).
// Layout: one received document chunk of LHHttp2::DocumentParts (vector<LHHttpDocumentParts*> at retail +0x884):
// Ego_r.pdb LHHttpDocumentParts (12 bytes): +0 char* Buffer (new char[n]), +4 BufferLength, +8 CurrBufPos (not touched).
// No engine header exists for the record; local model with the PDB member names.
// Shape: the null-tested `delete[]` plus the pointer clear inside the if, then the unconditional size clear.
void __cdecl operator delete[](void* block);       // 0x00BFEB1C

namespace LHNetworkLib {

struct LHHttpDocumentParts {
    char*         Buffer;                          // +0x00
    unsigned long BufferLength;                    // +0x04
    unsigned long CurrBufPos;                      // +0x08
    ~LHHttpDocumentParts();
};

LHHttpDocumentParts::~LHHttpDocumentParts()
{
    if (Buffer) {
        delete[] Buffer;
        Buffer = 0;
    }
    BufferLength = 0;
}

}