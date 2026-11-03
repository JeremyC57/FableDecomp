P="rebuild/src/compiled/00/69/CCountedPointer00694ef0_Reset_00695a00.cpp"
EDITS=[(P,"void __fastcall Destroy_00694ef0(void* object);   // 0x00694EF0: the per-type deleter stored in the block\n",
"""namespace NCreatureAction { class CActionEventBase; }
template <class T> class CCountedPointer { public:
    static void __fastcall DeleteData(void* object);   // 0x00694EF0 for CActionEventBase: the per-type deleter stored in the block
};                                                     // (naming wave 34: Ego_r / FableWin ?DeleteData@?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@SIXPAX@Z)
"""),
(P,"m_pfnDestroy(Destroy_00694ef0)","m_pfnDestroy(CCountedPointer<NCreatureAction::CActionEventBase>::DeleteData)")]
