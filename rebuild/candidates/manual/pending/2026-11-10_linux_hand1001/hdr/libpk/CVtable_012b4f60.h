// CVtable_012b4f60 -- libpk's file handle over an outside file manager (the 12-byte handle the file-manager adapter CVtable_012b3224
// hands out): it forwards the libpk file-handle slots to the wrapped file object (+4) and, on Close, closes that object and asks
// its adapter (+8) to destroy the handle.
// Identity (descriptive; libpk has no PDB or RTTI): vtable 0x012B4F60 (5 slots), stored only by the landed constructor
//   CVtable_012b4f60::CVtable_012b4f60(void* first, void* second) 0x00C330A0 (the adapter at +8, the object at +4), whose only
//   caller is the landed CVtable_012b3224::Create 0x00C1B8B0 (`new CVtable_012b4f60(this, m_pFactory->Find(key))`). The adapter is
//   the file-manager object CSoundSystem::CSoundSystem 0x00C094A0 builds when the setup record carries an outside file manager,
//   else PKCDefaultFileManager 0x00C1AC40, whose own handle class is CVtable_012b4f4c (vtable 0x012B4F4C, ctor 0x00C32FC0): the
//   same five slots -- 0 GetSize(unsigned long*) 0x00C32F40, 1 Read(count, unused, a, b, c, d) 0x00C32F60 (`ret 18h`),
//   2 GetUnitSize() 0x00C32F30, 3 Close 0x00C32FA0, 4 the deleting destructor -- so this handle's slots carry those names.
// Slot 3 (0x00C33050: `m_pFile->Close(); m_pOwner->DestroyHandle(this);`) is landed as CEmitterOwner_00c33050::Validate and the
//   adapter's 0x00C1B930 (`if (handle) delete handle;`) as CTCDParticleEmitter::EditFrameUpdate (both propagated labels).
#ifndef FABLE_LIBPK_CVTABLE_012B4F60_H
#define FABLE_LIBPK_CVTABLE_012B4F60_H

class CUnprovenFileObject_012b4f60 {            // the wrapped file object: the libpk file-handle slots (outside implementation)
public:
    virtual bool GetSize(unsigned long* size);                                                  // slot 0
    virtual bool Read(unsigned long count, long unused, void* a, void* b, void* c, void* d);    // slot 1
    virtual unsigned long GetUnitSize();                                                        // slot 2
    virtual void Close();                                                                       // slot 3
};

class CVtable_012b3224;                         // the file-manager adapter that creates and destroys these handles

class CVtable_012b4f60 {                        // vtable 0x012B4F60, 12 bytes
public:
    CVtable_012b4f60(void* first, void* second);                                                // 0x00C330A0
    virtual bool GetSize(unsigned long* size);                                                  // slot 0: 0x00C33070
    virtual bool Read(unsigned long count, long unused, void* a, void* b, void* c, void* d);    // slot 1: 0x00C33080
    virtual unsigned long GetUnitSize();                                                        // slot 2: 0x00C33090
    virtual void Close();                                                                       // slot 3: 0x00C33050
    virtual ~CVtable_012b4f60();                                                                // slot 4 (deleting: 0x00C330C0)
    CUnprovenFileObject_012b4f60* m_pFile;      // +0x04 the wrapped file object (the constructor's second argument)
    CVtable_012b3224*             m_pOwner;     // +0x08 the adapter (the first argument)
};

#endif
