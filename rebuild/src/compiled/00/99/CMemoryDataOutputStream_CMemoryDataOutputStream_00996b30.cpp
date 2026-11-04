// CMemoryDataOutputStream::~CMemoryDataOutputStream() -- free the Data byte array, then the inlined base destructors: the NewLine string (~CDataOutputStream) and ~CDataStream, chaining to ~CBaseClassNonCopyable
// Leaf: ??1CMemoryDataOutputStream@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00996B30, 47 bytes, thiscall, tail `jmp` to the root destructor.
// Identity (proven): the body stores vtable 0x0129A788 = .?AVCMemoryDataOutputStream@@ (RTTI COL: CMemoryDataOutputStream -> CDataOutputStream
//   -> CDataStream -> CBaseClassNonCopyable -> CBaseClass; slots 1-4 SetPosition 0x00994B10, GetPosition 0x009945B0, Close 0x009945C0, Write
//   0x00996CC0), then the CDataStream vtable 0x0129A69C, and chains to ~CBaseClassNonCopyable 0x0099A300. The scalar deleting dtor
//   0x009973C0 (slot 0 of 0x0129A788) expands the same body, and the 36 direct callers destroy stack-local memory streams (the landed
//   ctor 0x00996AF0 builds them). Name: Ego_r.pdb ??1CMemoryDataOutputStream@@UAE@XZ. The catalog label
//   "~CCompressedMemoryDataInputStream" is wrong (that class's vtable is not stored here).
// Callees: free 0x00BFEA14 (STLport vector deallocation; statically linked CRT), CCharString::~CCharString 0x0099EAE0 (NewLine +0x04),
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (protected virtual, engine/CBaseClassNonCopyable.h).
// Layout: engine/CMemoryDataOutputStream.h (retail): vptr, NewLine +0x04 (CDataOutputStream), Data (CArray<char>, STLport vector, 12 bytes)
//   +0x08, CurrentPosition +0x14, sizeof 0x18. The header flattens the vptr, so the chain is a local polymorphic model sized from it.
// Shape: CMemoryDataOutputStream has a user destructor (its vtable store), CDataOutputStream none (no 0x0129A6E0 store: a compiler-generated
//   dtor drops it, GOTCHAS), CDataStream an inline user `{}` (the 0x0129A69C store before the tail jump).
#include "engine/CBaseClassNonCopyable.h"
#define CMemoryDataOutputStream CMemoryDataOutputStreamFields
#include "engine/CMemoryDataOutputStream.h"
#undef CMemoryDataOutputStream
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
#include "engine/CArray.h"
class CCharStringData;
class CCharString {
public:
    ~CCharString();                                        // 0x0099EAE0
private:
    CCharStringData* PStringData;
};
class CDataStream : public CBaseClassNonCopyable {
public:
    virtual ~CDataStream() { }                             // vtable 0x0129A69C
    virtual void SetPosition(unsigned long position) = 0;  // slot 1
    virtual unsigned long GetPosition() const = 0;         // slot 2
    virtual void Close() = 0;                              // slot 3
};
class CDataOutputStream : public CDataStream {
public:
    virtual void Write(const void* data, long length) = 0; // slot 4
protected:
    CCharString NewLine;                                   // +0x04
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    virtual ~CMemoryDataOutputStream();
    virtual void SetPosition(unsigned long position);      // 0x00994B10
    virtual unsigned long GetPosition() const;             // 0x009945B0
    virtual void Close();                                  // 0x009945C0
    virtual void Write(const void* data, long length);     // 0x00996CC0
private:
    CArray<char> Data;                                     // +0x08
    unsigned long CurrentPosition;                         // +0x14
};
typedef char CMemoryDataOutputStreamExtent[sizeof(CMemoryDataOutputStream) == sizeof(CMemoryDataOutputStreamFields) ? 1 : -1];

CMemoryDataOutputStream::~CMemoryDataOutputStream()
{
}