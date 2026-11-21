#ifndef FABLE_ENGINE_NCAMERAMODES_CSCRIPTMODULEFOVZOOM_H
#define FABLE_ENGINE_NCAMERAMODES_CSCRIPTMODULEFOVZOOM_H
// NCameraModes::CScriptModuleFOVZoom and CScriptModuleFOVZoomStayRelative (p25Q, new header; row 0x00852E10, the StayRelative ctor).
// CScriptModuleFOVZoom: Ego_d.pdb LF_CLASS 0x3C12F (field list 0x3C12E: public base CScriptModuleBase, sizeof 132) and Ego_r.pdb
// struct_layouts_egor.tsv (0x84; the landed ctor 0x00852560 / dtor 0x00854380, vtable 0x01275A1C): LookAtPos +0x4C, EndFrame +0x58,
// WorldFrames +0x5C (protected), FOVIncrement +0x60, FOVFraction +0x64, Increment +0x68, StartPos +0x74, ShortenOnOcclusion +0x80,
// ResetCamera +0x81 (private). Methods (Ego_d field list): the ctor (const C3DVector& x3, float, long, bool), Update, IsShortenOnOcclusion,
// ResetCameraOnEnd (virtual overrides), SetResetCameraOnEnd(bool), OnInitLeaf (protected virtual), CalculateIncrementVector (protected,
// landed 0x008520F0), the compiler-generated virtual dtor.
// CScriptModuleFOVZoomStayRelative: Ego_d.pdb LF_CLASS 0x570C2 (field list 0x570C1: public base CScriptModuleFOVZoom, sizeof 160) and
// Ego_r.pdb (0xa0; vtable 0x01275A94): Thing +0x84 (CIntelligentPointer<CThing>), Axis +0x8C, Angle +0x98, Mag +0x9C (private). Methods:
// the ctor (const CIntelligentPointer<CThing> by value, const C3DVector& x3, float, long, bool; row 0x00852E10), Update (landed
// 0x00852FD0), the compiler-generated virtual dtor. Declarations only; parameter names are Ego_d.pdb's. Vtable slots as the base header
// (retail 0x01275A94: 0 0x008544C0, 1 Update 0x00852FD0, 3 0x00854360, 7 0x00854370, 8 0x008520C0; slots 2, 4, 5, 6 the base's).
#include "engine/NCameraModes_CScriptModuleBase.h"
namespace NCameraModes {
class CScriptModuleFOVZoom : public CScriptModuleBase {
public:
    CScriptModuleFOVZoom(const C3DVector& look_at_pos, const C3DVector& start_pos, const C3DVector& end_pos,
                         float fraction_of_fov_to_reach, long world_frames, bool shortenonocclusion);   // 0x00852560
    virtual void Update();                                        // 0x00852160
    virtual bool IsShortenOnOcclusion() const;                    // 0x00854360
    virtual bool ResetCameraOnEnd() const;                        // 0x00854370
    void SetResetCameraOnEnd(bool);                             // no Ego_d record (parameter name unknown)
protected:
    virtual void OnInitLeaf();                                    // 0x008520C0
    void CalculateIncrementVector(const C3DVector& s, const C3DVector& e, long nf);   // 0x008520F0
    C3DVector LookAtPos;                                          // +0x4C
    long      EndFrame;                                           // +0x58
    long      WorldFrames;                                        // +0x5C
private:
    float     FOVIncrement;                                       // +0x60
    float     FOVFraction;                                        // +0x64
    C3DVector Increment;                                          // +0x68
    C3DVector StartPos;                                           // +0x74
    bool      ShortenOnOcclusion;                                 // +0x80
    bool      ResetCamera;                                        // +0x81
    friend struct CScriptModuleFOVZoomLayout;
};
struct CScriptModuleFOVZoomLayout {
    FABLE_STATIC_ASSERT(sizeof(CScriptModuleFOVZoom) == 0x84);
};
class CScriptModuleFOVZoomStayRelative : public CScriptModuleFOVZoom {
public:
    CScriptModuleFOVZoomStayRelative(const CIntelligentPointer<CThing> thing, const C3DVector& look_at_pos, const C3DVector& start_pos,
                                     const C3DVector& end_pos, float fraction_of_fov_to_reach, long world_frames,
                                     bool shortenonocclusion);    // 0x00852E10
    virtual void Update();                                        // 0x00852FD0
private:
    CIntelligentPointer<CThing> Thing;                            // +0x84
    C3DVector                   Axis;                             // +0x8C
    float                       Angle;                            // +0x98
    float                       Mag;                              // +0x9C
    friend struct CScriptModuleFOVZoomStayRelativeLayout;
};
struct CScriptModuleFOVZoomStayRelativeLayout {
    FABLE_STATIC_ASSERT(sizeof(CScriptModuleFOVZoomStayRelative) == 0xa0);
};
}
#endif
