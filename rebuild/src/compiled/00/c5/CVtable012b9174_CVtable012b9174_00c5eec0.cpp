// CVtable_012b9174::CVtable_012b9174() -- default constructor: own vtable, one pointer at +0x604 nulled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5EEC0, oracle length 19 bytes.
// Callees: none.
//
// Retail bytes:  mov eax,ecx ; mov dword ptr [eax],offset vftable ; mov dword ptr [eax+604h],0 ; ret
// The vtable 0x012B9174 has no RTTI locator row (a no-RTTI module: 8 slots, 0x00C5ED80..) so the
// class is named by its vtable; the manifest's "CBaseIntelligentPointer" label is propagated.  The
// 0x600 bytes between the vptr and the nulled member are left to other constructors / untouched.
struct CVtable_012b9174 {
    virtual void v00();
    unsigned char _body[0x600];    // +0x004 .. +0x603 untouched here
    void*         m_pCurrent;      // +0x604
    CVtable_012b9174();            // 0x00C5EEC0
};

CVtable_012b9174::CVtable_012b9174()
    : m_pCurrent(0)
{
}