// CEntityScriptInfo::CEntityScriptInfo() -- default constructor: the Script CCharString, the SaveData array (three zero words: begin/end/capacity of the retail 12-byte CArray), UncompressedSize 0 and Valid false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052AB00, length 29 bytes (the oracle row carries 3 bytes of int3 padding).
// Callees: CCharString::CCharString() @ 0x0099E4B0. PDB layout (NThingManager::CEntityRuntimePersistence::CEntityScriptInfo): Script +0,
// SaveData +4 (CArray<unsigned char>, 16 bytes in the Ego PDB but 12 bytes in retail: begin/end/capacity), UncompressedSize +0x14 (PDB) ->
// +0x10 retail, Valid +0x18 (PDB) -> +0x14 retail. No vtable. The CArray is modelled locally as its three pointers (inline zeroing).
#include "fable_string.h"

struct CRetailByteArray_0052ab00 {                 // retail CArray<unsigned char>: begin, end, capacity end
    unsigned char* m_pBegin;
    unsigned char* m_pEnd;
    unsigned char* m_pCapacity;
    CRetailByteArray_0052ab00() : m_pBegin(0), m_pEnd(0), m_pCapacity(0) { }
};

struct CEntityScriptInfo {
    CCharString               Script;              // +0x00
    CRetailByteArray_0052ab00 SaveData;            // +0x04
    long                      UncompressedSize;    // +0x10
    bool                      Valid;               // +0x14
    CEntityScriptInfo();
};

CEntityScriptInfo::CEntityScriptInfo()
    : Script()
    , SaveData()
    , UncompressedSize(0)
    , Valid(false)
{
}
