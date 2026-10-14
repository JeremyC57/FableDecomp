// CLHFileManager::CreateFile1(const SFileSpec_00aab7a0& spec) -- allocate a 0x14-byte CLHFile (operator new @ 0x00BFEA1A) and construct it from this manager and the spec (0x00ADD380); null when the allocation failed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAB7A0, length 48 bytes.
// Vtable 0x0129D2B4 slot 1 (labelled vfunc_1). Callees: operator new, CLHFile's constructor @ 0x00ADD380 (labelled "CPlayerGuiDef_LoadAnimationSet"). Shape: the `new` null test, `xor eax,eax`, `ret 4`.
struct SFileSpec_00aab7d0;
struct SFileSpec_00aab7a0;
struct CLHFileManager;
struct CLHFile {                                   // 0x14 bytes, vtable 0x0129F094
    unsigned char _body[0x14];
    CLHFile(CLHFileManager* manager, const SFileSpec_00aab7a0& spec);   // 0x00ADD380
};
struct CLHFileManager {                            // vtable 0x0129D2B4
    virtual CLHFile* CreateFile0(const SFileSpec_00aab7d0& spec);   // slot 0, 0x00AAB7D0
    virtual CLHFile* CreateFile1(const SFileSpec_00aab7a0& spec);   // slot 1, 0x00AAB7A0
};

CLHFile* CLHFileManager::CreateFile1(const SFileSpec_00aab7a0& spec)
{
    return new CLHFile(this, spec);
}