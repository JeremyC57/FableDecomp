void CTCVillage::AddThingToVillage(CThing& thing)
{
    CTCVillageMember* villageMember;
    thing.GetTC(&villageMember);
    if (villageMember->GetPVillage() == this)
        return;
    villageMember->SetPVillage(this);
    switch (thing.GetType()) {
    case 1: {                                            // AI creature
        CThingAICreature& creature = static_cast<CThingAICreature&>(thing);
        CIntelligentPointer<CThingAICreature> person(&creature);
        VillagePeople.push_back(person);
        if (creature.GetCreatureType() == 3)
            ++NumGuards;
        if (LaggyLimbo) {
            if (!thing.HasScriptThing() || !thing.HasTC(TCI_SCRIPTED_CONTROL)) {
                thing.SetInLimbo(true);
                thing.SetPersistOnUnloadWhenInLimbo(true);
            }
        }
        break;
    }
    case 3: {                                            // building
        CThingBuilding& building = static_cast<CThingBuilding&>(thing);
        {
            CIntelligentPointer<CThingBuilding> pointer(&building);
            VillageBuildings.push_back(pointer);
        }
        building.GetPhysicsTC()->GetPhysicsMeshInfo();
        if (LaggyLimbo) {
            building.SetOverrideAutomaticLocking(true);
            building.LockAllDoors(true, false);
        }
        CTCThingOwner* owner;
        if (thing.GetTC(&owner)) {
            for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->GetObjectList().begin();
                 it != owner->GetObjectList().end(); ++it) {
                if (IS_THING_ALIVE(*it) && (*it)->GetType() == 5) {
                    CThingObject* object = static_cast<CThingObject*>(it->Get());
                    CTCLight* light;
                    if (object->GetTC(&light)) {
                        if ((bool)((object->GetProperties() >> 8) & 1) || (bool)((object->GetProperties() >> 9) & 1)) {
                            bool active;
                            if (thing.HasTC(TCI_TAVERN))
                                active = !PeekWorld().PeekGameTimeManager().IsDay();
                            else
                                active = PeekWorld().PeekGameTimeManager().IsTimeOfDayInRange(GetPDef()->HouseLightsOnTime,
                                                                                             GetPDef()->HouseLightsOffTime);
                            light->SetActive(active);
                        }
                    }
                }
            }
        }
        break;
    }
    case 5: {                                            // object
        CThingObject& object = static_cast<CThingObject&>(thing);
        if ((bool)((object.GetProperties() >> 9) & 1) || (bool)((object.GetProperties() >> 8) & 1)) {
            {
                CIntelligentPointer<CThing> torch(&thing);
                VillageTorches.push_back(torch);
            }
            if (!thing.IsDead()) {
                CTCLight* light;
                if (thing.GetTC(&light)) {
                    if (PeekWorld().PeekGameTimeManager().IsDay())
                        light->SetActive(false);
                    else
                        light->SetActive(true);
                }
            }
        } else if ((bool)((object.GetProperties() >> 1) & 1)) {
            { CIntelligentPointer<CThingObject> pointer(&object); VillageCrops.push_back(pointer); }
            if (std::find(PDef->Tasks.begin(), PDef->Tasks.end(), (EVillageTaskType)2) != PDef->Tasks.end())
                CropTasks.push_back(CCountedPointer<CVillageTaskPickCrops>(new CVillageTaskPickCrops(object)));
        } else if ((bool)((object.GetProperties() >> 24) & 1)) {
            { CIntelligentPointer<CThing> pointer(&thing); VillageHorns.push_back(pointer); }
        } else if ((bool)((object.GetProperties() >> 28) & 1)) {
            { CIntelligentPointer<CThing> pointer(&thing); VillageGates.push_back(pointer); }
        } else if ((bool)((object.GetProperties() >> 3) & 1)) {
            { CIntelligentPointer<CThing> pointer(&thing); VillageCages.push_back(pointer); }
        } else if ((bool)((object.GetProperties() >> 14) & 1)) {
            { CIntelligentPointer<CThing> pointer(&thing); VillageCampfires.push_back(pointer); }
        } else if ((bool)((object.GetProperties() >> 2) & 1)) {
        }
        break;
    }
    case 10: {                                           // track node
        CThingTrackNode& node = static_cast<CThingTrackNode&>(thing);
        if (node.IsStart && node.TrackName == PDef->PatrolTrackName.GetString()) {
            if (std::find(PDef->Tasks.begin(), PDef->Tasks.end(), (EVillageTaskType)1) != PDef->Tasks.end()) {
                { CIntelligentPointer<CThingTrackNode> pointer(&node); VillagePatrols.push_back(pointer); }
                PatrolTasks.push_back(CCountedPointer<CVillageTaskPatrol>(new CVillageTaskPatrol(node)));
            }
        }
        break;
    }
    case 11:                                             // marker
        { CIntelligentPointer<CThingMarker> pointer(&static_cast<CThingMarker&>(thing)); VillageMarkers.push_back(pointer); }
        break;
    case 13:
        break;
    default:
        if (thing.HasTC(TCI_DUMMY_VILLAGER))
            { CIntelligentPointer<CThing> pointer(&thing); VillageDummies.push_back(pointer); }
        break;
    }
}
