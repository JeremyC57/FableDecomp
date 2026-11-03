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
        VillagePeople.push_back(CIntelligentPointer<CThingAICreature>(&creature));
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
        VillageBuildings.push_back(CIntelligentPointer<CThingBuilding>(&building));
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
                        if ((object->GetProperties() >> 8) & 1 || (object->GetProperties() >> 9) & 1) {
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
        if ((object.GetProperties() >> 9) & 1 || (object.GetProperties() >> 8) & 1) {
            VillageTorches.push_back(CIntelligentPointer<CThing>(&thing));
            if (!thing.IsDead()) {
                CTCLight* light;
                if (thing.GetTC(&light))
                    light->SetActive(!PeekWorld().PeekGameTimeManager().IsDay());
            }
        } else if ((object.GetProperties() >> 1) & 1) {
            VillageCrops.push_back(CIntelligentPointer<CThingObject>(&object));
            const std::vector<EVillageTaskType>& tasks = PDef->Tasks;
            if (std::find(tasks.begin(), tasks.end(), (EVillageTaskType)2) != tasks.end())
                CropTasks.push_back(CCountedPointer<CVillageTaskPickCrops>(new CVillageTaskPickCrops(object)));
        } else if ((object.GetProperties() >> 24) & 1) {
            VillageHorns.push_back(CIntelligentPointer<CThing>(&thing));
        } else if ((object.GetProperties() >> 28) & 1) {
            VillageGates.push_back(CIntelligentPointer<CThing>(&thing));
        } else if ((object.GetProperties() >> 3) & 1) {
            VillageCages.push_back(CIntelligentPointer<CThing>(&thing));
        } else if ((object.GetProperties() >> 14) & 1) {
            VillageCampfires.push_back(CIntelligentPointer<CThing>(&thing));
        } else if ((object.GetProperties() >> 2) & 1) {
        }
        break;
    }
    case 10: {                                           // track node
        CThingTrackNode& node = static_cast<CThingTrackNode&>(thing);
        if (node.IsStart && node.TrackName == PDef->PatrolTrackName.GetString()) {
            const std::vector<EVillageTaskType>& tasks = PDef->Tasks;
            if (std::find(tasks.begin(), tasks.end(), (EVillageTaskType)1) != tasks.end()) {
                VillagePatrols.push_back(CIntelligentPointer<CThingTrackNode>(&node));
                PatrolTasks.push_back(CCountedPointer<CVillageTaskPatrol>(new CVillageTaskPatrol(node)));
            }
        }
        break;
    }
    case 11:                                             // marker
        VillageMarkers.push_back(CIntelligentPointer<CThingMarker>(&static_cast<CThingMarker&>(thing)));
        break;
    case 13:
        break;
    default:
        if (thing.HasTC(TCI_DUMMY_VILLAGER))
            VillageDummies.push_back(CIntelligentPointer<CThing>(&thing));
        break;
    }
}
