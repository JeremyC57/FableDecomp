// SThingRecord_00748530::SThingRecord_00748530(const SThingRef& ref) -- build an intelligent pointer from the reference's thing and copy its two words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00748530, length 50 bytes (+ int3 padding).
// Leaf: ??0SThingRecord_00748530
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10, SetPItem @ 0x00A01B90. Vtable 0x01237F1C = CIntelligentPointer.
// The catalog label "CWADFile::CWADFile" is propagated (the body stores no CWADFile vtable and has no CBase call).
// Shape: the thing pointer is read into ebx BEFORE the base constructor (the converting constructor's argument), then the
// vtable and SetPItem; the two trailing words are copied through eax/ecx after it: a plain initialiser list.
class CBaseObject;
class CThing;
class CThingTrackNode;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                   // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
struct CIntelligentPointer : CBaseIntelligentPointer {    // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(CBaseObject* item) { SetPItem(item); }
};
struct SThingRef { CBaseObject* m_pThing; long m_a; long m_b; };
struct SThingRecord_00748530 {
    CIntelligentPointer m_thing;   // +0x00
    long m_a;                      // +0x08
    long m_b;                      // +0x0c
    SThingRecord_00748530(const SThingRef& ref);
};
SThingRecord_00748530::SThingRecord_00748530(const SThingRef& ref) : m_thing(ref.m_pThing), m_a(ref.m_a), m_b(ref.m_b)
{
}