// StartTimer(void*, unsigned long) -- copies a default settings block, builds a 28-byte timer descriptor and registers it
// Leaf: StartTimer
// Retail 0x00C1C1A0, 0x64 bytes. NOT CWorldMap::SaveToFile: the manifest label is a propagation (the real
// CWorldMap::SaveToFile is vtable 0x01244AEC slot 4, 0x004FDA60). It is a __fastcall global: its only caller
// (0x00C09701, inside the SEH function at 0x00C096E0) loads ecx = [esi+0x24] and edx = [esi+0x2C] and tests the eax
// result. The body takes no stack arguments (`ret`), stores the ecx argument in the descriptor, ignores edx and
// returns the registration result; and calls library helpers in the 0x00C33xxx block whose manifest labels
// are all wrong: 0x00C33770 (no-arg init of the globals at 0x0143BC18..0x0143BC20), 0x00C33400 (stdcall: copies the
// 0xA0-byte default block at 0x0139C988 with `rep movsd`), 0x00C33470 (stdcall wrapper over 0x00C3E260),
// 0x00C333E0 (stdcall no-op `ret 4`) and 0x00C334D0 (stdcall, two arguments, forwards to the object at 0x0143BB58).
// The class and helper names are descriptive placeholders; the field at +0x60 of the default block is scaled by
// 80/1000 (unsigned `mul 0x10624DD3 ; shr 6`) into the descriptor's period.
struct CDefaultSettings
{
    unsigned long Words[0x18];          // +0x00
    unsigned long Rate;                 // +0x60
    unsigned long More[0x0F];           // +0x64 .. +0xA0
};

struct CTimerDescriptor
{
    unsigned long Field00;              // +0x00
    unsigned long Field04;              // +0x04
    unsigned long Period;               // +0x08
    unsigned long Enabled;              // +0x0C
    unsigned long Field10;              // +0x10
    void*         Owner;                // +0x14
    unsigned long Field18;              // +0x18
};

void InitTimerGlobals();                                               // 0x00C33770
void __stdcall GetDefaultSettings(CDefaultSettings* out);              // 0x00C33400
void __stdcall PrepareDescriptor(CTimerDescriptor* desc);              // 0x00C33470
void __stdcall NoOp(unsigned long value);                              // 0x00C333E0
void* __stdcall RegisterDescriptor(unsigned long flags, CTimerDescriptor* desc); // 0x00C334D0

void* __fastcall StartTimer(void* owner, unsigned long unused)
{
    InitTimerGlobals();
    CDefaultSettings settings;
    GetDefaultSettings(&settings);
    CTimerDescriptor desc;
    desc.Enabled = 1;
    PrepareDescriptor(&desc);
    desc.Period = settings.Rate * 80 / 1000;
    desc.Owner = owner;
    NoOp(0);
    return RegisterDescriptor(0, &desc);
}
