D="rebuild/src/compiled/00/"
A=D+"68/global_CParticleMorphsTransferEntry_00681ba0.cpp"
B=D+"6e/CTCTalk_Persist_006e2cb0.cpp"
C=D+"70/CTCQuestCard_OnSerialise_00702720.cpp"
E=D+"72/CTCActionUseScriptedHook_OnSerialise_00729cf0.cpp"
F=D+"83/CThingTrackNode_SerialiseGameState_00837ce0.cpp"
G=D+"83/CThingSwitch_SerialiseGameState_0083c230.cpp"
H=D+"83/CThingPhysicalSwitch_SerialiseGameState_0083d940.cpp"
T2="template <class T> void Transfer(const char* name, T& value);"
T3="template <class T> void Transfer(const char* name, T& value, const T& defaultValue);"
EDITS=[
(A,"void Read(const char*,CCharString&);}","template<class T> void Transfer(const char*,T&);};  // 0x00411010 Transfer<CCharString> (Ego_r / FableWin ??$Transfer@VCCharString@@@...AAVCCharString@@@Z)"),
(A,"ctx->Read(name->Text(),value);","ctx->Transfer(name->Text(),value);"),
(B,"""    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
    void Transfer(const char* name, CCharString* value);                     // 0x00411010""",
"    "+T3+"   // 0x004045C0 <bool>\n    "+T2+"   // 0x00411010 <CCharString>"),
(B,'context->Transfer("DialogueVoice", &DialogueVoice);','context->Transfer("DialogueVoice", DialogueVoice);'),
(C,"""    void Transfer(const char* name, bool& value, const bool& defaultValue);     // 0x004045C0 Transfer<bool>
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x00410BE0 <long>
    void Transfer(const char* name, CCharString* value);                        // 0x00411010""",
"    "+T3+"   // 0x004045C0 <bool> / 0x00410BE0 <long>\n    "+T2+"   // 0x00411010 <CCharString>"),
(E,"""    void Transfer(const char* name, bool& value, const bool& defaultValue);                         // 0x004045C0
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x00410BE0 <long>
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue); // 0x0044BB00
    void Transfer(const char* name, CCharString* value);                                            // 0x00411010""",
"    "+T3+"   // 0x004045C0 <bool> / 0x00410BE0 <long> / 0x0044BB00 <unsigned __int64>\n    "+T2+"   // 0x00411010 <CCharString>"),
(E,'context.Transfer("EntranceConnectedToUID", EntranceConnectedToUID, 0);','context.Transfer<unsigned __int64>("EntranceConnectedToUID", EntranceConnectedToUID, 0);',2),
(E,'context.Transfer("CameraTrackUID", CameraTrackUID, 0);','context.Transfer<unsigned __int64>("CameraTrackUID", CameraTrackUID, 0);',2),
(F,"""    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x00410BE0 <long>
    void Transfer(const char* name, bool& value, const bool& defaultValue);                           // 0x004045C0
    void Transfer(const char* name, CCharString* value);                                              // 0x00411010""",
"    "+T3+"   // 0x0044BB00 <unsigned __int64> / 0x00410BE0 <long> / 0x004045C0 <bool>\n    "+T2+"   // 0x00411010 <CCharString>"),
(F,'context->Transfer("NextNodeUID", NextNodeUID, 0);','context->Transfer<unsigned __int64>("NextNodeUID", NextNodeUID, 0);'),
(F,'context->Transfer("PreviousNodeUID", PreviousNodeUID, 0);','context->Transfer<unsigned __int64>("PreviousNodeUID", PreviousNodeUID, 0);'),
(G,"""    void Transfer(const char* name, float* value, const float& defaultValue);   // 0x00410620
    void Transfer(const char* name, CCharString* value);                        // 0x00411010""",
"    "+T3+"   // 0x00410620 <float>\n    "+T2+"   // 0x00411010 <CCharString>"),
(G,'context->Transfer("TriggerRadius", &TriggerRadius, 0.0f);','context->Transfer("TriggerRadius", TriggerRadius, 0.0f);'),
(G,'context->Transfer("TimeToChangeEnvironmentDef", &TimeToChangeEnvironmentDef, 0.0f);','context->Transfer("TimeToChangeEnvironmentDef", TimeToChangeEnvironmentDef, 0.0f);'),
(H,"    void Transfer(const char* name, CCharString* value); // 0x00411010","    "+T2+"   // 0x00411010 <CCharString>"),
]
for P in (B,C,E,F,G,H):
    EDITS.append((P, '", &', '", ', -1))
