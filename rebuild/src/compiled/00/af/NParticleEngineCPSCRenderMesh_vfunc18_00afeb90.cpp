// Original method recovered from Ego_r and FableWin PDBs; see advisor/NAMES.md.
// 00AFEB90 NParticleEngine::CPSCRenderMesh::DatabaseCopy, 620 retail bytes.
// Exact RTTI-vtable evidence identifies slot 18. The generated header is absent;
// this model follows PDB CPSCRenderMesh's 100-byte field layout and packed unsigned
// bit units. Assignment order was transcribed from the retail field loads.
namespace NParticleEngine {
class CParticleSystem;
class CPSCBase { public:
    virtual void v000() {}
    CParticleSystem* ParticleSystem;
    bool UpdateStarted : 1;
    bool Initialised : 1;
};
struct CRGBColour { unsigned int Colour; };
class CPSCRenderMesh : public CPSCBase { public:
    unsigned int BankIndex;
    unsigned int TrailBankIndex;
    CRGBColour TrailStartColour, TrailMidColour, TrailEndColour;
    CRGBColour StartColour, MidColour, EndColour;
    unsigned int BlendMode : 3;
    unsigned int TrailBlendMode : 3;
    unsigned int BlendOp : 3;
    unsigned int TrailBlendOp : 3;
    unsigned int FadeInEndInteger : 10;
    unsigned int FadeOutBeginInteger : 10;
    unsigned int TrailLengthInteger : 12;
    unsigned int FlickerMinAlphaInteger : 10;
    unsigned int FlickerMinSizeInteger : 10;
    unsigned int StartRenderSizeX : 11;
    unsigned int StartRenderSizeY : 11;
    unsigned int StartRenderSizeZ : 11;
    unsigned int CentredOnPos : 1;
    unsigned int AlphaFadeEnable : 1;
    unsigned int EndRenderSizeX : 11;
    unsigned int EndRenderSizeY : 11;
    unsigned int EndRenderSizeZ : 11;
    unsigned int SizeFadeEnable : 1;
    unsigned int FlickerEnable : 1;
    unsigned int TrailUseStartColour : 1;
    unsigned int TrailUseMidColour : 1;
    unsigned int TrailUseEndColour : 1;
    unsigned int UseRenderSizeParam : 1;
    unsigned int FlickerBias : 8;
    unsigned int FlickerSpeed : 12;
    unsigned int AlphaFadeMinimum : 7;
    unsigned int UseStartColour : 1;
    unsigned int SizeFadeMinimum : 7;
    unsigned int UseMidColour : 1;
    unsigned int TrailWidth : 10;
    unsigned int UseEndColour : 1;
    unsigned int RenderSizeParam;
    unsigned int PrimHandleData;
    unsigned int PrimHandleInfo;
    long ChildPrimIndex;
    unsigned int TrailPrimHandleData;
    unsigned int TrailPrimHandleInfo;
    long ChildTrailPrimIndex;
    void DatabaseCopy(const CPSCBase& other);
};
void CPSCRenderMesh::DatabaseCopy(const CPSCBase& other)
{
    const CPSCRenderMesh* source = (const CPSCRenderMesh*)&other;
    BankIndex = source->BankIndex;
    StartRenderSizeX = source->StartRenderSizeX;
    StartRenderSizeY = source->StartRenderSizeY;
    StartRenderSizeZ = source->StartRenderSizeZ;
    EndRenderSizeX = source->EndRenderSizeX;
    EndRenderSizeY = source->EndRenderSizeY;
    EndRenderSizeZ = source->EndRenderSizeZ;
    UseRenderSizeParam = source->UseRenderSizeParam;
    RenderSizeParam = source->RenderSizeParam;
    UseStartColour = source->UseStartColour;
    UseMidColour = source->UseMidColour;
    UseEndColour = source->UseEndColour;
    StartColour = source->StartColour;
    MidColour = source->MidColour;
    EndColour = source->EndColour;
    CentredOnPos = source->CentredOnPos;
    FadeInEndInteger = source->FadeInEndInteger;
    FadeOutBeginInteger = source->FadeOutBeginInteger;
    AlphaFadeEnable = source->AlphaFadeEnable;
    AlphaFadeMinimum = source->AlphaFadeMinimum;
    SizeFadeEnable = source->SizeFadeEnable;
    SizeFadeMinimum = source->SizeFadeMinimum;
    FlickerEnable = source->FlickerEnable;
    FlickerSpeed = source->FlickerSpeed;
    FlickerBias = source->FlickerBias;
    FlickerMinAlphaInteger = source->FlickerMinAlphaInteger;
    FlickerMinSizeInteger = source->FlickerMinSizeInteger;
    TrailBankIndex = source->TrailBankIndex;
    TrailLengthInteger = source->TrailLengthInteger;
    TrailWidth = source->TrailWidth;
    TrailUseStartColour = source->TrailUseStartColour;
    TrailUseMidColour = source->TrailUseMidColour;
    TrailUseEndColour = source->TrailUseEndColour;
    TrailStartColour = source->TrailStartColour;
    TrailMidColour = source->TrailMidColour;
    TrailEndColour = source->TrailEndColour;
    BlendMode = source->BlendMode;
    BlendOp = source->BlendOp;
    TrailBlendMode = source->TrailBlendMode;
    TrailBlendOp = source->TrailBlendOp;
}
}