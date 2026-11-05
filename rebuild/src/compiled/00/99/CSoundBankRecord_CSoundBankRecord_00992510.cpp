// CSoundBankRecord::~CSoundBankRecord() -- destructor of a 0x20-byte sound-bank record: four strings, then two counted pointers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00992510, length 120 bytes.
// Callees: CCharString::~CCharString() @ 0x0099EAE0 (x4), operator delete @ 0x00BFE9BC (count blocks).
// Label correction: the manifest's "CEditFractal::~CEditFractal" is a propagated name. The record is the value half (+4) of the 0x24-byte
// elements of the vector at +0x30 of the sound manager whose destructor is 0x00991880 (vtable 0x0129A574, RTTI "CASoundManager"); the
// four strings are built by RegisterSoundBanks (0x009919C0, "Register Sound Bank 1/2"). The sound module is absent from the Ego PDB, so the
// class and member names are descriptive: two counted pointers at +0x00 / +0x08, four CCharStrings at +0x10..+0x1c.
// Shape: no vtable store; members destroyed in reverse order with one zero register (edi); each counted pointer is
// `if (block) { --count; if (count == 0) { deleter(object); delete block; } }` re-reading the block, then object = 0, block = 0.
// Leaf: ??1CSoundBankRecord
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
#include "engine/CCountedPointer.h"


class CCharString {
public:
    ~CCharString();                          // 0x0099EAE0
    class CCharStringData* PStringData;
};

struct CSoundBank;
struct CSoundBankData;

class CSoundBankRecord {
public:
    CCountedPointer<CSoundBank>     m_pBank;         // +0x00
    CCountedPointer<CSoundBankData> m_pData;         // +0x08
    CCharString                     m_name;          // +0x10
    CCharString                     m_fileName;      // +0x14
    CCharString                     m_dataFileName;  // +0x18
    CCharString                     m_description;   // +0x1c
    ~CSoundBankRecord();
};

CSoundBankRecord::~CSoundBankRecord()
{
}