# T26: 0x0057A5DD is CTCHeroStats::MirroredCriminalStatus::MirroredCriminalStatus() (Ego_r/FableWin ??0MirroredCriminalStatus@CTCHeroStats@@QAE@XZ), cx12 4342
A = "rebuild/src/compiled/00/57/unattributed_CStringOwner0057f616_0057f616.cpp"
B = "rebuild/src/compiled/00/57/unattributed_CClass0057f17a_0057f17a.cpp"
NEST = """class CTCHeroStats { public:
    class MirroredCriminalStatus { public:
        MirroredCriminalStatus();                    // 0x0057A5DD
    };
};"""
EDITS = [
    (A, """struct CMember_0057a5ddMember {
    CMember_0057a5ddMember();                        // 0x0057A5DD
};""", NEST),
    (A, "    CMember_0057a5ddMember m_string;                 // +0x0", "    CTCHeroStats::MirroredCriminalStatus m_0;        // +0x0"),
    (A, "    : m_string()", "    : m_0()"),
    (B, "struct CClass_0057f17a {\n    unsigned char _body[4];\n    void Initialise();                     // 0x0057A5DD\n",
        NEST + "\n\nstruct CClass_0057f17a {\n    CTCHeroStats::MirroredCriminalStatus m_0;   // +0x0 (out-of-line ctor 0x0057A5DD)\n    unsigned char _body[4 - 1];\n"),
    (B, "CClass_0057f17a::CClass_0057f17a(const CAllocator&)\n{\n    Initialise();\n}", "CClass_0057f17a::CClass_0057f17a(const CAllocator&)\n    : m_0()\n{\n}"),
]
