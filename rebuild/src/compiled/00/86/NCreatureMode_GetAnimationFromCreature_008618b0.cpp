// NCreatureMode::GetAnimationFromCreature(const CCharString& name, const CThingCreatureBase& creature) -- the creature's animation entry for `name`, or null when the lookup only finds the "DEFAULT" fallback entry
// Leaf: ?GetAnimationFromCreature@NCreatureMode@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008618B0, length 74 bytes (+6 int3). `__fastcall` free function (name in ecx,
//   creature in edx), plain `ret`; the pointer in eax.
// Mangled (Ego_r.pdb): ?GetAnimationFromCreature@NCreatureMode@@YIPBVCAnimationEntry@@ABVCCharString@@ABVCThingCreatureBase@@@Z.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CThingCreatureBase::GetAnimFromName(const CCharString&, long)
//   const 0x00662A00 (twice), CCharString::~CCharString 0x0099EAE0. Literal "DEFAULT" @ 0x01230554.
// Shape: the "DEFAULT" string is an implicit conversion temporary (re-addressed with `lea` for the call, destroyed right
//   after it, in the `push ecx` slot); the default entry is held in ebx; `if (fallback == anim) return 0; return anim;` is
//   if-converted to the branch-free `xor ecx,ecx ; cmp ebx,eax ; sete cl ; dec ecx ; and eax,ecx` (the FIRST lookup on the
//   left of `==` gives `cmp ebx,eax`; `anim == fallback` gives `cmp eax,ebx`).
class CCharString {                                     // 4-byte string handle (data block pointer)
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CAnimationEntry {                                 // Ego_r.pdb, 16 bytes
public:
    long BankIndex;                                     // +0x00
};
class CMeshDataBank;
class CTCGraphicAppearance;
class CThingCreatureBase {
public:
    const CAnimationEntry& GetAnimFromName(const CCharString& name, long flags) const;   // 0x00662A00
};
namespace NCreatureMode {
const CAnimationEntry* __fastcall GetAnimationFromCreature(const CCharString& name, const CThingCreatureBase& creature);
}

const CAnimationEntry* __fastcall NCreatureMode::GetAnimationFromCreature(const CCharString& name, const CThingCreatureBase& creature)
{
    const CAnimationEntry* fallback = &creature.GetAnimFromName("DEFAULT", 0);
    const CAnimationEntry* anim = &creature.GetAnimFromName(name, 0);
    if (fallback == anim)
        return 0;
    return anim;
}