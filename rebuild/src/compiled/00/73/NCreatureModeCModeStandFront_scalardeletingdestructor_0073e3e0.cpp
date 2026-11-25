#pragma optimize("t", on)
class CModeStandBase {
public:
    virtual ~CModeStandBase();   // 0x0073D860
};
// Naming (NF423 gen, 2026-11-25): the callee 0x00739DF0 now binds its PDB spelling ??1CAnimFilterOf3Groups@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?DestructMember_00739df0@@YIXPAX@Z.
class CAnimFilterOf3Groups { public: virtual ~CAnimFilterOf3Groups(); };
void* __fastcall CModeStandFront_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    reinterpret_cast<CAnimFilterOf3Groups*>(static_cast<unsigned char*>(mode) + 104)->CAnimFilterOf3Groups::~CAnimFilterOf3Groups();
    ((CModeStandBase*)(mode))->CModeStandBase::~CModeStandBase();
    if (flags & 1)
        operator delete(mode);
    return mode;
}