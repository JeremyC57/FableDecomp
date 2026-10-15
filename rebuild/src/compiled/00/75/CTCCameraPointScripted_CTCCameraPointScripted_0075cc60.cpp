// CTCCameraPointScripted::CTCCameraPointScripted(CThing*) -- CTCCameraPointDefinitionBase(thing), the scripted camera-point vtable, StartPos zeroed, ThingUID / PAT / TransitionTime zeroed and the three "set" flags cleared
// Leaf: ??0CTCCameraPointScripted@@QAE@PAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075CC60, length 66 bytes (+ int3 padding). Manifest label "CTCHeroCentre::
// CTCHeroCentre" is propagated; vtable 0x01269C84 = CTCCameraPointScripted (rtti-vtables.tsv).
// Callees: CTCCameraPointDefinitionBase::CTCCameraPointDefinitionBase(CThing*) @ 0x0075C180 (stores vtable 0x01269B04; manifest
// "CTCOpinionSource" is propagated).
// Shape: Ego_r.pdb layout (0x88 bytes): StartPos +0x40, EndPos +0x4c, LookPos +0x58, LookPosEnd +0x64 (C3DVector, default
// constructor does nothing), ThingUID +0x70 (unsigned __int64), PAT +0x78, TransitionTime +0x7c, LookDirSet / StartPosSet /
// EndPosSet +0x80..+0x82. Only StartPos is explicitly built from (0,0,0) -- the other three vectors are left untouched, which
// is why the zero stores skip +0x4c..+0x6c. One `xor eax,eax` feeds every dword and byte store, all after the vtable store.
// Local model: no engine header exists for CTCCameraPointScripted / CTCCameraPointDefinitionBase.
struct CThing;

struct CTCCameraPointDefinitionBase {                       // vtable 0x01269B04
    virtual void v00();
    unsigned char _base[0x3c];                              // +0x04 .. +0x40 (CTCBase + camera-point definition members)
    CTCCameraPointDefinitionBase(CThing* thing);            // 0x0075C180
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

enum EEditorPointAcquisionType { EDITOR_POINT_ACQUISITION_0 };

struct CTCCameraPointScripted : CTCCameraPointDefinitionBase {   // vtable 0x01269C84
    C3DVector StartPos;                                     // +0x40
    C3DVector EndPos;                                       // +0x4c
    C3DVector LookPos;                                      // +0x58
    C3DVector LookPosEnd;                                   // +0x64
    unsigned __int64 ThingUID;                              // +0x70
    EEditorPointAcquisionType PAT;                          // +0x78
    float TransitionTime;                                   // +0x7c
    bool LookDirSet;                                        // +0x80
    bool StartPosSet;                                       // +0x81
    bool EndPosSet;                                         // +0x82
    virtual void v00();
    CTCCameraPointScripted(CThing* thing);
};

CTCCameraPointScripted::CTCCameraPointScripted(CThing* thing)
    : CTCCameraPointDefinitionBase(thing), StartPos(0.0f, 0.0f, 0.0f), ThingUID(0), PAT(EDITOR_POINT_ACQUISITION_0),
      TransitionTime(0.0f), LookDirSet(false), StartPosSet(false), EndPosSet(false)
{
}