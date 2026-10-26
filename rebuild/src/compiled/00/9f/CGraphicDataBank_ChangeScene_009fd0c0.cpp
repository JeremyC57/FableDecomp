// CGraphicDataBank::ChangeScene() -- hand the scene word at +0x268 to the two resource lists at +0x168 and +0x1a4 (0x009FC160)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD0C0, length 48 bytes.
// Callees: the resource list's scene change @ 0x009FC160 twice (labelled "CTCCoopSpirit::AddExperience", propagated). Shape: the word re-read for the second call.
struct CResourceList_009fd0c0 {
    unsigned char _body[0x3c];
    void SetScene(long scene);                     // 0x009FC160
};

class CGraphicDataBank { public:
    unsigned char          _pad[0x168];
    CResourceList_009fd0c0 m_textures;             // +0x168
    CResourceList_009fd0c0 m_frames;               // +0x1a4
    unsigned char          _pad1e0[0x88];
    long                   m_scene;                // +0x268
    void ChangeScene();
};

void CGraphicDataBank::ChangeScene()
{
    m_textures.SetScene(m_scene);
    m_frames.SetScene(m_scene);
}