#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <stdlib.h>

enum EOpinionReactionType { OPINION_REACTION_TYPE_WIFE_FIRST=57, OPINION_REACTION_TYPE_LAST=79 };
struct COpinionReactionMaskDef {
    bool ReactionEnabledDefault;
    std::map<EOpinionReactionType,bool> ReactionEnabled;
    int refs, releases;
};
template<class T> struct CDefPointer {
    T* object;
    CDefPointer() : object(0) {}
    ~CDefPointer() {
        if (object) {
            COpinionReactionMaskDef* writable=const_cast<COpinionReactionMaskDef*>(object);
            --writable->refs;
            ++writable->releases;
        }
    }
    T* operator->() const { return object; }
};
struct CDefString {
    const char* text;
    bool operator==(const char* rhs) const { return strcmp(text,rhs)==0; }
};
struct CGameDefinitionManager {
    bool class_ok, available;
    COpinionReactionMaskDef def;
    mutable int names, loads;
    mutable long name_index, def_index;
    CDefString GetDefClassTypeNameFromGlobalIndex(long i) const {
        ++names;
        name_index=i;
        CDefString s={ class_ok ? "OPINION_REACTION_MASK" : "OPINION_DEED_MASK" };
        return s;
    }
    bool GetDef(long i,CDefPointer<const COpinionReactionMaskDef>* p) const {
        ++loads;
        def_index=i;
        if (!available || i<1) return false;
        p->object=&def;
        ++const_cast<COpinionReactionMaskDef&>(def).refs;
        return true;
    }
};
struct CWorld {
    CGameDefinitionManager manager;
    CGameDefinitionManager const& PeekDefinitionManager() const { return manager; }
};
class CTCCreatureOpinionOfHero {
public:
    CWorld world;
    int world_calls;
    std::vector<bool> ReactionMask;
    CWorld& GetWorld() { ++world_calls; return world; }
    void SetOpinionReactionMask(long);
};

void CTCCreatureOpinionOfHero::SetOpinionReactionMask(long reaction_mask_def)
{
    for (int c0 = 0; c0 < OPINION_REACTION_TYPE_LAST; ++c0)
        ReactionMask[c0] = c0 < OPINION_REACTION_TYPE_WIFE_FIRST;

    if (!reaction_mask_def)
        return;

    const CGameDefinitionManager& def_mgr = GetWorld().PeekDefinitionManager();
    CDefPointer<const COpinionReactionMaskDef> pdef;
    if (!(def_mgr.GetDefClassTypeNameFromGlobalIndex(reaction_mask_def) == "OPINION_REACTION_MASK"))
        return;

    if (def_mgr.GetDef(reaction_mask_def, &pdef)) {
        for (int c0 = 0; c0 < OPINION_REACTION_TYPE_WIFE_FIRST; ++c0)
            ReactionMask[c0] = pdef->ReactionEnabledDefault;

        std::vector<bool> found_reaction(OPINION_REACTION_TYPE_LAST, false);
        typedef std::map<EOpinionReactionType, bool> ReactionMap;
        for (ReactionMap::const_iterator it = pdef->ReactionEnabled.begin(),
             end_it = pdef->ReactionEnabled.end(); it != end_it; ++it) {
            const EOpinionReactionType reaction_type = it->first;
            if (reaction_type < OPINION_REACTION_TYPE_LAST) {
                found_reaction[reaction_type] = true;
                ReactionMask[reaction_type] = it->second;
            }
        }
    }
}

int main() {
    const long indices[]={0,-1,1,17,123};
    const int keys[]={0,1,55,56,57,58,77,78,79,80,120};
    for (int q=0; q<5; ++q)
        for (int mode=0; mode<8; ++mode)
            for (int variant=0; variant<32; ++variant) {
                CTCCreatureOpinionOfHero component;
                CGameDefinitionManager& manager=component.world.manager;
                manager.class_ok=(mode&1)!=0;
                manager.available=(mode&2)!=0;
                manager.def.ReactionEnabledDefault=(mode&4)!=0;
                manager.def.refs=4; manager.def.releases=0;
                manager.names=manager.loads=component.world_calls=0;
                manager.name_index=manager.def_index=-999;
                component.ReactionMask.assign(83, false);
                bool expected[83];
                const bool loaded=indices[q]>0 && manager.class_ok && manager.available;
                int i;
                for (i=0;i<83;++i) {
                    component.ReactionMask[i]=((i+variant)&1)!=0;
                    expected[i]=(i>=79) ? component.ReactionMask[i] :
                        (i<57 && (!loaded || manager.def.ReactionEnabledDefault));
                }
                for (i=0;i<11;++i) {
                    if (!variant || (((variant*3+i)%5)==0)) continue;
                    const int key=keys[i];
                    const bool flag=((i+variant)&1)!=0;
                    manager.def.ReactionEnabled[static_cast<EOpinionReactionType>(key)]=flag;
                    if (loaded && key<79) expected[key]=flag;
                }
                component.SetOpinionReactionMask(indices[q]);
                for (i=0;i<83;++i)
                    if (component.ReactionMask[i]!=expected[i]) return 1;
                const bool lookup=indices[q]!=0;
                const bool fetch=lookup && manager.class_ok;
                if(component.world_calls!=int(lookup) || manager.names!=int(lookup) ||
                   manager.loads!=int(fetch) || manager.def.refs!=4 ||
                   manager.def.releases!=int(loaded)) return 2;
                if(manager.name_index!=(lookup ? indices[q] : -999) ||
                   manager.def_index!=(fetch ? indices[q] : -999)) return 3;
            }
    puts("00681cd0_TEST PASS");
    return 0;
}