// CEngineLandscapeMemoryManager::EnablePoolAllocation
// void __fastcall (self, bool enable)
class CEngineLandscapeMemoryManager {
public:
    void FreeDisposalList();   // 0x00BD9CE0 (Ego_r.pdb ?FreeDisposalList@CEngineLandscapeMemoryManager@@QAEXXZ)
private: public:
    char pad[0x39];
    bool m_poolEnabled; // +0x39
    void EnablePoolAllocation(bool enable);
};

// external helper at 0xbd9ce0, a this-method (ecx = self) — declared only,
// so the compiler must emit a real rel32 call (no inlining)

void CEngineLandscapeMemoryManager::EnablePoolAllocation(bool enable)
{
    if (enable != m_poolEnabled) {
        ((CEngineLandscapeMemoryManager*)this)->FreeDisposalList();
        m_poolEnabled = enable;
    }
}