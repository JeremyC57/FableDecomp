// CTCClock::GetHandAnimationUpdateAmount() -- the time-of-day advance since LastTimeUpdated (wrapping through 1.0 when the day rolled over), scaled by the clock def's hand speed: x2/3 for type 2, x8 for type 1, otherwise 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00732730, length 80 bytes.
// Callees: CTCBase::PeekWorld() const @ 0x00686A80 (`mov eax,[g_world] ; mov eax,[eax+24h] ;
// ret`; the landed 0x0077DB90 calls it too), whose PGameTimeManager (+0x1c) holds the normalised time of
// day, Time, at +8 (Ego_r.pdb). Float pool: 1.0f @ 0x0122DED8, 2/3 @ 0x012544AC, 8.0f @ 0x0125916C, 0.0f @ 0x0122DEDC.
// Layout = Ego_r.pdb CTCClock (0x14 bytes): PDef +0xc (CDefPointer<CClockDef const>; the def's hand type
// at +0x34), LastTimeUpdated +0x10.
// Shape: `time > LastTimeUpdated` is `fcom ; test ah,41h ; jne`; the wrapped arm adds
// `(1.0f - LastTimeUpdated)` to the time still on the x87 stack; the def type
// tests are an `==` chain on the type read once (a `switch` lowers to `dec eax ; je`); each arm
// multiplies the delta by a pool constant, the fall-through discards it and loads the 0.0f pool word.
class CGameTimeManager { public:
    unsigned char _pad[8];
    float         Time;                     // +0x08 normalised time of day
};
class CWorld { public:
    unsigned char     _pad[0x1c];
    CGameTimeManager* PGameTimeManager;     // +0x1c scoped_ptr<CGameTimeManager>
};
class CTCBase { protected:
    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};

class CClockDef { public:
    unsigned char _pad[0x34];
    long          HandType;                 // +0x34
};

class CTCClock : public CTCBase { public:
    void*         __vftable;                // +0x00
    unsigned char _pad04[8];                // +0x04 CTCBase
    CClockDef*    PDef;                     // +0x0c CDefPointer<CClockDef const>
    float         LastTimeUpdated;          // +0x10
    float GetHandAnimationUpdateAmount();
};

float CTCClock::GetHandAnimationUpdateAmount()
{
    float time = PeekWorld().PGameTimeManager->Time;
    float delta;
    if (time > LastTimeUpdated)
        delta = time - LastTimeUpdated;
    else
        delta = time + (1.0f - LastTimeUpdated);
    long type = PDef->HandType;
    if (type == 2)
        return delta * 0.6666667f;
    if (type == 1)
        return delta * 8.0f;
    return 0.0f;
}