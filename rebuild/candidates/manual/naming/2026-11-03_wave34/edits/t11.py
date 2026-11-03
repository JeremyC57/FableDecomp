A="rebuild/src/compiled/00/53/NUISystemCComponent_LoadAndReturnDef_00530ec0.cpp"
B="rebuild/src/compiled/00/76/CTCObjectAugmentations_OnSerialise_00767200.cpp"
EDITS=[
(A,"    template <class T> bool GetDef(long globalIndex, CDefPointer<T>* out);   // 0x0042AEDA",
   "    template <class T> bool GetDef(long globalIndex, CDefPointer<const T>* out) const;   // 0x0042AEDA (Ego_r / FableWin ??$GetDef@VCUIDef@NUISystem@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCUIDef@NUISystem@@@@@Z)"),
(A,"template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<T>* out)",
   "template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<const T>* out) const"),
(A,"    out->Object = (T*)found;","    out->Object = (const T*)found;"),
(B,"""class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
    bool GetDef(const CCharString& name, CDefPointer<const CThingBaseDef>& out);   // 0x00528760
};""","""class CDefinitionManager { public:
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* out) const;   // 0x00528760 <CThingBaseDef> (Ego_r / FableWin ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z)
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
};"""),
(B,"CGameDefinitionManager::Get().GetDef(names[i], Augmentations[i]);","CGameDefinitionManager::Get().GetDef(names[i], &Augmentations[i]);"),
]
