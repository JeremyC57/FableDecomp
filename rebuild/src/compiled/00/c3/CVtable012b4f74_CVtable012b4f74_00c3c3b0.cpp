// CVtable_012b4f74::CVtable_012b4f74() -- base CVtable_012b5860() called, this class's vtable, then an embedded seven-dword record at +0x10 and the word at +0x2c cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C3B0, length 46 bytes.
// Leaf: ??0CVtable_012b4f74
// Callees: CVtable_012b5860::CVtable_012b5860() @ 0x00C3C590 (landed: base 0x00C4CF20, vtable 0x012B5860, +0xc cleared).
// The manifest label "CShopItemDef::CShopItemDef" is propagated (CShopItemDef's own constructor is 0x004D405A with
// vtable 0x0123B644); vtable 0x012B4F74 has no RTTI, so the class is named by its vtable like the landed base.
// Shape: the vtable store, `xor eax,eax`, the record addressed once (`lea ecx,[esi+10h]`) and its seven words
// zeroed through ecx by its inline constructor's 28-byte memset (expanded into dword stores through the
// destination register; member-wise initialisers fold the offsets into esi), then +0x2c through esi (an initialiser after the member).
#include <string.h>

struct CVtable_012b5860 {                       // vtable 0x012B5860, 0x10 bytes
    virtual void v00();
    unsigned char _body[0xc];
    CVtable_012b5860();                         // 0x00C3C590
};

struct SZeroRecord {                            // seven words, cleared by the inline constructor's memset
    long m_words[7];
    SZeroRecord() { memset(this, 0, sizeof(*this)); }
};

struct CVtable_012b4f74 : CVtable_012b5860 {    // vtable 0x012B4F74
    SZeroRecord m_record;                       // +0x10
    long        m_value;                        // +0x2c
    virtual void v00();
    CVtable_012b4f74();
};

CVtable_012b4f74::CVtable_012b4f74()
    : m_value(0)
{
}