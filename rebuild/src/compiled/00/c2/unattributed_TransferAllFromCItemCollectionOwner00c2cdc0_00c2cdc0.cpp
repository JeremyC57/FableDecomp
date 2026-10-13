// CItemCollectionOwner_00c2cdc0::TransferAllFrom(CItemSource*) -- target(+0x4)->Reserve(src->GetCount() + target->GetCount()), then detach each item from the source and Add it here
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2CDC0, oracle length 100 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push ebp ; push esi ; mov esi,[esp+10h] ; mov edx,[esi] ; mov ebx,ecx ; push edi ; mov edi,[ebx+0x4] ; mov eax,edi ; mov ebp,[eax] ; mov ecx,esi ; call [edx+0x3c] ; mov edx,[edi] ; mov ecx,edi ; mov [esp+14h],eax ; call [edx+0x28] ; mov ecx,[esp+14h] ; add ecx,eax ; push ecx ; mov ecx,[ebx+0x4] ; call [ebp+0x10] ; mov eax,[esi] ; mov ecx,esi ; call [eax+0x2c] ; mov edi,eax ; test edi,edi ; je E ; L: mov edx,[esi] ; push edi ; mov ecx,esi ; call [edx+0x20] ; mov eax,[ebx] ; push edi ; mov ecx,ebx ; call [eax+0x24] ; mov edx,[esi] ; mov ecx,esi ; call [edx+0x2c] ; mov edi,eax ; test edi,edi ; jne L ; E: pop edi ; pop esi ; pop ebp ; pop ebx ; ret 4
// EH-module collection transfer (no frame: nothing to unwind): the target collection at +0x4
// is grown by the source's count plus its own (virtual slots +0x3c / +0x28 / +0x10; the
// target's vtable is read once up front and reused for the reserve call, the object pointer is
// re-read -- the retail TU assumes no aliasing), then every item the source hands out
// (slot +0x2c) is detached from it (slot +0x20) and added here (slot +0x24). The
// manifest label is propagated; the classes are named by role. One of 41 bodies.
struct CItem;

struct CItemSource {
    virtual void _slot00();
    virtual void _slot04();
    virtual void _slot08();
    virtual void _slot0c();
    virtual void _slot10();
    virtual void _slot14();
    virtual void _slot18();
    virtual void _slot1c();
    virtual void Remove(CItem* item);
    virtual void _slot24();
    virtual void _slot28();
    virtual CItem* GetFirst();
    virtual void _slot30();
    virtual void _slot34();
    virtual void _slot38();
    virtual long GetCount();
};

struct CItemTarget {
    virtual void _slot00();
    virtual void _slot04();
    virtual void _slot08();
    virtual void _slot0c();
    virtual void Reserve(long count);
    virtual void _slot14();
    virtual void _slot18();
    virtual void _slot1c();
    virtual void _slot20();
    virtual void _slot24();
    virtual long GetCount();
};

struct CItemCollectionOwner_00c2cdc0 {
    virtual void _slot00();
    virtual void _slot04();
    virtual void _slot08();
    virtual void _slot0c();
    virtual void _slot10();
    virtual void _slot14();
    virtual void _slot18();
    virtual void _slot1c();
    virtual void _slot20();
    virtual void Add(CItem* item);
    CItemTarget* m_pTarget;                // +0x4
    void TransferAllFrom_CItemCollectionOwner_00c2cdc0(CItemSource* source);
};

void CItemCollectionOwner_00c2cdc0::TransferAllFrom_CItemCollectionOwner_00c2cdc0(CItemSource* source)
{
    m_pTarget->Reserve(source->GetCount() + m_pTarget->GetCount());
    for (CItem* item = source->GetFirst(); item; item = source->GetFirst()) {
        source->Remove(item);
        Add(item);
    }
}