scriptName zzEstrusUtil

;;; Estrus API
;;;	EstrusEffectStopAPI(Actor akTarget)
;;;		stop estrus effect
;;;
;;;	EstrusEffectAPI(Actor akTarget, Int EffectType = 1, Int AnimType = 0, Int AnimPattern = 0, bool debugOn = false)
;;;		start estrus effect
;;;		EffectType:
;;;			1:	Estrus
;;;			2:	Estrus Self
;;;			3:	Estrus Undress
;;;		AnimType:
;;;			1:	Estrus Tentacle
;;;			2:	Estrus Etc
;;;			3:	Estrus Machine
;;;			99:	Random
;;;		AnimPattern:
;;;			0:	Random
;;;			1:	pattern1
;;;			2:	pattern2
;;;			3:	pattern3 (reserve)
;;;			...
;;;			6:	pattern6 (reserve)
;;;

function EstrusEffectStopAPI(Actor akTarget, bool debugOn = false) global
	_EstrusEffectStopAPI(akTarget, debugOn)
endfunction

function EstrusEffectEscapeAPI(Actor akTarget, bool debugOn = false) global
	_EstrusEffectEscapeAPI(akTarget, debugOn)
endfunction

int function EstrusEffectGetTypeAPI(Actor akTarget, bool debugOn = false) global
	return _EstrusEffectGetTypeAPI(akTarget, debugOn)
endfunction

int function EstrusEffectGetSetAPI(Actor akTarget, bool debugOn = false) global
	return _EstrusEffectGetSetAPI(akTarget, debugOn)
endfunction

int function EstrusEffectGetStageAPI(Actor akTarget, bool debugOn = false) global
	return _EstrusEffectGetStageAPI(akTarget, debugOn)
endfunction

function EstrusEffectAPIEndless(Actor akTarget, Int EffectType = 1, Int AnimType = 0, Int AnimPattern = 0 , Int RepeatNumberStage4 = 6, Int RepeatNumberStage6 = 6, Int RepeatNumberStage8 = 12, bool onlooker = true, bool enableShortcut = true, bool debugOn = true) global
	_EstrusEffectAPIEndless(akTarget, EffectType, AnimType, AnimPattern, RepeatNumberStage4, RepeatNumberStage6, RepeatNumberStage8, onlooker, enableShortcut, debugOn)
endfunction

function EstrusEffectAPIEx(Actor akTarget, Int EffectType = 1, Int AnimType = 0, Int AnimPattern = 0, Int EscapeChanceStage1 = 50, Int EscapeChanceStage4 = 10, Int EscapeChanceStage6 = 5, Int EscapeChanceStage8 = 1, Int RepeatNumberStage4 = 6, Int RepeatNumberStage6 = 6, Int RepeatNumberStage8 = 12, bool onlooker = true, bool debugOn = true) global
	_EstrusEffectAPIEx(akTarget, EffectType, AnimType, AnimPattern, EscapeChanceStage1, EscapeChanceStage4, EscapeChanceStage6, EscapeChanceStage8, RepeatNumberStage4, RepeatNumberStage6, RepeatNumberStage8, onlooker, debugOn)
endfunction

function EstrusEffectAPI(Actor akTarget, Int EffectType = 1, Int AnimType = 0, Int AnimPattern = 0, bool onlooker = true, bool debugOn = true) global
	_EstrusEffectAPI(akTarget, EffectType, AnimType, AnimPattern, onlooker, debugOn)
EndFunction

Function EstrusEffectSetTypeAPI(Actor akTarget, Int AnimType, bool debugOn = false) global
	_EstrusEffectSetTypeAPI(akTarget, AnimType, debugOn)
EndFunction

Function EstrusEffectSetSetAPI(Actor akTarget, Int AnimPattern, bool debugOn = false) global
	_EstrusEffectSetSetAPI(akTarget, AnimPattern, debugOn)
EndFunction

Function EstrusEffectSetRepeatNumberAPI(Actor akTarget, Int stage, Int num, bool debugOn = false) global
	_EstrusEffectSetRepeatNumberAPI(akTarget, stage, num, debugOn)
EndFunction

Function EstrusEffectSetEscapeChanceAPI(Actor akTarget, Int stage, Int num, bool debugOn = false) global
	_EstrusEffectSetEscapeChanceAPI(akTarget, stage, num, debugOn)
EndFunction

Function EstrusEffectSetOnlookerAPI(Actor akTarget, bool onlooker, bool debugOn = false) global
	_EstrusEffectSetOnlookerAPI(akTarget, onlooker, debugOn)
EndFunction

Function ClearAllTargetAndOnlookers(bool debugOn = false) global
	_ClearAllTargetAndOnlookers(debugOn)
EndFunction

Function GatheringOnlookers(actor akTarget, bool debugOn = false) global
	_GatheringOnlookers(akTarget, debugOn)
EndFunction

Function SetLastTarget(actor target, bool debugOn = false) global
	_SetLastTarget(target, debugOn)
EndFunction

Function ReevaluatingOnlookers(bool onlookerEnable, bool debugOn = false) global
	_ReevaluatingOnlookers(onlookerEnable, debugOn)
EndFunction

Function AddOnlookerDisablerToActor(actor target, bool debugOn = false) global
	_AddOnlookerDisablerToActor(target, debugOn)
EndFunction

Function RemoveOnlookerDisablerToActor(actor target, bool debugOn = false) global
	_RemoveOnlookerDisablerToActor(target, debugOn)
EndFunction

Function AddSpellDisablerToActor(actor target, bool debugOn = false) global
	_AddSpellDisablerToActor(target, debugOn)
EndFunction

Function RemoveSpellDisablerToActor(actor target, bool debugOn = false) global
	_RemoveSpellDisablerToActor(target, debugOn)
EndFunction

armor Function GetOnlookerDisablerForm(bool debugOn = false) global
	return _GetOnlookerDisablerForm(debugOn)
EndFunction

armor Function GetSpellDisablerForm(bool debugOn = false) global
	return _GetSpellDisablerForm(debugOn)
EndFunction
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;






function _EstrusEffectStopAPI(Actor akTarget, bool debugOn) global
		Debug.TraceConditional("=================Estrus API abort(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)		
		Spell zzEstrusStopAbility = game.GetFormFromFile(0x0101E42A,"zzEstrus.esp") as spell
		akTarget.AddSpell(zzEstrusStopAbility)
endfunction

function _EstrusEffectEscapeAPI(Actor akTarget, bool debugOn) global
		Debug.TraceConditional("=================Estrus API Escape(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		Spell zzEstrusEscapeAbility = game.GetFormFromFile(0x0101E42B,"zzEstrus.esp") as spell
		akTarget.AddSpell(zzEstrusEscapeAbility)
endfunction

int function _EstrusEffectGetTypeAPI(Actor akTarget, bool debugOn) global
		Debug.TraceConditional("=================Estrus API GetType(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		MiscObject zzEstrusTokenType = game.GetFormFromFile(0x0101E996,"zzEstrus.esp") as MiscObject
		int num = akTarget.GetItemCount(zzEstrusTokenType)
		debug.TraceConditional("Estrus: INF: EstrusEffectGetTypeAPI  target=" + akTarget as String + ", type =" + num as String, debugOn)
		return num
endfunction

int function _EstrusEffectGetSetAPI(Actor akTarget, bool debugOn) global
		Debug.TraceConditional("=================Estrus API GetSet(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		MiscObject zzEstrusTokenSet = game.GetFormFromFile(0x0101E997,"zzEstrus.esp") as MiscObject
		int num = akTarget.GetItemCount(zzEstrusTokenSet)
		debug.TraceConditional("Estrus: INF: EstrusEffectGetSetAPI   target=" + akTarget as String + ", set  =" + num as String, debugOn)
		return num
endfunction

int function _EstrusEffectGetStageAPI(Actor akTarget, bool debugOn) global
		Debug.TraceConditional("=================Estrus API GetStage(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		MiscObject zzEstrusTokenStage = game.GetFormFromFile(0x0101E995,"zzEstrus.esp") as MiscObject
		int num = akTarget.GetItemCount(zzEstrusTokenStage)
		debug.TraceConditional("Estrus: INF: EstrusEffectGetStageAPI target=" + akTarget as String + ", stage=" + num as String, debugOn)
		return num
endfunction

function _EstrusEffectAPIEndless(Actor akTarget, Int EffectType, Int AnimType, Int AnimPattern, Int RepeatNumberStage4, Int RepeatNumberStage6, Int RepeatNumberStage8, bool onlooker, bool enableShortcut, bool debugOn) global
	string myname = akTarget.GetActorBase().GetName()
	Debug.TraceConditional("=================Estrus APIEx Start(" + myname + akTarget + ")================", debugOn)
	
	MiscObject zzEstrusTokenStage = game.GetFormFromFile(0x0101E995,"zzEstrus.esp") as MiscObject
	int num = akTarget.GetItemCount(zzEstrusTokenStage)
	if(num > 0)
		akTarget.removeItem(zzEstrusTokenStage, num)
	EndIf
	_EstrusEffectSetEndlessAPI(akTarget, enableShortcut, debugOn)
	EstrusEffectSetRepeatNumberAPI(akTarget, 4, RepeatNumberStage4, debugOn)
	EstrusEffectSetRepeatNumberAPI(akTarget, 6, RepeatNumberStage6, debugOn)
	EstrusEffectSetRepeatNumberAPI(akTarget, 8, RepeatNumberStage8, debugOn)
	EstrusEffectAPI(akTarget, EffectType, AnimType, AnimPattern, onlooker, debugOn)
endfunction

function _EstrusEffectAPIEx(Actor akTarget, Int EffectType, Int AnimType, Int AnimPattern, Int EscapeChanceStage1, Int EscapeChanceStage4, Int EscapeChanceStage6, Int EscapeChanceStage8, Int RepeatNumberStage4, Int RepeatNumberStage6, Int RepeatNumberStage8, bool onlooker, bool debugOn) global
	string myname = akTarget.GetActorBase().GetName()
	Debug.TraceConditional("=================Estrus APIEx Start(" + myname + akTarget + ")================", debugOn)
	
	EstrusEffectSetEscapeChanceAPI(akTarget, 1, EscapeChanceStage1, debugOn)
	EstrusEffectSetEscapeChanceAPI(akTarget, 4, EscapeChanceStage4, debugOn)
	EstrusEffectSetEscapeChanceAPI(akTarget, 6, EscapeChanceStage6, debugOn)
	EstrusEffectSetEscapeChanceAPI(akTarget, 8, EscapeChanceStage8, debugOn)
	EstrusEffectSetRepeatNumberAPI(akTarget, 4, RepeatNumberStage4, debugOn)
	EstrusEffectSetRepeatNumberAPI(akTarget, 6, RepeatNumberStage6, debugOn)
	EstrusEffectSetRepeatNumberAPI(akTarget, 8, RepeatNumberStage8, debugOn)
	EstrusEffectAPI(akTarget, EffectType, AnimType, AnimPattern, onlooker, debugOn)
endfunction

function _EstrusEffectAPI(Actor akTarget, Int EffectType, Int AnimType, Int AnimPattern, bool onlooker, bool debugOn) global
	string myname = akTarget.GetActorBase().GetName()
	int estrusStage
		
	if(EffectType != 3)
		estrusStage = EstrusEffectGetStageAPI(akTarget, debugOn)
		if(estrusStage > 0)
			Debug.TraceConditional("=================Estrus API abort(" + myname + akTarget + ")================", debugOn)
			EstrusEffectStopAPI(akTarget, debugOn)
			return
		EndIf
		Debug.TraceConditional("=================Estrus API Start(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		EstrusEffectSetTypeAPI(akTarget, AnimType, debugOn)
		EstrusEffectSetSetAPI(akTarget, AnimPattern, debugOn)
	EndIf
	EstrusEffectSetOnlookerAPI(akTarget, onlooker, debugOn)
	
	Spell rAPI;
	Spell rAPI2;
	spell zzEstrusEffectForAPI = game.GetFormFromFile(0x010166e6, "zzEstrus.esp") as spell
	spell zzEstrusEffectSelfForAPI = game.GetFormFromFile(0x01016c55, "zzEstrus.esp") as spell
	spell zzEstrusUndressForAPI = game.GetFormFromFile(0x01016c56, "zzEstrus.esp") as spell
	spell zzEstrusEffectForAPI2 = game.GetFormFromFile(0x0102707d, "zzEstrus.esp") as spell
	spell zzEstrusEffectSelfForAPI2 = game.GetFormFromFile(0x0102707e, "zzEstrus.esp") as spell
	spell zzEstrusUndressForAPI2 = game.GetFormFromFile(0x01027080, "zzEstrus.esp") as spell

	if(EffectType == 1)
		rAPI = zzEstrusEffectForAPI
		rAPI2= zzEstrusEffectForAPI2
	elseIf(EffectType == 2)
		rAPI = zzEstrusEffectSelfForAPI
		rAPI2= zzEstrusEffectSelfForAPI2
	elseif(EffectType == 3)
		rAPI = zzEstrusUndressForAPI
		rAPI2= zzEstrusUndressForAPI2
	else
		Debug.TraceConditional("=================Estrus API abort(" + myname + akTarget + ")================", debugOn)
		return
	EndIf
	
;	ObjectReference zzEstrusMarker = game.GetFormFromFile(0x01016c51, "zzEstrus.esp") as ObjectReference
	Actor rPlayer = game.GetForm(0x00000014) as actor
;	if(zzEstrusMarker)
;	Else
;		Debug.TraceConditional("=================Estrus API abort(" + myname + akTarget + ") can't get estrusMarker", debugOn)
;		return
;	EndIf
	if(akTarget)
	Else
		Debug.TraceConditional("=================Estrus API abort(" + myname + akTarget + ") can't get target actor", debugOn)
		return
	EndIf
	
;	zzEstrusMarker.MoveTo(akTarget)
	if((EffectType == 1 && akTarget != rPlayer) || (EffectType == 2 && akTarget == rPlayer) || EffectType == 3)
		Debug.TraceConditional("=================Estrus API cast start(" + myname + akTarget + rAPI + ")================", debugOn)
		akTarget.AddSpell(rAPI2, false)
;		rAPI.cast(zzEstrusMarker,akTarget)
		Utility.wait(2)
		Debug.TraceConditional("=================Estrus API cast finish(" + myname + akTarget + ")================", debugOn)
;	Elseif(EffectType == 3)
;		Debug.TraceConditional("=================Estrus API cast start(" + myname + akTarget + ")================", debugOn)
;		rAPI.cast(zzEstrusMarker,akTarget)
;		Utility.wait(2)
;		Debug.TraceConditional("=================Estrus API cast finish(" + myname + akTarget + ")================", debugOn)
	Else
		Debug.TraceConditional("=================Estrus API abort(" + myname + akTarget + ")================", debugOn)
	EndIf

	Debug.TraceConditional("=================Estrus API Finish(" + myname + akTarget + ")================", debugOn)	
EndFunction

function _EstrusEffectSetEndlessAPI(Actor akTarget, bool enableShortcut, bool debugOn) global
		Debug.TraceConditional("=================Estrus API SetEndless(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		MiscObject zzEstrusTokenEndless = game.GetFormFromFile(0x01026B19,"zzEstrus.esp") as MiscObject
		int currentNum = akTarget.GetItemCount(zzEstrusTokenEndless)
		if(currentNum > 0)
			akTarget.removeItem(zzEstrusTokenEndless, currentNum, true)
		endIf
		akTarget.addItem(zzEstrusTokenEndless, 1, true)
		
		MiscObject zzEstrusTokenShortcut = game.GetFormFromFile(0x01026B1A,"zzEstrus.esp") as MiscObject
		currentNum = akTarget.GetItemCount(zzEstrusTokenShortcut)
		if(currentNum > 0)
			akTarget.removeItem(zzEstrusTokenShortcut, currentNum, true)
		endIf
		if(enableShortcut)
			akTarget.addItem(zzEstrusTokenShortcut, 1, true)
		EndIf
		debug.TraceConditional("Estrus: INF: _EstrusEffectSetEndlessAPI target=" + akTarget as String, debugOn)
endfunction

function _EstrusEffectSetOnlookerAPI(Actor akTarget, bool onlooker, bool debugOn) global
		Debug.TraceConditional("=================Estrus API SetOnlooker(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		MiscObject zzEstrusTokenOnlooker = game.GetFormFromFile(0x0101F463,"zzEstrus.esp") as MiscObject
		int currentNum = akTarget.GetItemCount(zzEstrusTokenOnlooker)
		if(currentNum > 0)
			akTarget.removeItem(zzEstrusTokenOnlooker, currentNum, true)
		endIf
		if(onlooker)
			akTarget.addItem(zzEstrusTokenOnlooker, 1, true)
		endIf
		debug.TraceConditional("Estrus: INF: EstrusEffectSetOnlookerAPI target=" + akTarget as String + ", onlooker=" + onlooker as String, debugOn)
endfunction

Function _EstrusEffectSetTypeAPI(Actor akTarget, Int AnimType, bool debugOn = false) global
		Debug.TraceConditional("=================Estrus API SetType(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		MiscObject zzEstrusTokenType = game.GetFormFromFile(0x0101E996,"zzEstrus.esp") as MiscObject
		int currentNum = akTarget.GetItemCount(zzEstrusTokenType)
		if(currentNum > 0)
			akTarget.removeItem(zzEstrusTokenType, currentNum, true)
		endIf
		if(AnimType > 0)
			akTarget.addItem(zzEstrusTokenType, AnimType, true)
		endIf
		debug.TraceConditional("Estrus: INF: EstrusEffectSetTypeAPI  target=" + akTarget as String + ", type: " + currentNum + "=>" + AnimType, debugOn)
EndFunction

Function _EstrusEffectSetSetAPI(Actor akTarget, Int AnimPattern, bool debugOn = false) global
		Debug.TraceConditional("=================Estrus API SetSet(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		MiscObject zzEstrusTokenSet  = game.GetFormFromFile(0x0101E997,"zzEstrus.esp") as MiscObject
		int currentNum = akTarget.GetItemCount(zzEstrusTokenSet)
		if(currentNum > 0)
			akTarget.removeItem(zzEstrusTokenSet, currentNum, true)
		endIf
		if(AnimPattern > 0)
			akTarget.addItem(zzEstrusTokenSet, AnimPattern, true)
		endIf
		debug.TraceConditional("Estrus: INF: EstrusEffectSetSetAPI   target=" + akTarget as String + ", set : " + currentNum + "=>" + AnimPattern, debugOn)
EndFunction

Function _EstrusEffectSetRepeatNumberAPI(Actor akTarget, Int stage, Int num, bool debugOn = false) global
		Debug.TraceConditional("=================Estrus API Set RepeatNumber(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		
		MiscObject zzEstrusTokenRepeatStage
		if(stage == 4)
			zzEstrusTokenRepeatStage = game.GetFormFromFile(0x0101E998,"zzEstrus.esp") as MiscObject
		elseIf(stage == 6)
			zzEstrusTokenRepeatStage = game.GetFormFromFile(0x0101E999,"zzEstrus.esp") as MiscObject
		elseIf(stage == 8)
			zzEstrusTokenRepeatStage = game.GetFormFromFile(0x0101E99A,"zzEstrus.esp") as MiscObject
		else
			Debug.TraceConditional("=================Estrus API Set RepeatNumber: abort: invalid stage: " + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		endIf
		
		int currentNum = akTarget.GetItemCount(zzEstrusTokenRepeatStage)
		if(currentNum > 0)
			akTarget.removeItem(zzEstrusTokenRepeatStage, currentNum, true)
		endIf
		if(num > 0)
			akTarget.addItem(zzEstrusTokenRepeatStage, num, true)
		endIf
		debug.TraceConditional("Estrus: INF: EstrusEffectSetRepeatNumberInStage target=" + akTarget as String + ", repeat stage: " + stage + ", repeat num: " + currentNum + "=>" + num, debugOn)
EndFunction

Function _EstrusEffectSetEscapeChanceAPI(Actor akTarget, Int stage, Int num, bool debugOn = false) global
		Debug.TraceConditional("=================Estrus API Set EscapeChance(" + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		MiscObject zzEstrusTokenEscapeChance
		if(stage == 1)
			zzEstrusTokenEscapeChance = game.GetFormFromFile(0x0101E99B,"zzEstrus.esp") as MiscObject
		elseIf(stage == 4)
			zzEstrusTokenEscapeChance = game.GetFormFromFile(0x0101E99C,"zzEstrus.esp") as MiscObject
		elseIf(stage == 6)
			zzEstrusTokenEscapeChance = game.GetFormFromFile(0x0101E99D,"zzEstrus.esp") as MiscObject
		elseIf(stage == 8)
			zzEstrusTokenEscapeChance = game.GetFormFromFile(0x0101E99E,"zzEstrus.esp") as MiscObject
		else
			Debug.TraceConditional("=================Estrus API Set EscapeChance: abort: invalid stage: " + akTarget.GetActorBase().GetName() + akTarget + ")================", debugOn)
		endIf

		int currentNum = akTarget.GetItemCount(zzEstrusTokenEscapeChance)
		if(currentNum > 0)
			akTarget.removeItem(zzEstrusTokenEscapeChance, currentNum, true)
		endIf
		if(num > 0)
			akTarget.addItem(zzEstrusTokenEscapeChance, num, true)
		endIf
		debug.TraceConditional("Estrus: INF: EstrusEffectSetEscapeChance target=" + akTarget as String + ", EscapeChance stage: " + stage + ", Chance: "+ currentNum + "%=>" + num + "%", debugOn)
EndFunction

Function _SetLastTarget(actor target, bool debugOn) global
	Debug.TraceConditional("=================Estrus API SetLastTarget(" + target + ")================", debugOn)
	int handle = ModEvent.Create("SetLastTarget")
    if (handle)
		ModEvent.PushForm(handle, target as form)
		ModEvent.Send(handle)
	EndIf
EndFunction

Function _ReevaluatingOnlookers(bool onlookerEnable, bool debugOn) global
	Debug.TraceConditional("=================Estrus API ReevaluatingOnlookers()================", debugOn)
	int handle = ModEvent.Create("ReevaluatingOnlookers")
    if (handle)
		ModEvent.PushBool(handle, onlookerEnable)
		ModEvent.Send(handle)
	EndIf
EndFunction

Function _ClearAllTargetAndOnlookers(bool debugOn) global
	Debug.TraceConditional("=================Estrus API ClearAllTargetAndOnlookers()================", debugOn)
	int handle = ModEvent.Create("ClearingOnlookers")
    if (handle)
		ModEvent.Send(handle)
	EndIf
EndFunction

Function _GatheringOnlookers(form akTarget, bool debugOn) global
	Debug.TraceConditional("=================Estrus API GatheringOnlookers()================", debugOn)
	int handle = ModEvent.Create("GatheringOnlookers")
    if (handle)
		ModEvent.PushForm(handle, akTarget as form)
		ModEvent.Send(handle)
	EndIf
EndFunction

Function _AddOnlookerDisablerToActor(actor target, bool debugOn) global
	Debug.TraceConditional("=================Estrus API AddOnlookerDisablerToActor(" + target + ")================", debugOn)
	armor zzEstrusOnlookerDisabler = game.GetFormFromFile(0x01022F9F,"zzEstrus.esp") as armor
	if(target.getItemCount(zzEstrusOnlookerDisabler) == 0)
		target.addItem(zzEstrusOnlookerDisabler, 1)
	EndIf
EndFunction

Function _RemoveOnlookerDisablerToActor(actor target, bool debugOn) global
	Debug.TraceConditional("=================Estrus API RemoveOnlookerDisablerToActor(" + target + ")================", debugOn)
	armor zzEstrusOnlookerDisabler = game.GetFormFromFile(0x01022F9F,"zzEstrus.esp") as armor
	int num = target.getItemCount(zzEstrusOnlookerDisabler)
	if(num > 0)
		target.removeItem(zzEstrusOnlookerDisabler, num)
	EndIf
EndFunction

Function _AddSpellDisablerToActor(actor target, bool debugOn) global
	Debug.TraceConditional("=================Estrus API AddSpellDisablerToActor(" + target + ")================", debugOn)
	armor zzEstrusSpellDisabler = game.GetFormFromFile(0x01022FA0,"zzEstrus.esp") as armor
	if(target.getItemCount(zzEstrusSpellDisabler) == 0)
		target.addItem(zzEstrusSpellDisabler, 1)
	EndIf
EndFunction

Function _RemoveSpellDisablerToActor(actor target, bool debugOn) global
	Debug.TraceConditional("=================Estrus API RemoveSpellDisablerToActor(" + target + ")================", debugOn)
	armor zzEstrusSpellDisabler = game.GetFormFromFile(0x01022FA0,"zzEstrus.esp") as armor
	int num = target.getItemCount(zzEstrusSpellDisabler)
	if(num > 0)
		target.removeItem(zzEstrusSpellDisabler, num)
	EndIf
EndFunction

armor Function _GetOnlookerDisablerForm(bool debugOn) global
	Debug.TraceConditional("=================Estrus API GetOnlookerDisablerForm================", debugOn)
	return game.GetFormFromFile(0x01022F9F,"zzEstrus.esp") as armor
EndFunction

armor Function _GetSpellDisablerForm(bool debugOn) global
	Debug.TraceConditional("=================Estrus API GetSpellDisablerForm================", debugOn)
	return game.GetFormFromFile(0x01022FA0,"zzEstrus.esp") as armor
EndFunction

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;; Redirect Functions;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Bool Function IsDangerousWater(Form waterForm) global
	return EstrusForSkyrimNG.IsDangerousWater(waterForm)
EndFunction

Function SetDangerousWater(Form waterForm, Bool isDangerous) global
	EstrusForSkyrimNG.SetDangerousWater(waterForm, isDangerous)
EndFunction

Actor Function GetFollower(Bool isFemale, Int Number) global
	return EstrusForSkyrimNG.GetFollower(isFemale, Number)
EndFunction

Actor Function GetRandomFollower(Bool isFemale) global
	return EstrusForSkyrimNG.GetRandomFollower(isFemale)
EndFunction

int Function GetFollowersCount(Bool isFemale) global
	return EstrusForSkyrimNG.GetFollowersCount(isFemale)
EndFunction

String Function GetTriFilePath(Actor akActor, Int type) global
	return EstrusForSkyrimNG.GetTriFilePath(akActor, type)
EndFunction

Function SetTriFilePath(Actor akActor, Int type, String path) global
	EstrusForSkyrimNG.SetTriFilePath(akActor, type, path)
EndFunction

Idle Function GetCurrentIdle(Actor akActor) global
	return EstrusForSkyrimNG.GetCurrentIdle(akActor)
EndFunction

Function SetAnimObjectPath(Form animObject, String newPath) global
	EstrusForSkyrimNG.SetAnimObjectPath(animObject, newPath)
EndFunction

String Function GetAnimObjectPath(Form animObject) global
	return EstrusForSkyrimNG.GetAnimObjectPath(animObject)
EndFunction

Function Debug1() global
	EstrusForSkyrimNG.Debug1()
EndFunction

ObjectReference Function GetFurnitureReference(Actor akActor) global
	akActor.GetFurnitureReference()
EndFunction

Function SwitchTriFiles(Actor akActor, String headPath, String eyesPath, String browsPath) global
	SetTriFilePath(akActor, 1, headPath)
	SetTriFilePath(akActor, 2, eyesPath)
	SetTriFilePath(akActor, 6, browsPath)
EndFunction

;;; not implemented
Function SetUFOCamSpeedMult(Float ratio) global
	EstrusForSkyrimNG.SetUFOCamSpeedMult(ratio)
EndFunction

Function StartFlyCam() global
	EstrusForSkyrimNG.StartFlyCam()
EndFunction

Function StopFlyCam() global
	EstrusForSkyrimNG.StopFlyCam()
EndFunction

Function ToggleFlyCam() global
	EstrusForSkyrimNG.ToggleFlyCam()
EndFunction
