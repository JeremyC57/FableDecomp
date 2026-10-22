#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CVtable0125a670::CVtable0125a670(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), two zero counters, a zero and a unit float, a zero word, a list member built with a default allocator, own vtable, three primitive handles reserved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064ED06, length 73 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65 (`push [ebp+0Ch] ; push [ebp+8]`), the list constructor @ 0x00655AD8 (on +0x3c, the empty allocator
// tag by reference, homed in the high byte of a dead argument slot `lea eax,[ebp+0Fh]`), the primitive-handle vector's Reserve @ 0x00443F90 (on +0x1c, 3).
// Vtable 0x0125A670 is not in rtti-vtables.tsv (a CDrawBase-derived drawer with counters, two floats and a list); the chunk label
// "CDrawMeleeActionButtonEffects::CDrawMeleeActionButtonEffects" belongs to the sibling @ 0x00654258 (vtable 0x0125A714). Frame-pointer size TU.
// Shape: the float zero/one go through `fldz`/`fld1`, the longs share `xor eax,eax`; the vtable store sits between the tag's lea and the list call.
struct CPlayerGui;

struct CRetailAllocator { CRetailAllocator() { } };

struct CRetailList_0064ed06 {                      // VC6 Dinkumware list: one sentinel pointer
    void* head;
    CRetailList_0064ed06(const CRetailAllocator& allocator);   // 0x00655AD8
};

namespace NPlayerGui {

struct CRetailHandleVector {
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    void Reserve(long count);                      // 0x00443F90
};

struct CDrawBase {
    virtual void v00();
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

struct CDrawPerceivers : CDrawBase {               // vtable 0x0125A670
    virtual void v00();
    long  m_counterA;                              // +0x28
    long  m_counterB;                              // +0x2c
    float m_progress;                              // +0x30
    float m_scale;                                 // +0x34
    long  m_state;                                 // +0x38
    CRetailList_0064ed06 m_entries;                // +0x3c
    CDrawPerceivers(CPlayerGui* gui, void* drawList);
};

CDrawPerceivers::CDrawPerceivers(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), m_counterA(0), m_counterB(0), m_progress(0.0f), m_scale(1.0f), m_state(0), m_entries(CRetailAllocator())
{
    PrimitiveHandles.Reserve(3);
}

}