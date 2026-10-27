// NResourceDirectoryNames::GetHDDialogueDir() -- the language directory L"Data\\lang\\<language>\\"
// Leaf: ?GetHDDialogueDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041A330, 90 bytes, `__fastcall` (hidden CWideString result in ecx), plain `ret`.
// Identity (proven): after GetDVDDialogueDir and before GetDVDSoundDir (0x0041A390, L"Data\\Sound\\"), as in Ego_r.pdb; callers GFMain 0x00402510 and CNewFrontendGameComponent::Run 0x0042EC7C; retail TU order 0x0041A060 Levels, 0x0041A080 Defs, 0x0041A0A0/0x0041A0C0 Misc (Misc, HDMisc), 0x0041A100 Graphics (GraphicBank), 0x0041A120 lang (FontBank), 0x0041A180 Cache, 0x0041A1A0 (DebugSave), 0x0041A1C0 Meshes, 0x0041A1E0 Shaders, 0x0041A200 EngineCache, 0x0041A220 \Microsoft\Fable\ (WriteableEngineCache), 0x0041A290 Bones, 0x0041A2B0 Sound, 0x0041A2D0 lang (DVDDialogue), 0x0041A330 lang (HDDialogue), 0x0041A390.. Sound x4 (DVDSound, HDSound, HDMusic, DVDMusic), 0x0041A410 Levels (StaticMap) -- the Ego_r.pdb order of NResourceDirectoryNames::Get*Dir.
// Callees: NLocalisation::GetLanguageFolder 0x00415530 (returns CCharString "English"), CCharString::ToWideString 0x0099E480,
//   operator+(const unsigned short*, const CWideString&) 0x0099BFF0, operator+(const CWideString&, const unsigned short*)
//   0x0099BF30, CWideString::~CWideString 0x0099B510, CCharString::~CCharString 0x0099EAE0.
// Shape (family M019, representative 0x0041A120): one expression `L"Data\\lang\\" + GetLanguageFolder().ToWideString() +
//   L"\\"` -- the outer right operand L"\\" (0x0122DE78) is pushed first, the three temporaries die in reverse order.
class CWideString {
public:
    ~CWideString();                                       // 0x0099B510
    void* PStringData;
};
class CCharString {
public:
    ~CCharString();                                       // 0x0099EAE0
    CWideString ToWideString() const;                     // 0x0099E480
    void* PStringData;
};
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right);   // 0x0099BFF0
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30
namespace NLocalisation {
CCharString __fastcall GetLanguageFolder();               // 0x00415530
}
namespace NResourceDirectoryNames {
CWideString __fastcall GetHDDialogueDir()
{
    return (const unsigned short*)L"Data\\lang\\" + NLocalisation::GetLanguageFolder().ToWideString() + (const unsigned short*)L"\\";
}
}