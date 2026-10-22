struct CEngineScenePrimitiveStatsEnumerator {
    void* vftable;
};

void __fastcall CEngineScenePrimitiveStatsEnumerator_dtor(CEngineScenePrimitiveStatsEnumerator* enumerator);
void operator delete(void*);

void* __fastcall CEngineScenePrimitiveStatsEnumerator_scalar_deleting_destructor(CEngineScenePrimitiveStatsEnumerator* enumerator, int edx_unused, unsigned int flags)
{
    CEngineScenePrimitiveStatsEnumerator_dtor(enumerator);
    if (flags & 1)
        operator delete(enumerator);
    return enumerator;
}