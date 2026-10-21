// CAIStateGroup_NonVillageGuardPatrol::IsRunnable() -- runnable when a "MoveTrack" track node lies in the 10m square around the creature: latch the nearest one as the patrol node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093CC60, length 336 bytes (+ int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0 (five times), __ftol2 @ 0x00BFEA70, CCharString(const char*, long)
//          @ 0x0099EBF0 ("MoveTrack" @ 0x012925BC) / copy ctor @ 0x0099EC30 / ~CCharString @ 0x0099EAE0,
//          CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPBestThingInRoughArea<
//          CThingFilter_HasTrackName, CThingCompare_Nearest, CThingTrackNode> @ 0x0093CDB0, CBaseIntelligentPointer
//          ctor/SetPItem/GetPItem/dtor (temporary vtable 0x0125A894).
// Globals: 5.0f @ 0x012462A8.
// Shape: the box is a named local whose four truncated bounds evaluate right to left (maxY, maxX, minY, minX) with the
//          creature loaded once; the track name is a named CCharString copied into the filter temporary; arguments
//          right to left (comparator copies the position, then the filter); the hit is latched through a temporary.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04) with the
//          patrol node intelligent pointer at +0x14.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
};
struct CThing {
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CThingTrackNode;
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) { }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CThingFilter_HasTrackName {
    CCharString Name;
    CThingFilter_HasTrackName(const CCharString& name) : Name(name) { }
};
struct CThingCompare_Nearest {
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
struct CThingSearchTools {
    bool GetPBestThingInRoughArea(const C2DBoxI& area, const CThingFilter_HasTrackName& filter,
                                  const CThingCompare_Nearest& compare, CThingTrackNode** out);   // 0x0093CDB0
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the work GetPBestThingInRoughArea does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for CThingSearchTools::GetPBestThingInRoughArea @ 0x0093CDB0 (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) bool CThingSearchTools::GetPBestThingInRoughArea(const C2DBoxI& area, const CThingFilter_HasTrackName& filter, const CThingCompare_Nearest& compare, CThingTrackNode** out)
{
    *out = 0;
    long best = NearSink_s7_0(this, *(const long*)&area + *(const long*)&filter + *(const long*)&compare);
    if (best)
        *(long*)out = best;
    return *out != 0;
}
#pragma auto_inline(on)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerTrackNode : CBaseIntelligentPointer {   // CIntelligentPointer<CThingTrackNode>, vtable 0x0125A894
    virtual void v00();
    __forceinline CIntelligentPointerTrackNode(CThingTrackNode* const& item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
    __forceinline CIntelligentPointerTrackNode& operator=(const CIntelligentPointerTrackNode& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_NonVillageGuardPatrol : CAIStateGroupBase {
    CIntelligentPointerTrackNode PNode;         // +0x14 (descriptive)
    virtual bool IsRunnable();
};

bool CAIStateGroup_NonVillageGuardPatrol::IsRunnable()
{
    C2DBoxI area((long)(GetCreature()->GetPosition().X - 5.0f), (long)(GetCreature()->GetPosition().Y - 5.0f),
                 (long)(GetCreature()->GetPosition().X + 5.0f), (long)(GetCreature()->GetPosition().Y + 5.0f));
    CThingTrackNode* node = 0;
    bool found;
    {
        CCharString track("MoveTrack");
        found = PeekThingSearchTools()->GetPBestThingInRoughArea(area, CThingFilter_HasTrackName(track),
            CThingCompare_Nearest(GetCreature()->GetPosition()), &node);
    }
    if (found) {
        PNode = CIntelligentPointerTrackNode(node);
        return true;
    }
    return false;
}