// Behaviour test for 0x007B8530 CTCTavernTable::SetSeatState(long seat, bool used): in-range seats store the new state
// and move NumPeopleSeated by one only when the state changes; out-of-range seats (including negative) do nothing.
#include <stdio.h>
#include <string.h>
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

void CTCBase::v00() { }

int main()
{
    int ok = 1;
    bool seats[4] = { false, false, true, false };
    CTCTavernTable t;
    memset(&t.Occupants, 0, sizeof(t.Occupants)); memset(&t.Tankards, 0, sizeof(t.Tankards)); memset(&t.Food, 0, sizeof(t.Food));
    t.SeatsCurrentlyUsed._M_start = seats; t.SeatsCurrentlyUsed._M_finish = seats + 4; t.SeatsCurrentlyUsed._M_end_of_storage = seats + 4;
    t.NumPeopleSeated = 1;
    t.SetSeatState(0, true);   if (!seats[0] || t.NumPeopleSeated != 2) ok = 0;
    t.SetSeatState(0, true);   if (!seats[0] || t.NumPeopleSeated != 2) ok = 0;   // unchanged
    t.SetSeatState(2, false);  if (seats[2] || t.NumPeopleSeated != 1) ok = 0;
    t.SetSeatState(3, false);  if (seats[3] || t.NumPeopleSeated != 1) ok = 0;    // unchanged
    t.SetSeatState(4, true);   if (t.NumPeopleSeated != 1) ok = 0;                // out of range
    t.SetSeatState(-1, true);  if (t.NumPeopleSeated != 1) ok = 0;                // negative: huge unsigned
    printf("007b8530_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}