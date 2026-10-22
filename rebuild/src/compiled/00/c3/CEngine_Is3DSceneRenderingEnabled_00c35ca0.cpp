struct CEngine { unsigned char pad[0x10]; unsigned int flags; };

bool __stdcall Is3DSceneRenderingEnabled(CEngine* engine)
{
    return (engine->flags & 0x217) != 0;
}