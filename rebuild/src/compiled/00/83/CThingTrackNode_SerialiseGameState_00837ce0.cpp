// CThingTrackNode::SerialiseGameState(CPersistContext*) -- outside load modes 1/3 make sure the village-member and standard-physics components exist (around the CThingPhysical base serialise), then transfer the next/previous node UIDs and thing-file ids, the start/end flags and the track name
// Leaf: ?SerialiseGameState@CThingTrackNode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00837CE0, length 349 bytes (`ret 4`).
// Callees: CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, CCharString(const char*, long) @ 0x0099EBF0,
//   ~CCharString @ 0x0099EAE0, CThingPhysical::SerialiseGameState @ 0x006A5AF0, CPersistContext transfers:
//   Transfer<unsigned __int64> @ 0x0044BB00, Transfer<long> @ 0x00410BE0, Transfer<bool> @ 0x004045C0,
//   Transfer<CCharString> @ 0x00411010 (the manifest's "Transfer<signed_char>" labels are propagated).
// Layout: Ego_r.pdb CThingTrackNode minus 0x18 (the retail CThingPhysical sits 0x18 below the PDB, as the landed
//   CThingSwitch 0x0083C230): NextNodeThingFileID +0xd0, PreviousNodeThingFileID +0xd4 (PDB anonymous unions),
//   NextNodeUID +0xd8, PreviousNodeUID +0xe0, TrackName +0xe8, IsStart +0xec, IsEnd +0xed. CPersistContext::Mode +0x18.
// Shape: the mode is read once per test pair; each AddTC name is an implicit conversion temporary in the dead context
//   slot; the two 64-bit defaults share one 8-byte frame temporary, the long/bool defaults use the dead argument slot.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CTCBase;
class CTCParamsBase;

class CPersistContext { public:
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
    void Transfer(const char* name, long& value, const long& defaultValue);                           // 0x00410BE0
    void Transfer(const char* name, bool& value, const bool& defaultValue);                           // 0x004045C0
    void Transfer(const char* name, CCharString* value);                                              // 0x00411010
};

class CThing { public:
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};

class CThingPhysical : public CThing { public:
    unsigned char _pad00[0xd0];
    bool SerialiseGameState(CPersistContext* context);  // 0x006A5AF0
};

class CThingTrackNode : public CThingPhysical { public:
    long             NextNodeThingFileID;               // +0xd0
    long             PreviousNodeThingFileID;           // +0xd4
    unsigned __int64 NextNodeUID;                       // +0xd8
    unsigned __int64 PreviousNodeUID;                   // +0xe0
    CCharString      TrackName;                         // +0xe8
    bool             IsStart;                           // +0xec
    bool             IsEnd;                             // +0xed
    bool SerialiseGameState(CPersistContext* context);
};

bool CThingTrackNode::SerialiseGameState(CPersistContext* context)
{
    if (context->Mode != 1 && context->Mode != 3)
        AddTC("CTCVillageMember", false, 0);
    if (CThingPhysical::SerialiseGameState(context))
    {
        if (context->Mode != 1 && context->Mode != 3)
            AddTC("CTCPhysicsStandard", false, 0);
        context->Transfer("NextNodeUID", NextNodeUID, 0);
        context->Transfer("PreviousNodeUID", PreviousNodeUID, 0);
        context->Transfer("NextNodeThingFileID", NextNodeThingFileID, 0);
        context->Transfer("PreviousNodeThingFileID", PreviousNodeThingFileID, 0);
        context->Transfer("IsStart", IsStart, false);
        context->Transfer("IsEnd", IsEnd, false);
        context->Transfer("TrackName", &TrackName);
        return true;
    }
    return false;
}