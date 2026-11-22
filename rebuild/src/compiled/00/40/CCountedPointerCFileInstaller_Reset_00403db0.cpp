// CCountedPointer_00403630::Reset(T* object) -- let go of the current object (deleting it on the last reference) and take shared ownership of a new one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00403DB0, oracle length 103 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Our counted pointer is two words: the object and a 12-byte count block shared by every pointer to that
// object (reference count, the per-type deleter, and the object the deleter is given). Reset() is the
// assignment primitive: if we currently share a block we drop our reference, and when that was the last
// one we run the deleter on the object and free the block. Then we adopt the new object, allocating a
// fresh block with a count of one for it; a null object leaves the pointer empty. If the block
// allocation fails the object is held without a block and will never be deleted by us. This
// instantiation's deleter is the routine at 0x00403630.
//
// Matching: the block pointer is re-read after the decrement (the count is touched through a
// reference); operator new @ 0x00BFEA1A is null-checked, delete @ 0x00BFE9BC; the new block takes the
// object re-read through `this` ([esi]), not the argument. One of 121 bodies of this shape.
void __fastcall Destroy_00403630(void* object);   // the per-type deleter stored in the block

// Count block shared by all pointers to one object.
struct SCountBlock {
    long m_references;                     // how many pointers share the object
    void (__fastcall* m_pfnDestroy)(void*);// deletes the object on the last release
    void* m_pObject;                       // the object handed to the deleter
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_00403630), m_pObject(object) { }
};

struct CCountedPointer_00403630 {
    void* m_pObject;                       // the object we point at
    SCountBlock* m_pCount;                 // its shared count block (null when empty)
    void Reset(void* object);
};

void CCountedPointer_00403630::Reset(void* object)
{
    // Release what we hold now.
    if (m_pCount) {
        --m_pCount->m_references;
        if (m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            delete m_pCount;
        }
    }
    // Adopt the new object with a fresh count of one.
    m_pCount = 0;
    m_pObject = object;
    if (object)
        m_pCount = new SCountBlock(m_pObject);   // re-read through `this`, not the argument
}
