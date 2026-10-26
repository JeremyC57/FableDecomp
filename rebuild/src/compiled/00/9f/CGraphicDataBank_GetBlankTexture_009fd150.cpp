class CTexture; enum ETextureType {};
class CGraphicDataBank { public: CTexture* GetBlankTexture(ETextureType t); };
CTexture* CGraphicDataBank::GetBlankTexture(ETextureType t){ return (CTexture*)((char*)this + (int)t*8 + 0x2b4); }