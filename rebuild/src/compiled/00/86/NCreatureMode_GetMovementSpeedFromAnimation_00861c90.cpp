// NCreatureMode::GetMovementSpeedFromAnimation(const CCharString& name, const CThingCreatureBase& creature, const CMeshDataBank& bank) -- the movement speed of the creature's animation `name`; 0 when it only resolves to the "DEFAULT" fallback
// Leaf: ?GetMovementSpeedFromAnimation@NCreatureMode@@YIMABVCCharString@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00861C90, length 101 bytes (+11 int3). `__fastcall` free function (name in ecx,
//   creature in edx, the bank on the stack, `ret 4`); float in st(0).
// Mangled (Ego_r.pdb): ?GetMovementSpeedFromAnimation@NCreatureMode@@YIMABVCCharString@@ABVCThingCreatureBase@@ABVCMeshDataBank@@@Z.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CThingCreatureBase::GetAnimFromName 0x00662A00 (twice),
//   ~CCharString 0x0099EAE0, the `long` overload GetMovementSpeedFromAnimation 0x00861950. Literal "DEFAULT" @ 0x01230554,
//   0.0f @ 0x0122DEDC.
// Shape: GetAnimationFromCreature (0x008618B0, same TU) and the CAnimationEntry overload are inlined: the fallback select and
//   the null test fold into `cmp ebx,eax ; je zero ; test eax,eax ; jne call` (`if (!anim) return 0.0f;` puts the zero block
//   on the fall-through, `if (anim) return ...;` puts the call there); the entry's BankIndex goes in ecx.
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
float __fastcall GetMovementSpeedFromAnimation(long bankIndex, const CThingCreatureBase& creature, const CMeshDataBank& bank);   // 0x00861950
float __fastcall GetMovementSpeedFromAnimation(const CAnimationEntry& anim, const CThingCreatureBase& creature, const CMeshDataBank& bank);
float __fastcall GetMovementSpeedFromAnimation(const CCharString& name, const CThingCreatureBase& creature, const CMeshDataBank& bank);
}

const CAnimationEntry* __fastcall NCreatureMode::GetAnimationFromCreature(const CCharString& name, const CThingCreatureBase& creature)
{
    const CAnimationEntry* fallback = &creature.GetAnimFromName("DEFAULT", 0);
    const CAnimationEntry* anim = &creature.GetAnimFromName(name, 0);
    if (fallback == anim)
        return 0;
    return anim;
}

inline float __fastcall NCreatureMode::GetMovementSpeedFromAnimation(const CAnimationEntry& anim, const CThingCreatureBase& creature, const CMeshDataBank& bank)
{
    return GetMovementSpeedFromAnimation(anim.BankIndex, creature, bank);
}

float __fastcall NCreatureMode::GetMovementSpeedFromAnimation(const CCharString& name, const CThingCreatureBase& creature, const CMeshDataBank& bank)
{
    const CAnimationEntry* anim = GetAnimationFromCreature(name, creature);
    if (!anim)
        return 0.0f;
    return GetMovementSpeedFromAnimation(*anim, creature, bank);
}