// Retail 0x004159E0. `this` reaches CGameEvent::AddData (0x009F1760) unmodified,
// so CGameEvent derives from CMemoryBuffer at offset 0. The byte argument is appended
// by copying one byte from its own stack slot.
struct CMemoryBuffer {
};

class CGameEvent : public CMemoryBuffer {
public:
    void AddData(long a0, void const*a1);   // 0x009F1760 (Ego_r.pdb ?AddData@CGameEvent@@QAEXJPBX@Z)
private: public:
    void AddUBYTE(unsigned char value);
};

void CGameEvent::AddUBYTE(unsigned char value) {
    ((CGameEvent*)this)->AddData(1, &value);
}