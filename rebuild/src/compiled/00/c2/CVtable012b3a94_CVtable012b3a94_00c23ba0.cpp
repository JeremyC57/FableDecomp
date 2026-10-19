// CVtable_012b3a94::~CVtable_012b3a94() -- restore the vtable, `delete[]` the plain buffer at +0x14, then `delete[]` the count-prefixed array of 0xb4-byte entries at +0x10 (each entry destroyed through the iterator)
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C23BA0, length 64 bytes.
// Leaf: ??1CVtable_012b3a94
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+14h] ; push eax ; mov dword ptr [esi],offset CVtable_012b3a94::`vftable' ;
//                call operator delete[] ; mov esi,[esi+10h] ; add esp,4 ; test esi,esi ; je R ; mov ecx,[esi-4] ; push edi ;
//                push offset ~SEntry ; lea edi,[esi-4] ; push ecx ; push b4h ; push esi ; call `eh vector destructor iterator' ;
//                push edi ; call operator delete[] ; add esp,4 ; pop edi ; R: pop esi ; ret
// Manifest label "CResource::SetAsLRU" is propagated and wrong: the body is the plain destructor of a polymorphic class
// whose vtable 0x012b3a94 is not in rtti-vtables.tsv (no-RTTI 0x00C2xxxx module), so the class is named by its vtable.
// Callees: operator delete[] @ 0x00BFEB1C, `eh vector destructor iterator' @ 0x00BFEED8 (array, element size 0xb4, the
// count stored before the array, the entry destructor @ 0x00c207f0).
// Shape (as the landed 0x00C48DB0): a virtual function plus a NON-virtual destructor gives the plain `??1`; the
// `eh vector destructor iterator` form of `delete[]` needs /EHsc. The buffer's `delete[]` (a POD array) has no null
// test and its argument is read before the vtable store; the entry array's `delete[]` carries the compiler's null test.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
struct SEntry_00c207f0 {                               // 0xb4 bytes with a destructor @ 0x00c207f0
    unsigned char _body[0xb4];
    ~SEntry_00c207f0();
};

struct CVtable_012b3a94 {                               // vtable 0x012b3a94
    virtual void v00();
    unsigned char  _pad04[0x0c];
    SEntry_00c207f0*  m_pEntries;                      // +0x10
    unsigned char* m_pBuffer;                      // +0x14
    ~CVtable_012b3a94();
};

CVtable_012b3a94::~CVtable_012b3a94()
{
    delete[] m_pBuffer;
    delete[] m_pEntries;
}