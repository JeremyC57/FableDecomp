def R(k,o,b,t):
    j=o.index('// Both original PDB field lists: MeshPrimitive/RenderMode/IsCloth are public.')
    assert o[j-1]=='#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER' and o[j+1]=='public:' and o[j+2]=='#else'
    assert t[0]=='#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_PUBLICMEMBERS'
    return o[:j-1]+['#if defined(FABLE_RETAIL_PASS22_STATIC_MESH_RENDER) || defined(FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_PUBLICMEMBERS)', o[j], t[1]]+o[j+1:]
