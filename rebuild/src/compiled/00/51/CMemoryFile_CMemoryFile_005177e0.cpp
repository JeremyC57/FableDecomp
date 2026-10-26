// CMemoryFile::CMemoryFile() -- default constructor over the CBaseClass root: own vtable, the DataBuffer array zeroed (retail 12-byte CArray), Opened false, then the Pathname CWideString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005177E0, length 40 bytes (the oracle row carries 8 bytes of int3 padding).
// Callees: CBaseClass::CBaseClass @ 0x0099A2F0, CWideString::CWideString() @ 0x0099AED0. This class's vtable is 0x0122E0FC (rtti-vtables.tsv:
// CMemoryFile); the manifest label "CEntitySoundDef::CEntitySoundDef" is propagated. PDB layout (CMemoryFile, 40 bytes): AccessType +4,
// Position +8, Length +0xc (left for Open), DataBuffer +0x10 (16-byte PDB CArray = 12 bytes retail), Opened +0x20 -> +0x1c retail,
// Pathname +0x24 -> +0x20 retail. The engine header (CMemoryFile.h) carries the PDB offsets, so the retail layout is modelled locally.
#include "fable_string.h"

namespace N_0099a2f0 { struct CBaseClass {
    virtual void v00();
    CBaseClass();                                  // 0x0099A2F0
}; } using N_0099a2f0::CBaseClass;

struct CRetailByteArray_005177e0 {                 // retail CArray<unsigned char>: begin, end, capacity end
    unsigned char* m_pBegin;
    unsigned char* m_pEnd;
    unsigned char* m_pCapacity;
    CRetailByteArray_005177e0() : m_pBegin(0), m_pEnd(0), m_pCapacity(0) { }
};

class CMemoryFile : public CBaseClass { public:                  // vtable 0x0122E0FC
    long                      AccessType;          // +0x04 FileAccessType (not initialised here)
    long                      Position;            // +0x08
    long                      Length;              // +0x0c
    CRetailByteArray_005177e0 DataBuffer;          // +0x10
    bool                      Opened;              // +0x1c
    CWideString               Pathname;            // +0x20
    virtual void v00();
    CMemoryFile();
};

CMemoryFile::CMemoryFile()
    : CBaseClass()
    , DataBuffer()
    , Opened(false)
    , Pathname()
{
}