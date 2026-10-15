// CBankFileAsync::SetLoadingMemoryFailureHandler(CCountedPointer<CLoadingMemoryFailureHandler> handler) -- detach this bank from the old memory-failure handler, attach it to the new one and keep the new handler
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D66A0, 136 bytes (+8 int3), __thiscall, ret 8 (the counted pointer by value).
// Callees: CLoadingMemoryFailureHandler member 0x009D5A30 (remove bank; labelled CBankFile::PopLoadErrorQueue) and
// 0x009D6420 (add bank; labelled vector_push_back_4byte_list), both taking the bank; the count block's
// destroy function (`call [blk+4]` with the object in ecx: __fastcall), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CBankFileAsync::MemoryFailureHandler at its retail offset +0x120 (PDB +0x184; the retail
// CBankFile base is smaller): CCountedPointer = {object, count block}, block = {count, destroy, object}.
// Shape: the old handler (if any) forgets the bank, the new one (if any) learns it; then the inline counted-
// pointer assignment (same-block test, release with the member re-read after the decrement, as the landed
// CCountedPointer::operator= 0x009A9BF0 model, then the two stores and the add-ref) and the by-value
// parameter's inline destructor (its block kept in edi).
void operator delete(void* p);                      // 0x00BFE9BC

struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void* object);  // +4
    void* m_pObject;                                // +8
};

template <class T>
class CCountedPointer {
public:
    T* m_pObject;                                   // +0
    SCountBlock* m_pCount;                          // +4
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* object = other.m_pObject;
        SCountBlock* block = other.m_pCount;
        SCountBlock* current = m_pCount;
        if (current != block) {
            if (current) {
                --current->m_references;
                current = m_pCount;
                if (current->m_references == 0) {
                    current->m_pfnDestroy(current->m_pObject);
                    current = m_pCount;
                    delete current;
                }
            }
            m_pObject = object;
            m_pCount = block;
            if (block)
                ++block->m_references;
        }
        return *this;
    }
    T* operator->() const { return m_pObject; }
};

class CBankFileAsync;

class CLoadingMemoryFailureHandler {
public:
    void RemoveBank(CBankFileAsync* bank);          // 0x009D5A30
    void AddBank(CBankFileAsync* bank);             // 0x009D6420
};

class CBankFileAsync {
public:
    void* __vftable;                                // +0x000
    unsigned char _body[0x11c];                     // +0x004 .. +0x11F (CBankFile, ThreadedFile, LoadingMemoryPool)
    CCountedPointer<CLoadingMemoryFailureHandler> MemoryFailureHandler;   // +0x120
    void SetLoadingMemoryFailureHandler(CCountedPointer<CLoadingMemoryFailureHandler> handler);
};

void CBankFileAsync::SetLoadingMemoryFailureHandler(CCountedPointer<CLoadingMemoryFailureHandler> handler)
{
    if (MemoryFailureHandler.m_pObject)
        MemoryFailureHandler->RemoveBank(this);
    if (handler.m_pObject)
        handler->AddBank(this);
    MemoryFailureHandler = handler;
}
