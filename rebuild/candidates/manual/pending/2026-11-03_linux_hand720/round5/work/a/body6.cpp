void CTCVillage::AddThingToVillage(CThing& thing)
{
    CTCVillageMember* villageMember;
    thing.GetTC(&villageMember);
    if (villageMember->GetPVillage() == this)
        return;
    villageMember->SetPVillage(this);
    switch (thing.GetType()) {
    default:
        if (thing.HasTC(TCI_DUMMY_VILLAGER))
            VillageDummies.push_back(&thing);
        break;
    case 1: {                                            // AI creature
        CThingAICreature& creature = static_cast<CThingAICreature&>(thing);
        VillagePeople.push_back(&creature);
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
        VillageBuildings.push_back(&building);
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
            VillageTorches.push_back(&thing);
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
            VillageCrops.push_back(&object);
            if (std::find(PDef->Tasks.begin(), PDef->Tasks.end(), (EVillageTaskType)2) != PDef->Tasks.end())
                CropTasks.push_back(CCountedPointer<CVillageTaskPickCrops>(new CVillageTaskPickCrops(object)));
        } else if ((bool)((object.GetProperties() >> 24) & 1)) {
            VillageHorns.push_back(&thing);
        } else if ((bool)((object.GetProperties() >> 28) & 1)) {
            VillageGates.push_back(&thing);
        } else if ((bool)((object.GetProperties() >> 3) & 1)) {
            VillageCages.push_back(&thing);
        } else if ((bool)((object.GetProperties() >> 14) & 1)) {
            VillageCampfires.push_back(&thing);
        } else if ((bool)((object.GetProperties() >> 2) & 1)) {
        }
        break;
    }
    case 10: {                                           // track node
        CThingTrackNode& node = static_cast<CThingTrackNode&>(thing);
        if (node.IsStart && node.TrackName == PDef->PatrolTrackName) {
            if (std::find(PDef->Tasks.begin(), PDef->Tasks.end(), (EVillageTaskType)1) != PDef->Tasks.end()) {
                VillagePatrols.push_back(&node);
                PatrolTasks.push_back(CCountedPointer<CVillageTaskPatrol>(new CVillageTaskPatrol(node)));
            }
        }
        break;
    }
    case 11:                                             // marker
        VillageMarkers.push_back(&static_cast<CThingMarker&>(thing));
        break;
    case 13:
        break;
    }
}
