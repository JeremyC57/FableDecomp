// CTCTargeted::GetNumTargetPoints @ 0x007ea2d0
// long __fastcall GetNumTargetPoints(CTCTargeted* this)

struct Inner {
    char pad[0x64];
    Inner* sub; // +0x64
    // external __fastcall method at 0x4bde90: (this, void*, float) -> long
    long Query(void* p, float f);
};

// Data symbol (NF415, 2026-11-25): the first argument was the bare VA 0x13bac50; it is the const CCharString
//   TARGETING_CURSOR_POSITION_NAME @ 0x013BAC50 (Ego_r.pdb + FableWin.pdb `dynamic initializer for 'TARGETING_CURSOR_POSITION_NAME'';
//   reloc_bindings ?TARGETING_CURSOR_POSITION_NAME@@3VCCharString@@B; the landed 0x007EA490 / 0x008539C0 / 0x00661B40 use it).
class CCharString;
extern const CCharString TARGETING_CURSOR_POSITION_NAME;   // 0x013BAC50

class CTCTargeted { public:
    void* v;      // +0
    Inner* obj;   // +4
};

long __fastcall CTCTargeted_GetNumTargetPoints(CTCTargeted* targeted)
{
    Inner* o = targeted->obj->sub;
    if (o)
        return o->Query((void*)&TARGETING_CURSOR_POSITION_NAME, 1.0f);
    return 0;
}