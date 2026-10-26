// CAIStateGroupInfo::CAIStateGroupInfo(const CAIStateGroupInfo&) -- copy constructor: the name through CCharString's copy constructor, then the allocator pointer; returns this
// Leaf: ??0CAIStateGroupInfo@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FE940, 27 bytes, thiscall `ret 4`.
// Identity (proven): CAIStateGroupInfo::CAIStateGroupInfo(const CAIStateGroupInfo&); evidence: its 203 callers are the AI module's static initialisers (0x0121F86F..) that copy each temporary built by the landed CAIStateGroupInfo::CAIStateGroupInfo(CCharString, AllocFunc) 0x008FD21F into a global record (0x013BB008, +8, ...); the landed constructor's header names 0x008FE940 as the copy constructor; FableWin/Ego_r.pdb list ??0CAIStateGroupInfo@@QAE@ABV0@@Z (Ego_r.pdb CAIStateGroupInfo: StateGroupName +0, AllocFunc +4).
// Label: the manifest's "CComboMultiplierData" is propagated (family H044: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one record type).
// Callee: 0x0099EC30 (the first member's out-of-line copy).
// Shape: `first(o.first), second(o.second)` (or the matching assignments): edi holds the source, esi `this`, the 4-byte
//   second member is one eax move after the call.
class CCharString {
public:
    void* PStringData;                                     // +0x0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};
class CAIStateGroupBase;
typedef CAIStateGroupBase* (__fastcall* TAIStateGroupAllocFunc)();
class CAIStateGroupInfo {                                 // Ego_r.pdb, 8 bytes
public:
    CCharString            StateGroupName;                 // +0x0
    TAIStateGroupAllocFunc AllocFunc;                      // +0x4
    CAIStateGroupInfo(const CAIStateGroupInfo& other);     // PDB ??0CAIStateGroupInfo@@QAE@ABV0@@Z
};
CAIStateGroupInfo::CAIStateGroupInfo(const CAIStateGroupInfo& other) : StateGroupName(other.StateGroupName), AllocFunc(other.AllocFunc) { }
