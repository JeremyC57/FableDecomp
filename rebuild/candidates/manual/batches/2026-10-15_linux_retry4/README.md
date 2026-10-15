# Retry batch 910

- 008491e0 CCreatureAction_SendMessage::PerformAction: parked in chunk 130 (106v110, arguments pushed before GetWorld). An inline
  `PostMessage(def, sender, receiver)` helper whose body starts with `CWorld* world = GetWorld();` puts the world call after the three
  register arguments and before the pushes. RELOCATION_MATCH.
