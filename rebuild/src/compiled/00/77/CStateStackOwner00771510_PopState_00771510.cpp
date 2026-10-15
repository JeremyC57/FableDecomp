// CStateStackOwner_00771510::PopState() -- pops the top of a stack of definition pointers and re-applies the new top; false when the stack was already empty
// Leaf: PopState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771510, length 41 bytes.
// Callees: the apply-definition member @ 0x007713C0 (manifest "CTCOpinionSource::SetupFromDef"; `__fastcall`,
// one argument). Called from 0x00700904.
// Label: the manifest's "CTCCameraPointScriptedSpline::EditCycleNextKeyCamera" is propagated and rejected (that
// class's key cameras are 0x38-byte records at +0x68, this is a 4-byte-element vector at +0xc).
// Shape: `if (!empty()) { pop_back(); if (!empty()) Apply(back()); return true; } return false;` -- the
// early `return false` form puts the false exit first and turns the decrement into `add dword ptr
// [ecx+10h],-4`; this nesting keeps `last` in edx for the compare and the `add edx,-4` store, re-reads
// first/last after the store, and places the `xor al,al` tail last.
struct CStateDef;

template <class T> struct CRetailVector {           // retail std::vector: {first, last, end}
    T* first;
    T* last;
    T* end_of_storage;
    bool empty() const { return first == last; }
    void pop_back() { --last; }
    T& back() { return *(last - 1); }
};

struct CStateStackOwner_00771510 {
    unsigned char             _pad[0x0c];
    CRetailVector<CStateDef*> m_states;             // +0x0c
    void SetupFromDef(CStateDef* def);              // 0x007713C0
    bool PopState();
};

bool CStateStackOwner_00771510::PopState()
{
    if (!m_states.empty()) {
        m_states.pop_back();
        if (!m_states.empty())
            SetupFromDef(m_states.back());
        return true;
    }
    return false;
}