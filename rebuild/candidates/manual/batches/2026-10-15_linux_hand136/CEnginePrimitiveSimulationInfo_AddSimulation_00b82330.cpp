// CEnginePrimitiveSimulationInfo::AddSimulation(unsigned long index, const CCountedPointer<CEngineSimulationBase>& simulation) -- grows the simulation array to cover the index (null-filled) and stores the counted simulation pointer in that slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B82330, length 141 bytes.
// Signature from Ego_r.pdb: ?AddSimulation@CEnginePrimitiveSimulationInfo@@QAEXKABV?$CCountedPointer@VCEngineSimulationBase@@@@@Z
// Callees: 0x00B840C0 the retail vector<CCountedPointer<CEngineSimulationBase>>::resize(size_type, const T&) (the
// manifest's "vector<unsigned int>::resize" label is propagated), the count block's destroy function (+4, object
// in ecx) and 0x00BFE9BC operator delete when a replaced pointer's count drops to zero.
// Layout: the simulation info is a counted pointee (vptr, count) with its 12-byte retail array of 8-byte counted
// pointers at +8 (first) / +0xc (last), as the landed FindSimulation 0x00B812C0 reads it.
// Shape: the new size is `max(index + 1, size())` written as the conditional that re-evaluates size() in its
// arm; resize's fill value is a null CCountedPointer temporary (zeroed after the argument pushes); the slot is
// re-addressed from the reloaded first pointer, and the assignment is the landed CCountedPointer::operator=
// (0x0084F130 family: source block and object read first, release + destroy + delete at zero, adopt, add-ref).

struct SCountBlock {
    long  m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);              // +4
    void* m_pObject;                                     // +8
};
class CEngineSimulationBase;
struct CCountedPointer {                                 // CCountedPointer<CEngineSimulationBase>
    CEngineSimulationBase* m_pObject;                    // +0
    SCountBlock*           m_pCount;                     // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* count = other.m_pCount;
        CEngineSimulationBase* object = other.m_pObject;
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

struct CSimulationArray {                                // retail 12-byte vector: first, last, end
    CCountedPointer* m_pFirst;
    CCountedPointer* m_pLast;
    CCountedPointer* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    void resize(unsigned long newSize, const CCountedPointer& value);   // 0x00B840C0
};

class CEnginePrimitiveSimulationInfo {
public:
    void*            vptr;                               // +0x00
    long             m_refCount;                         // +0x04
    CSimulationArray Simulations;                        // +0x08
    void AddSimulation(unsigned long index, const CCountedPointer& simulation);
};

void CEnginePrimitiveSimulationInfo::AddSimulation(unsigned long index, const CCountedPointer& simulation)
{
    Simulations.resize(Simulations.size() > index + 1 ? Simulations.size() : index + 1, CCountedPointer());
    Simulations.m_pFirst[index] = simulation;
}
