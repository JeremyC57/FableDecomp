// CSystemRegistry::GetBOOL(const CWideString& valueName, int defaultValue) const -- read a registry flag: GetSLONG of the value (default as given), returned as a BOOL (0 or 1)
// Leaf: ?GetBOOL@CSystemRegistry@@QBEHABVCWideString@@H@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099CFE0, 26 bytes, thiscall `ret 8`: both arguments re-pushed, `call 0x0099CF80`
//   with ecx untouched, then `test eax,eax ; setne al ; movzx eax,al`.
// Identity (FINDINGS chunk 472; sweep2): Ego_r.pdb's name set has `?GetBOOL@CSystemRegistry@@QBEHABVCWideString@@H@Z`
//   next to GetSLONG / GetString / WriteBOOL; the callee 0x0099CF80 is the landed CSystemRegistry::GetSLONG (whose own
//   note already says GetBOOL is this next function); the caller is GFConfigDetection 0x004017B0 (startup settings).
//   The landed name "CEngineMap::PeekWaterHasType" is propagated (FINDINGS called it ReadBOOL; the PDB says GetBOOL).
class CWideString {
public:
    void* PStringData;                                  // +0x00
};

class CSystemRegistry {
public:
    long GetSLONG(const CWideString& valueName, long defaultValue) const;   // 0x0099CF80
    int  GetBOOL(const CWideString& valueName, int defaultValue) const;
};

int CSystemRegistry::GetBOOL(const CWideString& valueName, int defaultValue) const
{
    return static_cast<bool>(GetSLONG(valueName, defaultValue));   // bool, widened to the BOOL result (movzx)
}
