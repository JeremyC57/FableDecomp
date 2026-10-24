// CTCTavernTable::SetSeatState(long seat, bool used) -- for a seat index inside the seat-state vector: keep NumPeopleSeated in step with a changed state (count up when it becomes used, down when it is freed) and store the new state
// Grade: MATCH (near-miss retry agent901) -- the seat element is indexed twice (`SeatsCurrentlyUsed[seat] != used` ... `SeatsCurrentlyUsed[seat] = used`) instead of through a bool& local, which gives retail's register choice in the size check
// Leaf: ?SetSeatState@CTCTavernTable@@QAEXJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B8530, 57 bytes (+7 int3), thiscall `ret 8`.
// Identity: Ego_r.pdb ?SetSeatState@CTCTavernTable@@QAEXJ_N@Z (public, long + bool); callers 0x00949871, 0x0094B1ED,
//   0x0096CF6A (AI state groups). Catalog label "BitArray_SetBit" is descriptive.
// Layout: retail CTCTavernTable (landed ctor 0x007B8710 / GetFood 0x007B85C0 models; STLport vectors 12 bytes):
//   SeatsCurrentlyUsed +0x34 (a vector<bool> stored one byte per seat), NumPeopleSeated +0x40.
// Shape: `if (SeatsCurrentlyUsed.size() > seat)` (unsigned, `jbe` past the end), STLport operator[] as
//   `*(begin() + n)` re-reading the start pointer, the count change only when the stored state differs, the byte
//   store tail-duplicated into both count arms.
class CTCBase {
public:
    virtual void v00();
    void*         ParentThing;                          // +0x04
    unsigned char _pad08[4];                            // +0x08
};

template <class T> class CRetailVector {                // retail STLport vector (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};

class CTCTavernTable : public CTCBase {
public:
    bool                     BeingServed;               // +0x0c
    CRetailVector<void*>     Occupants;                 // +0x10 vector<CThingAICreature*>
    CRetailVector<void*>     Tankards;                  // +0x1c vector<CIntelligentPointer<CThingObject> > (words)
    CRetailVector<void*>     Food;                      // +0x28
    CRetailVector<bool>      SeatsCurrentlyUsed;        // +0x34
    long                     NumPeopleSeated;           // +0x40
    void SetSeatState(long seat, bool used);
};

void CTCTavernTable::SetSeatState(long seat, bool used)
{
    if (SeatsCurrentlyUsed.size() > (unsigned long)seat) {
        if (SeatsCurrentlyUsed[seat] != used) {
            if (used)
                ++NumPeopleSeated;
            else
                --NumPeopleSeated;
        }
        SeatsCurrentlyUsed[seat] = used;
    }
}