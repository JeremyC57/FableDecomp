// CSharedOwner_007042a0::~CSharedOwner_007042a0() -- releases the shared block at +0xd0 (--refs, virtual destroy at zero, member nulled), then CThingGameObject::~CThingGameObject by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007042A0, oracle length 41 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+0xd0] ; test ecx,ecx ; je T ; dec dword ptr [ecx+4] ; jne N ; mov eax,[ecx] ; call [eax+4] ; N: mov dword ptr [esi+0xd0],0 ; T: mov ecx,esi ; pop esi ; jmp CThingGameObject::~CThingGameObject
// Implicit/empty destructor: the shared-pointer member at +0xd0 is released inline (its control
// block carries a vtable whose slot 1 destroys it when the reference count at +4 drops to zero,
// and the member is nulled only when it was set), then the base class destructor @ 0x00838A00
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

class CThingGameObject { public:
    unsigned char _body[0xd0];
    ~CThingGameObject();                             // 0x00838A00
};

struct CSharedOwner_007042a0 : CThingGameObject {
    CSharedPointer m_pointer;              // +0xd0
    ~CSharedOwner_007042a0();
};

CSharedOwner_007042a0::~CSharedOwner_007042a0()
{
}