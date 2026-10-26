// Behaviour test for CActiveEntityScriptBase::CActiveEntityScriptBase (0x00CE1110).
#include <cstdio>
#include <cstring>

class CCharString { public:
    char text[16];
    CCharString() { text[0] = 0; }
    CCharString(const char* t) { std::strcpy(text, t); }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
};

struct SCountBlock { long m_count; };

class CScriptThing { public:
    void* PImp_Data; SCountBlock* PImp_Info; CCharString name;
    CScriptThing(void* d, SCountBlock* i, const char* n) : PImp_Data(d), PImp_Info(i), name(n) {}
    CScriptThing(const CScriptThing& o) : PImp_Data(o.PImp_Data), PImp_Info(o.PImp_Info), name(o.name)
    { if (PImp_Info != 0) ++PImp_Info->m_count; }
    const CCharString& GetName() const { return name; }
};

struct CScriptProcessBase { long a, size; float fraction; CScriptProcessBase(long x, long s, float f) : a(x), size(s), fraction(f) {} };

class CSpawnedFuncBase : public CScriptProcessBase { public:
    unsigned long ID; long LastScriptFrame; long CurrentScriptFrame; CCharString Name; CCharString AssociatedRegionName;
    bool FuncPaused; bool Interrupted; void* PPredicate[2];
    CSpawnedFuncBase(const CCharString& name)
        : CScriptProcessBase(1, 0x40, 0.1f), ID(1), LastScriptFrame(-1), CurrentScriptFrame(-1), Name(name),
          AssociatedRegionName(), FuncPaused(false), Interrupted(false) { PPredicate[0] = PPredicate[1] = 0; }
};

class CActiveEntityScriptBase : public CSpawnedFuncBase { public:
    void* PEntityScript[2]; unsigned long Flags; unsigned long long EntityUID; CScriptThing Entity;
    CActiveEntityScriptBase(const CScriptThing& entity, unsigned long long uid, unsigned long flags)
        : CSpawnedFuncBase(entity.GetName()), Flags(flags), EntityUID(uid), Entity(entity)
    { PEntityScript[0] = PEntityScript[1] = 0; }
};

int main()
{
    SCountBlock block; block.m_count = 1;
    int data = 0;
    CScriptThing thing(&data, &block, "HERO");
    CActiveEntityScriptBase s(thing, 0x123456789ULL, 5);
    if (std::strcmp(s.Name.text, "HERO") || s.ID != 1 || s.LastScriptFrame != -1 || s.CurrentScriptFrame != -1
        || s.size != 0x40 || s.Flags != 5 || s.EntityUID != 0x123456789ULL || s.Entity.PImp_Data != &data
        || block.m_count != 2 || s.FuncPaused || s.Interrupted || s.PEntityScript[0]) {
        std::printf("00ce1110_TEST FAIL\n");
        return 1;
    }
    std::printf("00ce1110_TEST PASS\n");
    return 0;
}