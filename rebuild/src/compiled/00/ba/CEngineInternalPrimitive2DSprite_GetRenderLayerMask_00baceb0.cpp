int __fastcall CEngineInternalPrimitive2DSprite_GetRenderLayerMask(const unsigned char* sprite)
{
    return *reinterpret_cast<const int*>(sprite + 48);
}