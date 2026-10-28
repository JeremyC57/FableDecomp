// CWorld::ConvertWorldToStaticMapPath(const CWideString& world, bool xbox, bool runtime) -- builds the level-file path for a world: Data\\Levels\\ + the world's file name + the stb extension variant (_RT_XB / _RT / _XB / plain by the runtime and Xbox flags)
// Leaf: ConvertWorldToStaticMapPath
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049D550, 189 bytes (padded to 192), `ret 10h` (hidden result + three arguments;
//   `this` is never read). PDB name ?ConvertWorldToStaticMapPath@CWorld@@QBE?AVCWideString@@ABV2@_N1@Z. Twin of 0x0049D6B0.
// Callees: CWideString() 0x0099AED0, SplitPath 0x00997620 (`__fastcall`: path ecx, drive edx, directory/name/extension on
//   the stack), the levels-directory getter 0x0041A410 (`__fastcall`, hidden result in ecx), operator+(const
//   CWideString&, const CWideString&) 0x0099BE70 and operator+(const CWideString&, const wchar_t*) 0x0099BF30
//   (`__fastcall`, hidden result ecx, left edx), CWideString::operator= 0x0099B7D0, copy ctor 0x0099B720, ~CWideString
//   0x0099B510. Strings L"_RT_XB.stb" 0x01238BD8, L"_RT.stb" 0x01238BC8, L"_XB.stb" 0x01238BB8, L".stb" 0x01238BAC.
// Shape: four complete `name = Dir() + name + L"...";` statements under the runtime/xbox ifs, cross-jumped by VC (a
//   ternary on the literal loads it into eax first); the extension is pushed first (the outer + takes it from the
//   stack), then &name for the inner +; the temporaries land in the dead argument slots;
//   `return name;` copies into the hidden result (no NRVO).
typedef unsigned short wchar16;                    // wchar_t under the era defaults
class CWideString { public:
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);       // 0x0099BF30
class CAFile {
public:
    static void __fastcall SplitPath(const CWideString&, CWideString*, CWideString*, CWideString*, CWideString*);   // 0x00997620
};  // 0x00997620
CWideString __fastcall GetLevelsDirectory_0041a410();       // 0x0041A410 (L"Data\\Levels\\", hidden result in ecx)
class CWorld { public:
    CWideString ConvertWorldToStaticMapPath(const CWideString& world, bool xbox, bool runtime) const;
};
CWideString CWorld::ConvertWorldToStaticMapPath(const CWideString& world, bool xbox, bool runtime) const
{
    CWideString name;
    CAFile::SplitPath(world, 0, 0, &name, 0);
    if (runtime) {
        if (xbox)
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L"_RT_XB.stb";
        else
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L"_RT.stb";
    }
    else {
        if (xbox)
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L"_XB.stb";
        else
            name = GetLevelsDirectory_0041a410() + name + (const wchar16*)L".stb";
    }
    return name;
}