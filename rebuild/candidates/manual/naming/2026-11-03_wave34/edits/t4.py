D="rebuild/src/compiled/00/"
import glob
def F(a): return glob.glob("rebuild/src/compiled/"+a[0:2]+"/"+a[2:4]+"/*_"+a+".cpp")[0]
EDITS=[
(F("004c97b0"),"    void OnActivateEntity(CThing* thing);        // 0x004AFA60","    void OnActivateEntity(CThing& thing);        // 0x004AFA60 (Ego_r / FableWin ?OnActivateEntity@CQuestManager@@QAEXAAVCThing@@@Z)"),
(F("004c97b0"),"Get().OnActivateEntity(this);","Get().OnActivateEntity(*this);"),
(F("0051e5a0"),"    void OnActivateEntity(CThing* thing);       // 0x004AFA60","    void OnActivateEntity(CThing& thing);       // 0x004AFA60 (Ego_r / FableWin ?OnActivateEntity@CQuestManager@@QAEXAAVCThing@@@Z)"),
(F("0051e5a0"),"Get().OnActivateEntity(thing);","Get().OnActivateEntity(*thing);"),
(F("0089b780"),"struct CTeleportThing {\n","class CThing { public:                             // the thing view the teleport code calls (SetAsGameStatePersisted / SetInLimbo are CThing members, Ego_r / FableWin)\n"),
(F("0089b780"),"#define T(x) ((CTeleportThing*)(x))","#define T(x) ((CThing*)(x))"),
(F("009cd330"),"struct CDiskFileCalls {","class CAFile { public:                               // the bank's file object (CCountedPointer<CDiskFileWin32>, a CAFile)"),
(F("009cd330"),"""    void* m_pVtable;                                // +0x00
    void* m_name;                                   // +0x04 CCharString
    CDiskFileCalls* m_pFile;                        // +0x08
    CFileDataOutputStream(CDiskFileCalls* file);    // 0x00994400
    ~CFileDataOutputStream();                       // 0x00994430""","""    void* m_name;                                   // +0x04 CCharString
    CAFile* m_pFile;                                // +0x08
    CFileDataOutputStream(CAFile& file);            // 0x00994400 (Ego_r / FableWin ??0CFileDataOutputStream@@QAE@AAVCAFile@@@Z)
    virtual ~CFileDataOutputStream();               // 0x00994430 (Ego_r / FableWin ??1CFileDataOutputStream@@UAE@XZ)"""),
(F("009cd330"),"    CDiskFileCalls*           BankFile_Data;        // +0x7c CCountedPointer<CDiskFileWin32>::Data","    CAFile*                   BankFile_Data;        // +0x7c CCountedPointer<CDiskFileWin32>::Data"),
(F("009cd330"),"    CFileDataOutputStream out(BankFile_Data);","    CFileDataOutputStream out(*BankFile_Data);"),
(F("008006d0"),"    const char* ToConstChar() const;                // 0x0099E4C0","    operator const char*() const;                   // 0x0099E4C0 (Ego_r / FableWin ??BCCharString@@QBEPBDXZ)"),
(F("008006d0"),'("NavPosition" + GFIntToCharString(i)).ToConstChar()','(const char*)("NavPosition" + GFIntToCharString(i))'),
(F("008006d0"),'("NavLayer" + GFIntToCharString(i)).ToConstChar()','(const char*)("NavLayer" + GFIntToCharString(i))'),
(F("00def760"),"""struct CQ_SecretPassageScript : CScriptBase {
    unsigned char _pad40[0x4c - 0x40];
    bool DoorsBlocked;                                 // +0x4C
    bool DoorsBeingDone;                               // +0x4D
    void SwitchDoorsOnAndOff();                        // 0x00DEFB10
};""","""namespace NScript {
class CQ_SecretPassageScript : public CScriptBase { public:   // NScript script class (both PDBs: class, namespace NScript)
    unsigned char _pad40[0x4c - 0x40];
    bool DoorsBlocked;                                 // +0x4C
    bool DoorsBeingDone;                               // +0x4D
    void SwitchDoorsOnAndOff();                        // 0x00DEFB10
};
}
using NScript::CQ_SecretPassageScript;"""),
(F("00e06050"),"enum ETraderComment { TRADER_COMMENT_NONE };\n",""),
(F("00e06050"),"    bool MakeTraderComment(const CCharString& comment, const CScriptThing& trader, ETraderComment type);   // 0x00E01900",
 "    enum ETraderComment { TRADER_COMMENT_NONE };       // nested in the script class (Ego_r / FableWin W4ETraderComment@CQ_TraderEscortScript@NScript@@)\n    bool MakeTraderComment(const CCharString& comment, const CScriptThing& trader, ETraderComment type);   // 0x00E01900"),
]
for a, extra in (("00e5f2b0", None), ("00e5f8f0", None)):
    EDITS.append((F(a),"""typedef long EHENCHMAN_COMMENT;
struct CV_BodyGuardScript {
 bool MakeHenchmanComment(const CCharString& text, long priority, EHENCHMAN_COMMENT type, const CScriptThing& target);   // 0x00E5EBD0
};""","""namespace NScript {
class CV_BodyGuardScript { public:                     // NScript script class (both PDBs)
 enum EHENCHMAN_COMMENT { };                           // nested comment kind (Ego_r / FableWin W4EHENCHMAN_COMMENT@CV_BodyGuardScript@NScript@@)
 bool MakeHenchmanComment(const CCharString& text, long priority, EHENCHMAN_COMMENT type, const CScriptThing& target);   // 0x00E5EBD0
};
}
using NScript::CV_BodyGuardScript;"""))
    EDITS.append((F(a),"MakeHenchmanComment(Me.GetDataString(), 2, 2, CScriptThing())","MakeHenchmanComment(Me.GetDataString(), 2, (CV_BodyGuardScript::EHENCHMAN_COMMENT)2, CScriptThing())"))
EDITS += [
(F("00e5f6b0"),"""enum EHENCHMAN_COMMENT { HENCHMAN_COMMENT_REGION_ENTRANCE = 2 };
struct CV_BodyGuardScript {
    bool MakeHenchmanComment(const CCharString& text, long priority, EHENCHMAN_COMMENT comment, const CScriptThing& about = CScriptThing());   // 0x00E5EBD0
};""","""namespace NScript {
class CV_BodyGuardScript { public:                     // NScript script class (both PDBs)
    enum EHENCHMAN_COMMENT { HENCHMAN_COMMENT_REGION_ENTRANCE = 2 };   // nested (Ego_r / FableWin W4EHENCHMAN_COMMENT@CV_BodyGuardScript@NScript@@); enumerator name descriptive
    bool MakeHenchmanComment(const CCharString& text, long priority, EHENCHMAN_COMMENT comment, const CScriptThing& about = CScriptThing());   // 0x00E5EBD0
};
}
using NScript::CV_BodyGuardScript;"""),
(F("00e5f6b0"),"MakeHenchmanComment(Me.GetDataString(), 2, HENCHMAN_COMMENT_REGION_ENTRANCE)","MakeHenchmanComment(Me.GetDataString(), 2, CV_BodyGuardScript::HENCHMAN_COMMENT_REGION_ENTRANCE)"),
(F("00e82950"),"struct CV_FisticuffsClubScript {\n","namespace NScript {\nclass CV_FisticuffsClubScript { public:               // NScript script class (both PDBs)\n"),
(F("00e82950"),"""    CCharString GetQuestText(CCharString suffix);       // 0x00E82F80
};""","""    CCharString GetQuestText(CCharString suffix);       // 0x00E82F80
};
}
using NScript::CV_FisticuffsClubScript;"""),
]
for a in ("00d37530","00daa260","00e7e640"):
    EDITS.append((F(a),"    CScriptThing GetScriptThing() const;                // 0x007E7490","    virtual CScriptThing GetScriptThing() const;        // 0x007E7490 (Ego_r / FableWin UBE: virtual, called directly on the local)"))
    EDITS.append((F(a),"    bool IsPerformingScriptTask() const;                // 0x007E7450","    virtual bool IsPerformingScriptTask() const;        // 0x007E7450 (Ego_r / FableWin UBE)"))
