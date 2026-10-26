// CThingManager::GetNonLocalThingChecksum() -- sums the checksum (virtual slot 23) of every thing in the thing list that is not Dead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051E530, length 48 bytes.
// Callees: CThing virtual slot 23 (+0x5c), the per-thing checksum.
// Layout: the thing list is a VC6-era Dinkumware std::list<CThing*> whose head-node pointer
// sits at +0x18 (node: +0 next, +4 prev, +8 value); CThing::Dead is bit 0 of the flag byte at
// +0x91 (PDB +0xa1 `uchar:1 Dead`; retail CThing sits 0x10 below the PDB there).
// Shape: the sum lives in ebp (declared first so it takes the last callee-saved register),
// `end()` re-read from the member after each virtual call.
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22();
    virtual unsigned long GetChecksum();         // slot 23 (+0x5c)
    unsigned char _pad04[0x8d];
    unsigned char Dead : 1;                      // +0x91 bit 0
};
struct SThingListNode {
    SThingListNode* PNext;                     // +0x00
    SThingListNode* PPrev;                     // +0x04
    CThing*         PThing;                    // +0x08
};
struct CThingList {                            // VC6 Dinkumware list: allocator word + head node
    SThingListNode* PHead;
    SThingListNode* begin() const { return PHead->PNext; }
    SThingListNode* end() const { return PHead; }
};
class CThingManager { public:
    unsigned char _pad00[0x18];
    CThingList    Things;                      // +0x18
    unsigned long GetNonLocalThingChecksum();
};

unsigned long CThingManager::GetNonLocalThingChecksum()
{
    unsigned long checksum = 0;
    for (SThingListNode* it = Things.begin(); it != Things.end(); it = it->PNext) {
        CThing* thing = it->PThing;
        if (!thing->Dead)
            checksum += thing->GetChecksum();
    }
    return checksum;
}