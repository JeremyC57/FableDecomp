// CThingTrackNode::Save(CCharString& out) -- append the track node's level-script text after CThingPhysical's: "LinkedToUID1 <prev uid>;", "LinkedToUID2 <next uid>;" (remapped for serialisation, 0 when unlinked), "Start TRUE|FALSE;", "End TRUE|FALSE;" and "ScriptName <track|NULL>;", each ending "\r\n"
// Grade: DIFFER(697v697) -- same size; only the ScriptName line's temporary slots (retail: NULL temp at local 8, the operator+ result sharing the conditional-temp flag slot 0xc) and the flag-init/out-load order at entry differ
// Leaf: ?Save@CThingTrackNode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00837570, length 697 bytes (virtual, `ret 4`).
// Callees: CThingPhysical::Save 0x006A4BA0 (non-virtual), CThing::IsLevelLess 0x004C7930, CThingManager::RemapUIDForSerialisation
//   0x0051E210 (on the global 0x013B8A1C), CCharString::FromUInt64 0x0099F930 (static __fastcall, hidden result in ecx),
//   operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&, const char*) 0x0099F600,
//   CCharString::operator+= 0x0099F0A0, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   CBasicString<char>::operator==(const char*) 0x004115A0.
// Strings: "LinkedToUID1 " 0x01272E28, "LinkedToUID2 " 0x01272E18, "Start " 0x01272E10, "End " 0x01272E08,
//   "ScriptName " 0x01239BD8, ";\r\n" 0x012353B8, "" 0x0122D70E, "NULL" 0x012393E4, "TRUE" 0x0122D6D4, "FALSE" 0x0122D6CC.
// Layout: CThingTrackNode as the landed OnInitialActivate 0x00837830 (Ego_r.pdb - 0x18): PNextNode +0xd0, PPreviousNode +0xd4,
//   TrackName +0xe8, IsStart +0xec, IsEnd +0xed; CThing UID +0x18 (unsigned __int64), level +0x8e (short), as the
//   landed CTCCameraPointScripted::OnSerialseLeaf 0x0075D320 uses them.
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;        // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);                           // 0x0099EBF0
    ~CCharString();                                                             // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);                          // 0x0099F0A0
    static CCharString __fastcall FromUInt64(unsigned __int64 value);           // 0x0099F930
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
CCharString __fastcall operator+(const char* left, const CCharString& right);       // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);       // 0x0099F600

struct CThingManager {
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210
};
extern CThingManager* g_pThingManager;                      // 0x013B8A1C
inline CThingManager* GetThingManager() { return g_pThingManager; }

#pragma pack(push, 4)
struct CThing {
    unsigned char    _pad04[0x18 - 4];
    unsigned __int64 UID;                                   // +0x18
    unsigned char    _pad20[0x8e - 0x20];
    short            Level;                                 // +0x8e
    unsigned char    _pad90[0xd0 - 0x90];
    virtual void Save(CCharString& out);
    bool IsLevelLess() const;                               // 0x004C7930
    unsigned __int64 GetUID() const { return UID; }
};
struct CThingPhysical : CThing {
    virtual void Save(CCharString& out);                    // 0x006A4BA0
};
struct CThingTrackNode : CThingPhysical {
    CThingTrackNode* PNextNode;                             // +0xd0
    CThingTrackNode* PPreviousNode;                         // +0xd4
    __int64          NextNodeUID;                           // +0xd8
    __int64          PreviousNodeUID;                       // +0xe0
    CCharString      TrackName;                             // +0xe8
    bool             IsStart;                               // +0xec
    bool             IsEnd;                                 // +0xed
    virtual void Save(CCharString& out);
};
#pragma pack(pop)

void CThingTrackNode::Save(CCharString& out)
{
    CThingPhysical::Save(out);
    out += "LinkedToUID1 " + CCharString::FromUInt64(PPreviousNode ? GetThingManager()->RemapUIDForSerialisation(true, IsLevelLess(), Level, PPreviousNode->GetUID()) : 0) + ";\r\n";
    out += "LinkedToUID2 " + CCharString::FromUInt64(PNextNode ? GetThingManager()->RemapUIDForSerialisation(true, IsLevelLess(), Level, PNextNode->GetUID()) : 0) + ";\r\n";
    out += CCharString("") + "Start " + (IsStart ? "TRUE" : "FALSE") + ";\r\n";
    out += CCharString("") + "End " + (IsEnd ? "TRUE" : "FALSE") + ";\r\n";
    out += "ScriptName " + (TrackName == "" ? CCharString("NULL") : TrackName) + ";\r\n";
}
