// _Move<CCountedPointer*> -- copy-assign [first, last) onto dest (count-block release/adopt inlined), return the advanced dest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB2A80, oracle length 115 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; push ebx ; mov ebx,ecx ; sub edx,ebx ; sar edx,3 ; test edx,edx ; jle Z ; push ebp ; push esi ; mov esi,[dest] ; push edi ; mov [esp+10h],edx ; L: mov edi,[ebx+4] ; mov eax,[esi+4] ; cmp eax,edi ; mov ebp,[ebx] ; je N ; test eax,eax ; je A ; dec dword ptr [eax] ; mov eax,[esi+4] ; cmp dword ptr [eax],0 ; jne A ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+4] ; push eax ; call operator delete ; add esp,4 ; A: test edi,edi ; mov [esi],ebp ; mov [esi+4],edi ; je N ; inc dword ptr [edi] ; N: mov eax,[esp+10h] ; add ebx,8 ; add esi,8 ; dec eax ; mov [esp+10h],eax ; jne L ; mov eax,esi ; ... ret 0Ch ; Z: mov eax,[dest] ; ... ret 0Ch
// Retail std::copy back end for CCountedPointer elements (`__fastcall`: first in ecx, last in edx,
// dest and two empty tag arguments on the stack, `ret 0Ch`): the signed element count is kept in
// a stack local and counted down; each assignment is the inlined CCountedPointer::operator= --
// when the count blocks differ, release the destination's block (destroy function @ slot +4
// with the object in ecx, flag-tested operator delete @ 0x00BFE9BC at zero), adopt the source's
// object and block, and bump its counter. Returns the advanced destination (the untouched dest
// for an empty range). One of 14 bodies of this shape.
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

struct CCountedPointer {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;   // the source's fields are read first (retail keeps them in edi/ebp across the release)
        void* object = other.m_pObject;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

struct SIteratorTag { SIteratorTag() { } };

CCountedPointer* __fastcall Move_CountedPointer_00ab2a80(CCountedPointer* first, CCountedPointer* last, CCountedPointer* dest, SIteratorTag category, SIteratorTag range)
{
    for (long count = last - first; 0 < count; --count, ++first, ++dest)
        *dest = *first;
    return dest;
}