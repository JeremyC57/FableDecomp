// CVtable_012b25a8::Lock() -- libpk bank object: enter both member containers (+0x60 then +0x50) through their virtual slot 6
// Leaf: ?Lock@CVtable_012b25a8@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E5E0, 22 bytes, thiscall; the second virtual call is a tail `jmp [edx+18h]`.
// Identity: libpk (0x00C08AE0..~0x00C40000: the GameCODA / PK sound middleware; no PDB, no RTTI, so every name here is descriptive).
//   The class is the one whose constructor 0x00C0EDF0 stores vtable 0x012B25A8 and builds the two 0x10-byte containers at +0x50
//   (vtable 0x012B2350) and +0x60 (vtable 0x012B23A0), each holding a pointer (+4) to its embedded .\PKCTRMorphable.cpp object (+8);
//   it is named by the libpk vtable-address convention (catalog: 0x00C0FFA0 = "CVtable_012b25a8::`scalar deleting destructor'").
//   The only caller is libpk CSystem's bank factory 0x00C08D40 ("LoadBank", parked chunk 692; .\CSystem.cpp), which calls
//   0x00C0E5E0 on the new bank right before the bank load 0x00C17640 and 0x00C0E5C0 right after it, each next to the matching
//   lock-pair call (0x00C10100 slot 6 before, 0x00C100F0 slot 5 after): Lock 0x00C0E5E0 enters +0x60 then +0x50 through slot 6,
//   Unlock 0x00C0E5C0 leaves +0x50 then +0x60 through slot 5 (reverse order). Lock / Unlock are the chunk 692 descriptive names.
//   The catalog label "CMapsAndRegionsDialog::OpenMapDialog" (on both rows) is propagated and wrong.
// Shape: a virtual call on an embedded member object stays an indirect `call [eax+N]` under VC7.1 (no devirtualisation), so
//   the plain member calls give retail's `lea ecx,[esi+N]` / vtable loads; the last call becomes the tail jump.
// Layout: no shared header (libpk); members at the constructor's offsets, unmodelled ones as opaque storage.

class CVtable_012b1ab0 {                                    // libpk container root: vtable 0x012B1AB0, slots 0-3 _purecall, slot 4 ??_G 0x00C09890
public:
    virtual void Slot0_012b1ab0() = 0;                      // descriptive (libpk has no PDB / RTTI)
    virtual void Slot1_012b1ab0() = 0;
    virtual void Slot2_012b1ab0() = 0;
    virtual void Slot3_012b1ab0() = 0;
    virtual ~CVtable_012b1ab0();                            // slot 4
};

class CVtable_012b2350 : public CVtable_012b1ab0 {          // vtable 0x012B2350 (12 slots), 0x10 bytes
public:
    virtual void Slot0_012b1ab0();                          // 0x00C0F660
    virtual void Slot1_012b1ab0();                          // 0x00C0F680
    virtual void Slot2_012b1ab0();                          // 0x00C0F6A0
    virtual void Slot3_012b1ab0();                          // 0x00C0F6C0
    virtual ~CVtable_012b2350();                            // slot 4 0x00C0FB80
    virtual void Unlock();                                  // slot 5 0x00C0F1A0: m_pMorph->slot 2 (this); descriptive
    virtual void Lock();                                    // slot 6 0x00C0F1B0: m_pMorph->slot 3 (this); descriptive
    void*         m_pMorph;                                 // +0x04 -> m_morph
    unsigned char m_morph[0x8];                             // +0x08 the .\PKCTRMorphable.cpp object (ctor 0x00C185B0)
};

class CVtable_012b23a0 : public CVtable_012b1ab0 {          // vtable 0x012B23A0 (12 slots), 0x10 bytes
public:
    virtual void Slot0_012b1ab0();                          // 0x00C0F860
    virtual void Slot1_012b1ab0();                          // 0x00C0F880
    virtual void Slot2_012b1ab0();                          // 0x00C0F8A0
    virtual void Slot3_012b1ab0();                          // 0x00C0F8C0
    virtual ~CVtable_012b23a0();                            // slot 4 0x00C0FBA0
    virtual void Unlock();                                  // slot 5 0x00C0F2B0
    virtual void Lock();                                    // slot 6 0x00C0F2C0
    void*         m_pMorph;                                 // +0x04 -> m_morph
    unsigned char m_morph[0x8];                             // +0x08
};

class CVtable_012b25a8 {                                    // libpk bank object: vtable 0x012B25A8 (7 slots), 0xA4 bytes; ctor 0x00C0EDF0, dtor 0x00C0ECE0
public:
    virtual void Slot0_012b25a8();                          // 0x00C0E970 (descriptive slot names)
    virtual void Slot1_012b25a8();                          // 0x00C0E980
    virtual void Slot2_012b25a8();                          // 0x00C0EB70
    virtual void Slot3_012b25a8();                          // 0x00C0EA10
    virtual void Slot4_012b25a8();                          // 0x00C0E6B0
    virtual void Slot5_012b25a8();                          // 0x00C0E600
    virtual ~CVtable_012b25a8();                            // slot 6 ??_G 0x00C0FFA0 -> 0x00C0ECE0
    void Lock();                                            // 0x00C0E5E0
    void Unlock();                                          // 0x00C0E5C0
    long             m_handle;                              // +0x04 (ctor: 0)
    void*            m_pOwner;                              // +0x08 (ctor argument)
    unsigned char    m_0c[0x20];                            // +0x0C container, ctor 0x00C0A6D0
    unsigned char    m_buffer[0xc];                         // +0x2C buffer (pointer freed in the dtor) + two words
    unsigned char    m_38[0x10];                            // +0x38 container, vtable 0x012B2560
    long             m_48;                                  // +0x48 (ctor: 0)
    long             m_4c;                                  // +0x4C (ctor: 1)
    CVtable_012b2350 m_50;                                  // +0x50
    CVtable_012b23a0 m_60;                                  // +0x60
    long             m_70;                                  // +0x70 (ctor: -1)
    unsigned char    m_74[0x30];                            // +0x74 container, vtable 0x012B23F0
};

void CVtable_012b25a8::Lock()
{
    m_60.Lock();
    m_50.Lock();
}