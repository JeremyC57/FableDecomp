// CMemoryDataOutputStream::`scalar deleting destructor'(unsigned int) -- free the data buffer, destroy the inherited NewLine string, the inlined CDataStream destructor stores its vtable and calls the root base destructor, flag-tested delete
// Leaf: ??_GCMemoryDataOutputStream@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009973C0, 68 bytes, thiscall, `ret 4`.
// Identity: slot 0 of the CMemoryDataOutputStream vtable 0x0129A788 (rtti-vtables.tsv); the second vptr store 0x0129A69C is
//   CDataStream's (its own deleting destructor 0x00996EA0 stores it before the same base call). The catalog label
//   CWaterPatchMesh::`scalar_deleting_destructor' is propagated.
// Callees: free 0x00BFEA14 (Data is a CArray<char> over the malloc allocator), ~CCharString 0x0099EAE0 (NewLine, +0x04),
//   ~CBaseClassNonCopyable 0x0099A300 (out of line: stores the root vtable), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CDataOutputStream (vptr, NewLine +0x04) and CMemoryDataOutputStream (Data +0x08, CurrentPosition)
//   with the 12-byte retail CArray (PDB 16), as the landed constructor 0x00996AF0 models it. No engine/ headers.
// Shape: CMemoryDataOutputStream's user-declared destructor stores its vptr; CDataOutputStream declares none, so its
//   generated destructor drops its vtable reset and only runs ~CCharString; CDataStream's inline `virtual ~CDataStream() {}`
//   stores its vtable before the out-of-line root destructor. The out-of-line ~CMemoryDataOutputStream defined in the TU
//   emits the vtable, and VC7.1 inlines it into the deleting destructor.
#include <stdlib.h>
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
template <class T> class CArray {                        // retail CArray: STLport vector<T> over the malloc allocator
public:
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    ~CArray() { if (m_pData) free(m_pData); }
};
class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
class CDataStream : public CBaseClassNonCopyable {      // vtable 0x0129A69C
public:
    virtual ~CDataStream() {}
};
class CDataOutputStream : public CDataStream {           // vtable 0x0129A6E0
public:
    CCharString NewLine;                                 // +0x04
};
class CMemoryDataOutputStream : public CDataOutputStream {   // vtable 0x0129A788
public:
    virtual ~CMemoryDataOutputStream();
    CArray<char>  Data;                                  // +0x08
    unsigned long CurrentPosition;                       // +0x14
};
CMemoryDataOutputStream::~CMemoryDataOutputStream()
{
}