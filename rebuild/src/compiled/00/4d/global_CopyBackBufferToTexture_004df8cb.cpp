struct CTexture;

struct CDisplayManager {
    void Helper_004dd342(char *a, char *b);
    CTexture * CopyBackBufferToTexture();
};

#pragma optimize("t",off)
CTexture * CDisplayManager::CopyBackBufferToTexture()
{
    char *pa, *pb;
    { char a; pa = &a; }
    { char b; pb = &b; }
    Helper_004dd342(pa, pb);
    return (CTexture *)this;
}