// CObstacleNavigatorExternalInterface::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x01241D04), the member at +8 destroyed (0x004DF54F, labelled "CActiveFile::OnReadFinished"), the CNavigatorExternalInterface base's inline destructor stores its vtable (0x01241D74), then free for flag bit 0 (the size-optimised module: `pop ecx`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF507, length 43 bytes.
// Leaf: ??_GCObstacleNavigatorExternalInterface@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x01241D04) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
#pragma optimize("s", on)
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CMember_004df54f {
    unsigned char _body[4];
    ~CMember_004df54f();                           // 0x004DF54F
};
struct CNavigatorExternalInterface {               // vtable 0x01241D74
    virtual void v00();
    long m_4;
    ~CNavigatorExternalInterface() { }             // inline: the vtable store
};
struct CObstacleNavigatorExternalInterface : CNavigatorExternalInterface {              // vtable 0x01241D04
    CMember_004df54f m_member8;                    // +0x08
    __forceinline virtual ~CObstacleNavigatorExternalInterface() { }
};

void* MakeOne_004df507()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CObstacleNavigatorExternalInterface;
}
