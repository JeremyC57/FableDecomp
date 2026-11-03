P1="rebuild/src/compiled/00/5c/CTCInventoryWeapons_OnSerialise_005c3a95.cpp"
P2="rebuild/src/compiled/00/57/CHeroStatsBlock00578797_Transfer_00578797.cpp"
P3="rebuild/src/compiled/00/40/global_WinMain_00403480.cpp"
EDITS=[
(P1,"""    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
    void Transfer(const char* name, bool* value);       // 0x004107C0
    void Transfer(const char* name, CCharString* value);   // 0x00411010
    void Transfer(const char* name, long* value);       // 0x005CC300
""","""    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x004045C0 <bool>
    template <class T> void Transfer(const char* name, T& value);   // 0x004107C0 <bool> / 0x00411010 <CCharString> / 0x005CC300 <long> (Ego_r ??$Transfer@J@)
"""),
(P1,'DefName", &meleeName);','DefName", meleeName);',0),
(P1,'DefName", &rangedName);','DefName", rangedName);',0),
(P1,'context.Transfer("MeleeWeaponCarried", &MeleeCarried);','context.Transfer("MeleeWeaponCarried", MeleeCarried);'),
(P1,'context.Transfer("RangedWeaponCarried", &RangedCarried);','context.Transfer("RangedWeaponCarried", RangedCarried);'),
(P1,'context.Transfer("ActiveMeleeWeaponDefIndex2", &MeleeDefIndex);','context.Transfer("ActiveMeleeWeaponDefIndex2", MeleeDefIndex);'),
(P1,'context.Transfer("ActiveRangedWeaponDefIndex2", &RangedDefIndex);','context.Transfer("ActiveRangedWeaponDefIndex2", RangedDefIndex);'),
(P2,"""class CPersistContext { public:
    void TransferBool(const char* name, bool* value);                 // 0x004107C0 (Transfer<bool>)
    void TransferSignedChar(const char* name, signed char* value);    // 0x0049B8D0 (Transfer<signed char>)
};
""","""class CCharString;
namespace std {
template <class T> class allocator;
template <class T, class A> class vector;
}
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value);   // 0x004107C0 <bool> (Ego_r / FableWin ??$Transfer@_N@...AA_N@Z)
    template <class T> void TransferVector(const char* name, std::vector<T, std::allocator<T> >& value);   // 0x0049B8D0 <CCharString> (Ego_r / FableWin ??$TransferVector@VCCharString@@@)
};
"""),
(P2,"""    signed char   m_tutorialsPlayed;               // +0x248
""","""    std::vector<CCharString, std::allocator<CCharString> > m_tutorialsPlayed;   // +0x248 (Ego_r CTCInventoryExperience::TutorialsPlayed, vector<CCharString>)
"""),
(P2,"""    context.TransferBool("OpenedExperienceSpending", &m_openedExperienceSpending);
    context.TransferSignedChar("TutorialsPlayed", &m_tutorialsPlayed);""","""    context.Transfer("OpenedExperienceSpending", m_openedExperienceSpending);
    context.TransferVector("TutorialsPlayed", m_tutorialsPlayed);"""),
(P3,'extern "C" long __fastcall GFMain(HINSTANCE instance, char* commandLine, int showCommand);   // 0x00402510',
    'long __fastcall GFMain(HINSTANCE instance, char* commandLine, int showCommand);   // 0x00402510 (Ego_r / FableWin ?GFMain@@YIJPAUHINSTANCE__@@PADH@Z)'),
]
