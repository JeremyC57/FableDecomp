// CVtable_012bed90::CVtable_012bed90() -- constructor: the critical-section-owning base (0x00C6E6A0), own vtable, two zeroed 0x18-byte records with pointers to them, a third zeroed record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6D9D0, length 94 bytes (+2 int3 padding).
// Leaf: ??0CVtable_012bed90
// Retail bytes:  push esi ; push edi ; mov esi,ecx ; call CVtable_012beda4::CVtable_012beda4 ; mov dword ptr [esi],012BED90h ;
//                lea eax,[esi+28h] ; xor ecx,ecx ; mov edx,eax ; mov [edx],ecx ; ... ; mov [edx+14h],ecx ; lea ecx,[esi+40h] ;
//                xor edx,edx ; mov edi,ecx ; mov [edi],edx ; ... ; mov [edi+14h],edx ; mov [esi+58h],eax ; xor eax,eax ;
//                mov [esi+5Ch],ecx ; mov [esi+60h],eax ; ... ; mov [esi+74h],eax ; pop edi ; mov eax,esi ; pop esi ; ret
// Manifest label "CResourceBank::CResourceBank" is propagated and wrong: vtable 0x012BED90 is not in rtti-vtables.tsv
// (no-RTTI script/engine module), and the base constructor 0x00C6E6A0 is the landed CVtable_012beda4 one (0x28 bytes:
// vptr, a critical-section owner at +4, a dword +0x20, a byte +0x24). Classes are named by vtable.
// Shape: everything after the vtable store is member construction in declaration order. The first two records clear
// themselves with an intrinsic `memset` of their word array (each expansion takes its own zero register and a copy of
// the record address: `mov edx,eax` / `mov edi,ecx`; the addresses stay live for the two pointer members that follow);
// the third record zeroes its words from an initialiser list (stores straight off esi, one shared zero). An init-list
// zeroing of all three shares a single zero register; memset for all three adds a `lea ecx,[esi+60h]`.
#include <string.h>

struct CVtable_012beda4 {                              // vtable 0x012BEDA4, 0x28 bytes
    virtual void v00();
    unsigned char _body[0x24];                         // +0x04 lock owner, count, flag
    CVtable_012beda4();                                // 0x00C6E6A0
};

struct SRecord_00c6d9d0 {                              // six words, cleared as a block
    long m_words[6];
    SRecord_00c6d9d0() { memset(m_words, 0, sizeof(m_words)); }
};

struct SPlainRecord_00c6d9d0 {                         // six words, zeroed one by one
    long m_words0;
    long m_words1;
    long m_words2;
    long m_words3;
    long m_words4;
    long m_words5;
    SPlainRecord_00c6d9d0() : m_words0(0), m_words1(0), m_words2(0), m_words3(0), m_words4(0), m_words5(0) { }
};

struct CVtable_012bed90 : CVtable_012beda4 {           // vtable 0x012BED90
    SRecord_00c6d9d0  m_first;                         // +0x28
    SRecord_00c6d9d0  m_second;                        // +0x40
    SRecord_00c6d9d0* m_pCurrent;                      // +0x58
    SRecord_00c6d9d0* m_pPrevious;                     // +0x5C
    SPlainRecord_00c6d9d0 m_third;                     // +0x60
    virtual void v00();
    CVtable_012bed90();
};

CVtable_012bed90::CVtable_012bed90()
    : m_first(), m_second(), m_pCurrent(&m_first), m_pPrevious(&m_second), m_third()
{
}