// CCountedPointer<COpinionReactionManager>::DeleteData(void*) -- the per-type deleter a counted pointer stores in its count block:
// destroy the object (non-virtual destructor) and free it, null-guarded (`delete (T*)data`).
// Leaf: ?DeleteData@?$CCountedPointer@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004ad600, 23 bytes, static __fastcall (data in ecx), plain `ret`.
// Renamed (rename1, 2026-10-24) from _global::DeleteObject_COpinionReactionManager_004ad600 (symbol OnDie) @ 00/4a/CTCMapwho_OnDie_004ad600.cpp:
//   the PDB (Ego_r) name set has `?DeleteData@?$CCountedPointer@VCOpinionReactionManager@@@@SIXPAX@Z` (public static void __fastcall
//   DeleteData(void*)); the body calls the landed COpinionReactionManager destructor @ 0x004AC800 then operator delete;
//   its only reference is the absolute store `mov [eax+4], 0x004ad600` in the count-block setup of
//   CCountedPointer<COpinionReactionManager>::Reset @ 0x004ADAE0 (PDB name set: ?Reset@?$CCountedPointer@VCOpinionReactionManager@@@@QAEXPAV...@Z).
//   The manifest's CTCMapwho::OnDie is a propagated label.
// Callees: ~COpinionReactionManager @ 0x004AC800 (thiscall), operator delete @ 0x00BFE9BC.
class COpinionReactionManager {
public:
    ~COpinionReactionManager();         // 0x004AC800
};

template <class T> class CCountedPointer {
public:
    static void __fastcall DeleteData(void* data)
    {
        delete (T*)data;
    }
};

template class CCountedPointer<COpinionReactionManager>;
