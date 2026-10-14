// CFilteredEventContainer_007441a0::`scalar deleting destructor'(unsigned int flags) -- the CAnimFilterBase member at +0x40 destroyed inline (its vtable 0x0122FFCC stored), then the event-container base destructor (0x00743D40) and free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007441A0, length 48 bytes.
// Leaf: ??_GCFilteredEventContainer_007441a0@@QAEPAXI@Z
// The chunk label is propagated; named by address. Shape: the member's vtable store, the out-of-line base destructor, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CAnimFilterBase {                           // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }                         // inline: the vtable store
};
struct CEventContainer_00743d40 {
    virtual void v00();
    unsigned char _body[0x3c];
    ~CEventContainer_00743d40();                   // 0x00743D40 (labelled "NEntityEvents_CEventContainer_Destructor")
};
struct __declspec(novtable) CFilteredEventContainer_007441a0 : CEventContainer_00743d40 {
    CAnimFilterBase m_filter;                      // +0x40
    ~CFilteredEventContainer_007441a0() { }
};

void Kill_007441a0(CFilteredEventContainer_007441a0* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
