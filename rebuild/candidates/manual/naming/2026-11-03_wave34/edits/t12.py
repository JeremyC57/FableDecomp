A="rebuild/src/compiled/00/59/CFrontEndManager_Update_00599e3f.cpp"
B="rebuild/src/compiled/00/42/global_InstallationComplete_0042e3ce.cpp"
EDITS=[
(A,"""class CFileInstaller { public:
    void Unpause();                                        // 0x009C3250
    static CFileInstaller* __fastcall Get();               // 0x00404440 (folded CTBaseSingleton<>::Get)
};""","""class CFileInstaller { public:
    void Unpause();                                        // 0x009C3250
};
template <class T> class CTBaseSingleton { public:
    static T& __fastcall Get();                            // 0x00404440 for CFileInstaller (Ego_r / FableWin ?Get@?$CTBaseSingleton@VCFileInstaller@@@@SIAAVCFileInstaller@@XZ)
};"""),
(A,"CFileInstaller::Get()->Unpause();","CTBaseSingleton<CFileInstaller>::Get().Unpause();",3),
(B,"""class CFileInstallerSingleton {
public:
    static CFileInstaller& __fastcall Get();             // 0x00404440
    static bool Exists() { return Instance.Data != 0; }
    static CCountedPointer<CFileInstaller> Instance;     // 0x013CA818
};""","""template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x00404440 for CFileInstaller (Ego_r / FableWin ?Get@?$CTBaseSingleton@VCFileInstaller@@@@SIAAVCFileInstaller@@XZ)
    static bool Exists() { return Instance.Data != 0; }
    static CCountedPointer<T> Instance;                  // 0x013CA818 (spelt as the landed 0x0098E320 / 0x00404440)
};"""),
(B,"if (CFileInstallerSingleton::Exists() && !CFileInstallerSingleton::Get().IsComplete())","if (CTBaseSingleton<CFileInstaller>::Exists() && !CTBaseSingleton<CFileInstaller>::Get().IsComplete())"),
]
