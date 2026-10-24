// _global::_Cons_val(CLipSyncPhonemeSample*, const CLipSyncPhonemeSample&) -- STL construct helper: placement-copy-construct a lip-sync phoneme sample (symbol, id, shared speech frame) into raw vector storage
// Flags: /Gr
// Leaf: _Cons_val
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00a49b70, 43 bytes.
// Callee: CCharString copy ctor 0x0099EC30.
// Shape: link-time __fastcall (/Gr STL TU): destination in ecx (esi), source in edx (edi); the
// placement new's null test guards the inlined IMPLICIT copy constructor: the out-of-line symbol
// copy, the id dword, then the counted pointer's copy `Data(o.Data), Info(o.Info) { if (Info)
// ++Info->RefCount; }` (count at +0 of the info block; the info pointer reuses edi).
// Layout: Ego_r.pdb CLipSyncPhonemeSample (0x10) = CCharString Symbol +0, ulong ID +4,
// CCountedPointer<CSpeechFrame> SampleFrame +8 (Data +8, Info +0x0C) -- matches the label.
inline void* operator new(unsigned int, void* where) { return where; }

namespace FableCons_00a49b70 {

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

struct CCPPointerInfo {
    long RefCount;                                      // +0x00
    void (__fastcall* DeleteFunc)(void*);               // +0x04
    void* Data;                                         // +0x08
};

template<class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& p) : Data(p.Data), Info(p.Info) { if (Info) ++Info->RefCount; }
};

class CSpeechFrame;

struct CLipSyncPhonemeSample {                          // 0x10 bytes
    CCharString                   Symbol;               // +0x00
    unsigned long                 ID;                   // +0x04
    CCountedPointer<CSpeechFrame> SampleFrame;          // +0x08
};

void __fastcall _Cons_val(CLipSyncPhonemeSample* ptr, const CLipSyncPhonemeSample& value)
{
    new ((void*)ptr) CLipSyncPhonemeSample(value);
}

}