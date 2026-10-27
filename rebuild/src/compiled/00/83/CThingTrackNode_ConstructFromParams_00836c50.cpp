// CThingTrackNode::ConstructFromParams(long defIndex, const C3DVector& position, CCharString trackName) -- build the game object at the position with no script name, then clear the node links and end flags and adopt the track name
// Leaf: ?ConstructFromParams@CThingTrackNode@@IAE_NJABVC3DVector@@VCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00836C50, 168 bytes, thiscall, `ret 0Ch` (the by-value name destroyed by the callee).
// Identity (proven): Ego_r.pdb `?ConstructFromParams@CThingTrackNode@@IAE_NJABVC3DVector@@VCCharString@@@Z` (3 words); the
//   body writes the CThingTrackNode members (retail layout of the landed SetTrackName 0x00836EE0) and its only caller
//   0x00838413 sits in CThingTrackNode::Create's TU, right after the class's scalar deleting destructor 0x00838500.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / operator= 0x0099EFB0,
//   CThingGameObject::ConstructFromParams(long, const C3DVector&, long, unsigned long, const CCharString&, CThingPhysical*,
//   CThingPhysical*, bool) 0x008388D0 (Ego_r.pdb spelling). String literal 0x0122D70E = "".
// Layout (retail; Ego_r.pdb 0x18 higher): PNextNode +0xd0, PPreviousNode +0xd4, NextNodeUID +0xd8, PreviousNodeUID +0xe0,
//   TrackName +0xe8, IsStart +0xec, IsEnd +0xed.
// Shape: the empty script name is a named local in its own block (built before the argument pushes, destroyed before the
//   test; a temporary is built after them), and the success path is the positive branch (retail falls through to it).
class C3DVector;
class CThingPhysical;
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

class CThingGameObject {                                    // retail 0xd0 bytes
public:
    unsigned char _pad_0x00[0xd0];
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& position, long a3, unsigned long a4, const CCharString& scriptName,
                             CThingPhysical* creator, CThingPhysical* originalCreator, bool a8);   // 0x008388D0
};

class CThingTrackNode : public CThingGameObject {
public:
    CThingTrackNode* PNextNode;                             // +0xd0
    CThingTrackNode* PPreviousNode;                         // +0xd4
    unsigned __int64 NextNodeUID;                           // +0xd8
    unsigned __int64 PreviousNodeUID;                       // +0xe0
    CCharString      TrackName;                             // +0xe8
    bool             IsStart;                               // +0xec
    bool             IsEnd;                                 // +0xed
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& position, CCharString trackName);
};

bool CThingTrackNode::ConstructFromParams(long defIndex, const C3DVector& position, CCharString trackName)
{
    bool constructed;
    {
        CCharString noScriptName("");
        constructed = CThingGameObject::ConstructFromParams(defIndex, position, 0, 0, noScriptName, 0, 0, true);
    }
    if (constructed) {
        PNextNode = 0;
        PPreviousNode = 0;
        NextNodeUID = 0;
        PreviousNodeUID = 0;
        IsStart = false;
        IsEnd = false;
        TrackName = trackName;
        return true;
    }
    return false;
}