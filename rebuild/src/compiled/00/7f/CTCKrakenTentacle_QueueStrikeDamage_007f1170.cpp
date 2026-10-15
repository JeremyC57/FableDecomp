// CTCKrakenTentacle::QueueStrikeDamage() -- while the kraken is alive, queue the strike: QueuedStrikeTick = current frame + (long)(def strike delay in seconds * frames per second)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F1170, length 84 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), the global frame counter getter @ 0x0049D870,
// __ftol2 @ 0x00BFEA70. Global: the frames-per-second long @ 0x01375550 (value 15).
// Layout (local model, no engine header): PTentacleDef +0x0c (def pointer as the first word), PKraken +0x10
// (CIntelligentPointer<CThing>), QueuedStrikeTick +0x2c (PDB offsets); the kraken thing's Dead bit is bit 0 of +0x91 (retail
// CThing: PDB +0xa1 minus 16); the def's strike delay at +0x40 (name descriptive).
// Shape: GetPItem is called for the null test, for the Dead test and once more with the result unused (the kraken fetched
// into a local that the body never reads -- the out-of-line call survives); the global and the def pointer are named
// locals read BEFORE the frame call (the int is parked in the `push ecx` frame slot for `fild`, the def in edi), the frame
// number rides in ebx across `__ftol2`.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* Get() const { return (T*)GetPItem(); }
};

struct CThing {
    unsigned char _pad_0x00[0x91];
    unsigned char Dead : 1;                         // +0x91 bit 0
};

struct CKrakenTentacleDef {
    unsigned char _pad_0x00[0x40];
    float         StrikeDelaySeconds;               // +0x40
};

extern long g_framesPerSecond;                      // 0x01375550
long GetCurrentFrame();                             // 0x0049D870

struct CTCKrakenTentacle {
    unsigned char               _pad_0x00[0x0c];
    const CKrakenTentacleDef*   PTentacleDef;       // +0x0c
    CIntelligentPointer<CThing> PKraken;            // +0x10
    unsigned char               _pad_0x18[0x2c - 0x18];
    long                        QueuedStrikeTick;   // +0x2c
    void QueueStrikeDamage();
};

void CTCKrakenTentacle::QueueStrikeDamage()
{
    if (PKraken.Get() && !PKraken.Get()->Dead) {
        CThing* kraken = PKraken.Get();
        long framesPerSecond = g_framesPerSecond;
        const CKrakenTentacleDef* def = PTentacleDef;
        QueuedStrikeTick = GetCurrentFrame() + (long)(framesPerSecond * def->StrikeDelaySeconds);
    }
}