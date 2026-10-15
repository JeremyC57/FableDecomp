// CThingManager::SetNonScriptedEntitesPaused(bool paused) -- records the pause mode; when pausing, kills every live thing of type 5 that carries component 119
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051EFC0, length 73 bytes.
// Callees: CThing::Kill(bool) @ 0x004C9B80 (skips Dead / Dying / KillLocked things, then runs the kill virtuals).
// Layout: NonScriptedEntitesPauseMode at +0x54 (PDB +0x68); the thing list is the VC6-era
// Dinkumware std::list<CThing*> whose head node pointer sits at +0x18. CThing: the thing type
// byte at +0x10, UniqueID +0x18, the UsingComponents presence bitset (unsigned long words) at
// +0x20 as in the landed 0x007504E0 GetInterface model -- component 119 is bit 23 of the word at
// +0x2c -- and Dead = bit 0 of the flag byte at +0x91 (PDB +0xa1).
// Shape: the argument is tested before the store (both from al); the bitset probe is an inline
// HasTC() returning the masked word (a bool result narrows to `shr ; test al,1`) that folds to `test dword ptr [ecx+2Ch],800000h`; a null test on each list value.
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x0c];
    unsigned char ThingType;                    // +0x10
    unsigned char _pad11[0x0f];
    unsigned long UsingComponents[9];           // +0x20 (bitset<274>)
    unsigned char _pad44[0x4d];
    unsigned char Dead : 1;                     // +0x91 bit 0
    unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    void Kill(bool immediately);                // 0x004C9B80
};
struct SThingListNode {
    SThingListNode* PNext;                      // +0x00
    SThingListNode* PPrev;                      // +0x04
    CThing*         PThing;                     // +0x08
};
struct CThingList {
    SThingListNode* PHead;
    SThingListNode* begin() const { return PHead->PNext; }
    SThingListNode* end() const { return PHead; }
};
struct CThingManager {
    unsigned char _pad00[0x18];
    CThingList    Things;                       // +0x18
    unsigned char _pad1c[0x38];
    bool          NonScriptedEntitesPauseMode;  // +0x54
    void SetNonScriptedEntitesPaused(bool paused);
};

void CThingManager::SetNonScriptedEntitesPaused(bool paused)
{
    NonScriptedEntitesPauseMode = paused;
    if (paused) {
        for (SThingListNode* it = Things.begin(); it != Things.end(); it = it->PNext) {
            CThing* thing = it->PThing;
            if (thing && !thing->Dead && thing->ThingType == 5 && thing->HasTC(119))
                thing->Kill(true);
        }
    }
}
