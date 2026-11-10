#include <map>
#include <list>
#include <math.h>
#include <assert.h>
#include <stdio.h>
enum EDAMAGE_MELEEttribute {
    DAMAGE_NULL=-1,
    DAMAGE_MELEE=0,
    DAMAGE_MELEE_UNARMED=1,
    DAMAGE_LIGHTNING=2,
    DAMAGE_FIRE=3,
    DAMAGE_PROJECTILE=4,
    DAMAGE_EXPLOSION=5,
    DAMAGE_DRAIN=6,
    DAMAGE_DRAIN_HEAL=7,
    DAMAGE_GENERIC_WILL=8,
    DAMAGE_DIVINE_WRATH=9,
    DAMAGE_UNHOLY_POWER=10
};
class CHitParameters {
public:
    std::map<EDAMAGE_MELEEttribute,float> DamageMap;
    const std::map<EDAMAGE_MELEEttribute,float>& GetDamageMap()const { return DamageMap; }
};
class CComboMultiplierData {
public:
    std::map<EDAMAGE_MELEEttribute,float> ComboMultiplier;
    float ExperienceMultiplierNumSecondsForAttenuation;
};
class CMainGameComponent { public: static long GetConstantFPS(){return 32;} };
static long frame=17;
class CTCHeroExperience {
public:
    float ExperienceMultiplierParam,Variation;
    long ExperienceAttenuationTimer;
    int HistoryCalls,VariationCalls,UpdateCalls,DataCalls;
    CComboMultiplierData Data;
    std::list<std::pair<EDAMAGE_MELEEttribute,long> > AttackHistory;
    CTCHeroExperience():ExperienceMultiplierParam(1.0f),Variation(0.5f),ExperienceAttenuationTimer(99),HistoryCalls(0),VariationCalls(0),UpdateCalls(0),DataCalls(0){
        Data.ExperienceMultiplierNumSecondsForAttenuation=0.125f;
        for(int i=0;i<3;++i)AttackHistory.push_back(std::make_pair(DAMAGE_LIGHTNING,0L));
    }
    void AddAttackOfTypeToAttackHistory(EDAMAGE_MELEEttribute type){
        ++HistoryCalls;AttackHistory.pop_front();AttackHistory.push_back(std::make_pair(type,frame));
    }
    const CComboMultiplierData& GetActiveComboMultiplierData(){++DataCalls;return Data;}
    float GetAttackVariationMultiplier(){++VariationCalls;return Variation;}
    void UpdateComboMultiplier(bool increase){assert(increase);++UpdateCalls;}
    void AddSuccessfulMultistrikeHit(const CHitParameters& hit);
};
void CTCHeroExperience::AddSuccessfulMultistrikeHit(const CHitParameters& hit)
{
    for (std::map<EDAMAGE_MELEEttribute, float>::const_iterator it = hit.GetDamageMap().begin(); it != hit.GetDamageMap().end(); ++it) {
        EDAMAGE_MELEEttribute type = it->first;
        float damage = it->second;
        AddAttackOfTypeToAttackHistory(type);
        const CComboMultiplierData& data = GetActiveComboMultiplierData();
        ExperienceAttenuationTimer = (long)(CMainGameComponent::GetConstantFPS() * data.ExperienceMultiplierNumSecondsForAttenuation);
        std::map<EDAMAGE_MELEEttribute, float>::const_iterator found = data.ComboMultiplier.find(type);
        if (found != data.ComboMultiplier.end()) {
            float strength = (float)fabs(damage);
            ExperienceMultiplierParam += (GetAttackVariationMultiplier()) * strength * found->second;
            UpdateComboMultiplier(true);
        }
    }
}

int main(){
    CTCHeroExperience empty;CHitParameters hit;
    empty.AddSuccessfulMultistrikeHit(hit);
    assert(empty.ExperienceMultiplierParam==1.0f&&empty.ExperienceAttenuationTimer==99);
    assert(empty.HistoryCalls==0&&empty.UpdateCalls==0&&empty.DataCalls==0);
    for(int mask=0;mask<8;++mask)for(int sign=0;sign<8;++sign){
        CTCHeroExperience hero;float expected=1.0f;int updates=0;
        for(int i=0;i<3;++i){
            EDAMAGE_MELEEttribute type=(EDAMAGE_MELEEttribute)i;
            float amount=(float)(i+1)*0.25f;
            hit.DamageMap[type]=(sign&(1<<i))?-amount:amount;
            if(mask&(1<<i)){hero.Data.ComboMultiplier[type]=(float)(i+1)*0.5f;expected+=0.5f*amount*((float)(i+1)*0.5f);++updates;}
        }
        hero.AddSuccessfulMultistrikeHit(hit);
        assert(hero.ExperienceMultiplierParam==expected&&hero.ExperienceAttenuationTimer==4);
        assert(hero.HistoryCalls==3&&hero.DataCalls==3&&hero.UpdateCalls==updates&&hero.VariationCalls==updates);
        assert(hero.AttackHistory.size()==3);
        int i=0;for(std::list<std::pair<EDAMAGE_MELEEttribute,long> >::const_iterator it=hero.AttackHistory.begin();it!=hero.AttackHistory.end();++it,++i){assert(it->first==(EDAMAGE_MELEEttribute)i&&it->second==frame);}
    }
    CTCHeroExperience zero;zero.Data.ComboMultiplier[DAMAGE_MELEE]=2.0f;hit.DamageMap.clear();hit.DamageMap[DAMAGE_MELEE]=0.0f;
    zero.AddSuccessfulMultistrikeHit(hit);assert(zero.ExperienceMultiplierParam==1.0f&&zero.UpdateCalls==1);
    puts("006d8830_TEST PASS");return 0;
}