// CActivationReceptor_00c38b20::CActivationReceptor_00c38b20() -- EH-framed default constructor of the 0x180-byte libpk element that the owner at 0x00C2D2F0 builds twice with __ehvec_ctor: five head+entry parts (+0x00 base, +0x2C, +0x60, +0x94, +0xCC), the state word +0xF8 = 0, the +0xFC registry, the +0x138 interpolated float (0, 0), then the -1 / 0 / 1.0f tail words
// Leaf: ??0CActivationReceptor_00c38b20@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C39130, 265 bytes, thiscall, plain `ret`, C++ EH frame (handler 0x01214B17).
// Caller: only as data: 0x00C2D326 pushes it (with the destructor 0x00C38B20, count 2, size 0x180) for the eh-vector
//   constructor of the owner's +0x18 array. Its destructor 0x00C38B20 is landed under this class name
//   (CActivationReceptor_00c38b20, descriptive: libpk has no PDB/RTTI; the manifest's "CTCTavernGameSpotTheAddition_
//   Constructor" is propagated junk).
// Layout (from this body and the landed destructor's state order 6..0): each part is a polymorphic 12/20/24-byte head
//   MEMBER (no part vtable store: a head base would give each part its own vptr store) with a trivial destructor (its
//   out-of-line constructor stores the head vtable) followed by a 0x20-byte entry built
//   from (part, 1); a part's own destructor (0x00C39BA0 / BF0 / C40 / C90 / CE0) only destroys the entry, so a part is
//   ONE EH state: base 0 (head 0x00C39D30 + entry 0x00C39290), +0x2C 1 (0x00C39F10 + entry at +0x14 0x00C39450), +0x60 2
//   (0x00C3A110 + 0x00C39610), +0x94 3 (0x00C3A310 + entry at +0x18 0x00C397D0), +0xCC 4 (0x00C3A520 + 0x00C39990),
//   registry +0xFC 5 (0x00C48640); the +0x138 CInterpolatedFloat(0, 0) 0x00C3A970 is last (nothing after it can throw).
// Names: every class here is a landed placeholder (one spelling per callee address, as each callee's own landed row
//   spells its constructor). The part destructors keep the spellings the landed element destructor binds; their own
//   landed rows spell them CPair_<addr>::~CPair_<addr> (review at landing).
// Shape: the parts are base / members in address order with inline constructors `: m_entry(this, (void*)1) {}`; the
//   state word is a mem-initializer (stored between the +0xCC part and the registry), the tail words are body stores
//   in retail order (the three -1 words, the sink pointer, the 1.0f).
struct CBase_00c1c810 {
    void* m_data;
    CBase_00c1c810();                                    // 0x00C1C810
    ~CBase_00c1c810();                                   // 0x00C1C820
};
struct CScriptEntry_00c39290 : CBase_00c1c810 { unsigned char _pad[0x14]; void* m_pFirst; void* m_member; CScriptEntry_00c39290(void* first, void* second); };   // 0x00C39290
struct CScriptEntry_00c39450 : CBase_00c1c810 { unsigned char _pad[0x14]; void* m_pFirst; void* m_member; CScriptEntry_00c39450(void* first, void* second); };   // 0x00C39450
struct CScriptEntry_00c39610 : CBase_00c1c810 { unsigned char _pad[0x14]; void* m_pFirst; void* m_member; CScriptEntry_00c39610(void* first, void* second); };   // 0x00C39610
struct CScriptEntry_00c397d0 : CBase_00c1c810 { unsigned char _pad[0x14]; void* m_pFirst; void* m_member; CScriptEntry_00c397d0(void* first, void* second); };   // 0x00C397D0
struct CScriptEntry_00c39990 : CBase_00c1c810 { unsigned char _pad[0x14]; void* m_pFirst; void* m_member; CScriptEntry_00c39990(void* first, void* second); };   // 0x00C39990

class CUnprovenBase_00c39d30 {                           // 12 bytes, vtable 0x012B5484
public:
    CUnprovenBase_00c39d30();                            // 0x00C39D30
    virtual void v00();
    void* Field04;
    void* Field08;
};
struct CSlotTable_00c39f10 {                             // 0x14 bytes, vtable 0x012B5494
    virtual void v00();
    unsigned long m_slots[3];
    unsigned long m_count;
    CSlotTable_00c39f10();                               // 0x00C39F10
};
struct CSlotTable_00c3a110 {                             // 0x14 bytes
    virtual void v00();
    unsigned long m_slots[3];
    unsigned long m_count;
    CSlotTable_00c3a110();                               // 0x00C3A110
};
struct CSlotTable_00c3a310 {                             // 0x18 bytes
    virtual void v00();
    unsigned long m_slots[4];
    unsigned long m_count;
    CSlotTable_00c3a310();                               // 0x00C3A310
};
class CUnprovenBase_00c3a520 {                           // 12 bytes, vtable 0x012B54C4
public:
    CUnprovenBase_00c3a520();                            // 0x00C3A520
    virtual void v00();
    void* Field04;
    void* Field08;
};

struct CReceptorBase_00c38b20 {   // 0x2C
    CUnprovenBase_00c39d30 m_head;                       // +0x00
    CScriptEntry_00c39290 m_entry;                       // +0x0C
    CReceptorBase_00c38b20() : m_entry(this, (void*)1) {}
    ~CReceptorBase_00c38b20();                           // 0x00C39BA0
};
struct CPart2C_00c38b20 {          // 0x34
    CSlotTable_00c39f10 m_head;                       // +0x00
    CScriptEntry_00c39450 m_entry;                       // +0x14
    CPart2C_00c38b20() : m_entry(this, (void*)1) {}
    ~CPart2C_00c38b20();                                 // 0x00C39BF0
};
struct CPart60_00c38b20 {          // 0x34
    CSlotTable_00c3a110 m_head;                       // +0x00
    CScriptEntry_00c39610 m_entry;                       // +0x14
    CPart60_00c38b20() : m_entry(this, (void*)1) {}
    ~CPart60_00c38b20();                                 // 0x00C39C40
};
struct CPart94_00c38b20 {          // 0x38
    CSlotTable_00c3a310 m_head;                       // +0x00
    CScriptEntry_00c397d0 m_entry;                       // +0x18
    CPart94_00c38b20() : m_entry(this, (void*)1) {}
    ~CPart94_00c38b20();                                 // 0x00C39C90
};
struct CPartCC_00c38b20 {       // 0x2C
    CUnprovenBase_00c3a520 m_head;                       // +0x00
    CScriptEntry_00c39990 m_entry;                       // +0x0C
    CPartCC_00c38b20() : m_entry(this, (void*)1) {}
    ~CPartCC_00c38b20();                                 // 0x00C39CE0
};
struct CAsyncDecoder_00c48640 {                          // +0xFC registry, 0x3C bytes
    unsigned char _body[0x3c];
    CAsyncDecoder_00c48640();                            // 0x00C48640
    ~CAsyncDecoder_00c48640();                           // 0x00C48290
};
struct CInterpolatedFloat {                              // +0x138, 0x34 bytes
    float m_current;
    float m_target;
    unsigned char _pad_0x08[0x28];
    bool m_isAngle;
    CInterpolatedFloat(float target, float time);        // 0x00C3A970
};
struct ISink_00c38b20;

struct CActivationReceptor_00c38b20 : CReceptorBase_00c38b20 {   // 0x180 bytes
    CPart2C_00c38b20 m_2c;                               // +0x2C
    CPart60_00c38b20 m_60;                               // +0x60
    CPart94_00c38b20 m_94;                               // +0x94
    CPartCC_00c38b20 m_cc;                               // +0xCC (entry +0xD8)
    long m_state;                                        // +0xF8
    CAsyncDecoder_00c48640 m_registry;                   // +0xFC
    CInterpolatedFloat m_138;                            // +0x138
    ISink_00c38b20* m_pSink;                             // +0x16C
    long m_170;                                          // +0x170
    long m_174;                                          // +0x174
    long m_178;                                          // +0x178
    float m_scale;                                       // +0x17C
    CActivationReceptor_00c38b20();
    ~CActivationReceptor_00c38b20();                     // 0x00C38B20
};

CActivationReceptor_00c38b20::CActivationReceptor_00c38b20()
    : m_state(0), m_138(0.0f, 0.0f)
{
    m_170 = -1;
    m_174 = -1;
    m_178 = -1;
    m_pSink = 0;
    m_scale = 1.0f;
}