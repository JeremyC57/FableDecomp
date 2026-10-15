// CVtable_012b3aa4::~CVtable_012b3aa4() -- restore the vtable, `delete[]` the plain buffer at +0x14, then `delete[]` the count-prefixed array of 0x0c-byte entries at +0x10 (each entry destroyed through the iterator)
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C24240, length 61 bytes.
// Leaf: ??1CVtable_012b3aa4
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+14h] ; push eax ; mov dword ptr [esi],offset CVtable_012b3aa4::`vftable' ;
//                call operator delete[] ; mov esi,[esi+10h] ; add esp,4 ; test esi,esi ; je R ; mov ecx,[esi-4] ; push edi ;
//                push offset ~SEntry ; lea edi,[esi-4] ; push ecx ; push 0ch ; push esi ; call `eh vector destructor iterator' ;
//                push edi ; call operator delete[] ; add esp,4 ; pop edi ; R: pop esi ; ret
// Manifest label "CResource::SetAsLRU" is propagated and wrong: the body is the plain destructor of a polymorphic class
// whose vtable 0x012b3aa4 is not in rtti-vtables.tsv (no-RTTI 0x00C2xxxx module), so the class is named by its vtable.
// Callees: operator delete[] @ 0x00BFEB1C, `eh vector destructor iterator' @ 0x00BFEED8 (array, element size 0x0c, the
// count stored before the array, the entry destructor @ 0x00c29eb0).
// Shape (as the landed 0x00C48DB0): a virtual function plus a NON-virtual destructor gives the plain `??1`; the
// `eh vector destructor iterator` form of `delete[]` needs /EHsc. The buffer's `delete[]` (a POD array) has no null
// test and its argument is read before the vtable store; the entry array's `delete[]` carries the compiler's null test.
struct SEntry_00c29eb0 {                               // 0x0c bytes with a destructor @ 0x00c29eb0
    unsigned char _body[0x0c];
    ~SEntry_00c29eb0();
};

struct CVtable_012b3aa4 {                               // vtable 0x012b3aa4
    virtual void v00();
    unsigned char  _pad04[0x0c];
    SEntry_00c29eb0*  m_pEntries;                      // +0x10
    unsigned char* m_pBuffer;                      // +0x14
    ~CVtable_012b3aa4();
};

CVtable_012b3aa4::~CVtable_012b3aa4()
{
    delete[] m_pBuffer;
    delete[] m_pEntries;
}
