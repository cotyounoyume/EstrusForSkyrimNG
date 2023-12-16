ScriptName EstrusForSkyrimNG

Bool Function IsDangerousWater(Form waterForm) global native
Function SetDangerousWater(Form waterForm, Bool isDangerous) global native

Actor Function GetFollower(Bool isFemale, Int Number) global native
Actor Function GetRandomFollower(Bool isFemale) global native
int Function GetFollowersCount(Bool isFemale) global native

String Function GetTriFilePath(Actor akActor, Int type) global native
Function SetTriFilePath(Actor akActor, Int type, String path) global native

Idle Function GetCurrentIdle(Actor akActor) global native
Function SetAnimObjectPath(Form animObject, String newPath) global native
String Function GetAnimObjectPath(Form animObject) global native

Function Debug1() global native

ObjectReference Function GetFurnitureReference(Actor akActor) global
	akActor.GetFurnitureReference()
EndFunction

Function SwitchTriFiles(Actor akActor, String headPath, String eyesPath, String browsPath) global
	SetTriFilePath(akActor, 1, headPath)
	SetTriFilePath(akActor, 2, eyesPath)
	SetTriFilePath(akActor, 6, browsPath)
EndFunction

Function SetUFOCamSpeedMult(Float ratio) global native
Function StartFlyCam() global native
Function StopFlyCam() global native
Function ToggleFlyCam() global native
