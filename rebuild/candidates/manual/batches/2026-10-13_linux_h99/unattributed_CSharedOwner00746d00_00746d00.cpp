// CSharedOwner_00746d00::~CSharedOwner_00746d00() -- releases the shared block at +0x14 (--refs, virtual destroy at zero, member nulled), then CBase_00686830::~CBase_00686830 by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00746D00, oracle length 35 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+0x14] ; test ecx,ecx ; je T ; dec dword ptr [ecx+4] ; jne N ; mov eax,[ecx] ; call [eax+4] ; N: mov dword ptr [esi+0x14],0 ; T: mov ecx,esi ; pop esi ; jmp CBase_00686830::~CBase_00686830
// Implicit/empty destructor: the shared-pointer member at +0x14 is released inline (its control
// block carries a vtable whose slot 1 destroys it when the reference count at +4 drops to zero,
// and the member is nulled only when it was set), then the base class destructor @ 0x00686830
// runs by tail call. The manifest label (`~C3DAnimationBlendState`) is propagated. One of 12 bodies.
struct SSharedBlock {
    virtual void v00();
    virtual void Destroy();                // slot 1: called when the count reaches zero
    long m_references;                     // +4
};

struct CSharedPointer {
    SSharedBlock* m_pBlock;
    ~CSharedPointer()
    {
        if (m_pBlock) {
            if (--m_pBlock->m_references == 0)
                m_pBlock->Destroy();
            m_pBlock = 0;
        }
    }
};

struct CBase_00686830 {
    unsigned char _body[0x14];
    ~CBase_00686830();                             // 0x00686830
};

struct CSharedOwner_00746d00 : CBase_00686830 {
    CSharedPointer m_pointer;              // +0x14
    ~CSharedOwner_00746d00();
};

CSharedOwner_00746d00::~CSharedOwner_00746d00()
{
}
