// GetStringFromDataMarker(const CCharString& marker, CCharString& out, const CDataMarkerOutput& data) -- copy the DataAsString of every element whose VariableName equals the marker into out
// Leaf: ?GetStringFromDataMarker@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD4FF0, 160 bytes, `__fastcall` global (marker in ecx, the out reference in
//   edx, the data marker output on the stack: `ret 4`) (PDB ?GetStringFromDataMarker@@YIXABVCCharString@@AAV1@ABVCDataMarkerOutput@@@Z).
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, inside the inlined CCharString::operator==), CCharString::operator= @ 0x0099EFB0.
// Layout (PDB): CDataMarkerOutput { vector<CElement> Elements }, CElement { CCharString VariableName +0, CCharString
//   DataAsString +4, unsigned long DataAsULONG +8 } (12 bytes: the 0x2AAAAAAB / `sar edx,1` divide).
// Shape: speed TU (no frame; the two register arguments spilled to the frame for the loop). An index loop over the
//   element vector: size() tested before the loop and recomputed at the bottom (begin re-read: the body may write
//   memory), `operator[]` is `*(begin() + n)` (the element address strength-reduced to ebx + esi), and every match
//   assigns (no early exit, so the last matching element wins).
class CCharStringData { public:                        // the shared string block (CBasicString<char>)
    const char*   m_pData;                      // +0x00
    unsigned long m_length;                     // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

class CCharString {
public:
    CCharStringData* PStringData;
    CCharString& operator=(const CCharString& other);           // 0x0099EFB0
    // The inlined equality: the same block, or both present with equal lengths and Compare == 0
    // (`neg eax ; sbb al,al ; inc al`).
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = PStringData;
        CCharStringData* b = other.PStringData;
        if (a == b)
            return true;
        if (a && b && a->m_length == b->m_length)
            return Compare_00411570(a->m_pData, b->m_pData) == 0;
        return false;
    }
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};
}
class CDataMarkerOutput {                       // 12 bytes in retail (PDB 16: Dinkumware vector)
public:
    class CElement {                            // 12 bytes
    public:
        CCharString   VariableName;             // +0x00
        CCharString   DataAsString;             // +0x04
        unsigned long DataAsULONG;              // +0x08
    };
    std::vector<CElement> Elements;            // +0x00
};

void __fastcall GetStringFromDataMarker(const CCharString& marker, CCharString& out, const CDataMarkerOutput& data)
{
    for (unsigned int i = 0; i < data.Elements.size(); ++i) {
        const CDataMarkerOutput::CElement& element = data.Elements[i];
        if (element.VariableName == marker)
            out = element.DataAsString;
    }
}