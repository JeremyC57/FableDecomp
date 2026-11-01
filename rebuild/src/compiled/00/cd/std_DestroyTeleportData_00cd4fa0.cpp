// std::_Destroy<TeleportData>(NScript::CQ_BountyHuntScript::TeleportData* __pointer) -- STLport single-element destroy: run the implicit ~TeleportData in place
// Leaf: ??$_Destroy@UTeleportData@CQ_BountyHuntScript@NScript@@@std@@YIXPAUTeleportData@CQ_BountyHuntScript@NScript@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD4FA0, 35 bytes, __fastcall (pointer in ecx): the inlined implicit ~TeleportData --
//   ~CCharString(+0x14), ~CCharString(+0x10), ~CCharString(+0x0C), `jmp ~CCharString` on +0x00.
// Identity: no call site (retail keeps unreferenced COMDATs; cf. chunk 655 0x00529CC0). Placement proves it: Ego_r.pdb emits,
//   in the script-manager TU, ??$_Dest_val<allocator<TeleportData@CQ_BountyHuntScript>,TeleportData> (donor 0x00CC0E30)
//   immediately followed by ??$_Dest_val<allocator<CDataMarkerOutput::CElement>,CElement> (0x00CC0EA0); retail has
//   0x00CD4FA0 (this body = the inlined ~TeleportData, see 0x00CD4550) immediately followed by 0x00CD4FD0, whose body is
//   the inlined ~CDataMarkerOutput::CElement (~CCharString +4, ~CCharString +0; element proven by the landed
//   __uninitialized_copy<CElement*> 0x00CD4E60 / GetStringFromDataMarker 0x00CD4FF0 in the same TU). The Dinkumware
//   _Dest_val(alloc&, T*) of Ego_r is STLport 4.6's `_Destroy(_Tp* __pointer)` (stl/_construct.h), one pointer argument,
//   __fastcall in this /Gr TU (the neighbouring STLport helpers 0x00CD4E60 / 0x00CD4EA0 are __fastcall too).
//   TeleportData's layout is the Ego_r.pdb one (24 bytes); the only PDB struct with CCharStrings at 0/0xC/0x10/0x14.
// Callee: CCharString::~CCharString 0x0099EAE0 (??1CCharString@@QAE@XZ).
class CCharStringData;
class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
    CCharStringData* PStringData;
};

namespace NScript {
class CQ_BountyHuntScript {
public:
    struct TeleportData {                               // 24 bytes (Ego_r.pdb)
        CCharString   defName;                          // +0x00
        unsigned long appearanceSeed;                   // +0x04
        float         health;                           // +0x08
        CCharString   scriptName;                       // +0x0c
        CCharString   teleportTo;                       // +0x10
        CCharString   teleportMkr;                      // +0x14
    };
};
}

namespace std {
// STLport 4.6 stl/_construct.h
template <class _Tp>
inline void _Destroy(_Tp* __pointer) {
    __pointer;
    __pointer->~_Tp();
}
template void _Destroy<NScript::CQ_BountyHuntScript::TeleportData>(NScript::CQ_BountyHuntScript::TeleportData*);
}