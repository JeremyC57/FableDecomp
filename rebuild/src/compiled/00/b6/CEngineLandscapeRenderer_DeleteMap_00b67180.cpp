struct CMapResource {
    virtual void* Destroy(unsigned int flags);
};

class CEngineMap { public:
    CMapResource* res;
};

class CEngineLandscapeRenderer { public:
    void DeleteMap(CEngineMap* map);
};

void CEngineLandscapeRenderer::DeleteMap(CEngineMap* map)
{
    CMapResource* r = map->res;
    if (r != 0)
        r->Destroy(1);
    map->res = 0;
}