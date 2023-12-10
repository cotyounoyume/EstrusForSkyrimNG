#include "Papyrus.h"
#include "CommonFollowerUtil.h"

namespace Papyrus 
{
    //class EstrusForSkyrimNG {
        //static inline RE::TESObjectREFR* GetSelectedReference(RE::StaticFunctionTag*) {
        //    const auto selectedRef = RE::Console::GetSelectedRef();
        //    return selectedRef.get();
        //}

        //static inline void Debug1(RE::StaticFunctionTag*) 
        //{ 
        //    logs::info("Action from papyrus"); 
        //}

        //static inline RE::Actor* getFollower(RE::StaticFunctionTag*, bool isFemale, uint32_t number) 
        //{
        //    if (isFemale) {
        //        return (RE::Actor*)FollowerCollection::getFollowerObjectFemale(number);
        //    } else {
        //        return (RE::Actor*)FollowerCollection::getFollowerObjectMale(number);
        //    }
        //}

        //static inline RE::Actor* getRandomFollower(RE::StaticFunctionTag*, bool isFemale) 
        //{
        //    if (isFemale) {
        //        return (RE::Actor*)FollowerCollection::getRandomFollowerObjectFemale();
        //    } else {
        //        return (RE::Actor*)FollowerCollection::getRandomFollowerObjectMale();
        //    }
        //}

        //static inline uint32_t getFollowersCount(RE::StaticFunctionTag*, bool isFemale) 
        //{
        //    if (isFemale) {
        //        return FollowerCollection::getFollowersCountFemale();
        //    } else {
        //        return FollowerCollection::getFollowersCountMale();
        //    }
        //}

        //static inline bool IsDangerousWater(RE::StaticFunctionTag*, RE::TESForm* waterForm) 
        //{
        //    if (waterForm->GetFormType() != RE::FormType::Water) {
        //        return false;
        //    }

        //    return dynamic_cast<RE::TESWaterForm*>(waterForm)->GetDangerous();
        //}

        //static inline void SetDangerousWater(RE::StaticFunctionTag*, RE::TESForm* waterForm, bool dangerous) 
        //{
        //    logs::info("waterForm {:08X} has type {}", waterForm->formID, waterForm->GetFormType());
        //    if (waterForm->GetFormType() != RE::FormType::Water) {
        //        logs::info("this route is failure");
        //        return;
        //    }

        //    if (dangerous) {
        //        logs::info("this route is true");
        //        dynamic_cast<RE::TESWaterForm*>(waterForm)->flags.set(RE::TESWaterForm::Flag::kCauseDamage);
        //    } else {
        //        logs::info("this route is false");
        //        dynamic_cast<RE::TESWaterForm*>(waterForm)->flags.set(RE::TESWaterForm::Flag::kNone);
        //    }
        //}


        //bool EstrusForSkyrimNG::Register(RE::BSScript::IVirtualMachine* a_vm) {
        //    a_vm->RegisterFunction("Debug1", CLASS_NAME, Debug1);

        //    logs::info("Registered funcs for class {}", CLASS_NAME);

        //    return true;
        //}

        //inline bool RegisterAll(RE::BSScript::IVirtualMachine* a_vm) 
        //{
        //    EstrusForSkyrimNG::Register(a_vm);
        //    return true;
        //}

//        // Idle function getCurrentIdle(RE::Actor akRE::Actor) global native
//        RE::TESIdleForm* GetCurrentIdle(RE::StaticFunctionTag*, RE::Actor* thisActor) {
//            if (!thisActor) {
//                logs::info("RE::Actor %08X is null", thisActor->formID);
//                return NULL;
//            }
//            auto ai = thisActor->GetActorRuntimeData().currentProcess;
//            auto high = ai->high;
//            high->muzzleFlash->
//            RE::ActorProcessManager* processManager = thisActor->processManager;
//            if (!processManager) {
//                logs::info("RE::Actor %08X 's processManager %08X is null", thisActor->formID, processManager);
//                return NULL;
//            }
//            MiddleProcess* middleProcess = processManager->middleProcess;
//            if (!middleProcess) {
//                logs::info("RE::Actor %08X 's middleProcess %08X is null", thisActor->formID, middleProcess);
//                return NULL;
//            }
//            TESIdleForm* refr = middleProcess->currentIdle;
//
//            if (!refr) {
//                logs::info("RE::Actor %08X 's current is null(%08X)", thisActor->formID, refr);
//                return NULL;
//            }
//
//            logs::info("RE::Actor %08X 's currentIdle formID is %08X", thisActor->formID, refr->formID);
//
//            return refr;
//        }
//
//        TESObjectREFR* GetFurnitureReference(RE::StaticFunctionTag*, RE::Actor* thisActor) {
//#ifdef _DEBUG
//            logs::info("func:%s start", __FUNCTION__);
//#endif
//            if (!thisActor) {
//                return NULL;
//            }
//            RE::ActorProcessManager* processManager = thisActor->processManager;
//            if (!processManager) {
//                return NULL;
//            }
//            MiddleProcess* middleProcess = processManager->middleProcess;
//            if (!middleProcess) {
//                return NULL;
//            }
//
//            NiPointer<TESObjectREFR> refr;
//            //		TESObjectREFR * refr = NULL;
//            uint32_t furnitureHandle = middleProcess->furnitureHandle;
//            //		UInt32 furnitureHandle = middleProcess->unk138;
//            if (furnitureHandle == (*g_invalidRefHandle) || furnitureHandle == 0) {
//                return NULL;
//            }
//            LookupREFRByHandle(furnitureHandle, refr);
//            //		LookupREFRByHandle(&furnitureHandle, &refr);
//
//            if (!refr) {
//                return NULL;
//            }
//            return refr;
//        }
//
//        // TextureSet function GetDefaultSkinTextureSet(ArmorAddon thisArmorAddon, Bool bFemale) global native
//        BGSTextureSet* GetDefaultSkinTextureSet(RE::StaticFunctionTag*, TESObjectARMA* thisArmorAddon, bool bFemale) {
//#ifdef _DEBUG
//            logs::info("func:%s start", __FUNCTION__);
//#endif
//            // BGSTextureSet* textureSetA4 = thisArmorAddon->unkA4; unkA4 is TextureSet Male Skin Texture;
//            // BGSTextureSet* textureSetA8 = thisArmorAddon->unkA4; unkA4 is TextureSet Female Skin Texture;
//
//            if (thisArmorAddon) {
//                return (bFemale) ? (BGSTextureSet*)thisArmorAddon->skinTextures[0]
//                                 : (BGSTextureSet*)thisArmorAddon->skinTextures[1];
//            } else {
//                return NULL;
//            }
//        }
//
//        // String function GetTriFilePath(RE::Actor akRE::Actor, Int type) global native
//        BSFixedString GetTriFilePath(RE::StaticFunctionTag*, RE::Actor* thisActor, uint32_t type) {
//#ifdef _DEBUG
//            logs::info("func:%s start", __FUNCTION__);
//#endif
//            TESNPC* npc = DYNAMIC_CAST(thisActor->baseForm, TESForm, TESNPC);
//            BGSHeadPart* hp = npc->GetHeadPartByType(type);
//            return (hp) ? hp->morph.GetModelName() : NULL;
//            //		(TESQuest*)LookupFormByID(formID);
//        }
//
//        // function SwitchTriFiles(RE::Actor akRE::Actor, String headPath, String eyesPath, String browsPath) global native
//        void SwitchTriFiles(RE::StaticFunctionTag*, RE::Actor* thisActor, BSFixedString pathHead, BSFixedString pathEyes,
//                            BSFixedString pathBrows) {
//#ifdef _DEBUG
//            logs::info("func:%s start", __FUNCTION__);
//#endif
//            TESNPC* npc = DYNAMIC_CAST(thisActor->baseForm, TESForm, TESNPC);
//            BGSHeadPart* hpHead = npc->GetHeadPartByType(1);
//            BGSHeadPart* hpEyes = npc->GetHeadPartByType(2);
//            BGSHeadPart* hpBrows = npc->GetHeadPartByType(6);
//            hpHead->morph.SetModelName(pathHead.data);
//            hpEyes->morph.SetModelName(pathEyes.data);
//            hpBrows->morph.SetModelName(pathBrows.data);
//        }
//
//        // String function GetAnimObjectPath(AnimObject) global native
//        BSFixedString GetAnimObjectPath(RE::StaticFunctionTag*, TESForm* thisForm) {
//#ifdef _DEBUG
//            logs::info("func:%s start", __FUNCTION__);
//#endif
//            TESObjectANIO* anim = DYNAMIC_CAST(thisForm, TESForm, TESObjectANIO);
//            return (anim) ? anim->textureSwap.GetModelName() : NULL;
//        }
//
//        // function SetAnimObjectPath(AnimObject anim, String newPath) global native
//        void SetAnimObjectPath(RE::StaticFunctionTag*, TESForm* thisForm, BSFixedString newPath) {
//#ifdef _DEBUG
//            logs::info("func:%s start", __FUNCTION__);
//#endif
//            TESObjectANIO* anim = DYNAMIC_CAST(thisForm, TESForm, TESObjectANIO);
//            if (!anim) {
//                return;
//            }
//
//            anim->textureSwap.SetModelName(newPath.data);
//        }
//
//        // Float function GetDefaultFOV() global native
//        float GetDefaultFOV(RE::StaticFunctionTag*) {
//#ifdef _DEBUG
//            logs::info("func:%s start", __FUNCTION__);
//#endif
//            PlayerCamera* camera = PlayerCamera::GetSingleton();
//            return camera->worldFOV;
//        }
//
//        // Float function GetCurrentFOV() global native
//        float GetCurrentFOV(RE::StaticFunctionTag*) {
//#ifdef _DEBUG
//            logs::info("func:%s start", __FUNCTION__);
//#endif
//            PlayerCamera* camera = PlayerCamera::GetSingleton();
//            return camera->firstPersonFOV;
//        }
//
//        // function SetCurrentFOV(Float fov) global native
//        void SetCurrentFOV(RE::StaticFunctionTag*, float fFOV) {
//#ifdef _DEBUG
//            logs::info("func:%s start", __FUNCTION__);
//#endif
//            PlayerCamera* camera = PlayerCamera::GetSingleton();
//            camera->firstPersonFOV = fFOV;
//        }
//
//        // function EstrusMFG(RE::Actor akRE::Actor, Int type, Int mood, Float strength) global native
//        void EstrusMFG(RE::StaticFunctionTag*, RE::Actor* thisActor, uint32_t type, uint32_t mood, float strength) {
//#ifdef _DEBUG
//            logs::info("func:%s start", __FUNCTION__);
//#endif
//            return;
//        }
//
//        // function EstrusMFG(RE::Actor akRE::Actor, Int type, Int mood, Float strength) global native
//        void forDebug(RE::StaticFunctionTag*) {
//#ifdef _DEBUG
//            logs::info("func:%s start", __FUNCTION__);
//#endif
//            logs::info("func:%s start", __FUNCTION__);
//            return;
//        }





        /*
        //function SetGlobalTimeMultiplier(Float ratio) global native
        void SetGlobalTimeMultiplier(RE::StaticFunctionTag *base, float fTime)
        {
        #ifdef _DEBUG
        logs::info("func:%s start",__FUNCTION__);
        #endif
        return GlobalTimeController::GetSingleton()->SetGlobalTimeMultiplier(fTime, true);
        }

        //function StartFlyCam() global native
        void StartFlyCam(RE::StaticFunctionTag*)
        {
        #ifdef _DEBUG
        logs::info("func:%s start",__FUNCTION__);
        #endif
        bool bStop = g_timeController->unk0A == 1;
        PlayerCameraEx* camera = PlayerCameraEx::GetSingleton();
        if (camera->cameraState == camera->cameraStates[camera->kCameraState_Free])
        {
        return;
        }else{
        bool bStop = g_timeController->unk0A == 1;
        PlayerCameraEx* camera = PlayerCameraEx::GetSingleton();
        CALL_MEMBER_FN(camera, ToggleFlyCam)(true);
        InputManagerEx* im = InputManagerEx::GetSingleton();
        CALL_MEMBER_FN(im, DisableControls)(0x0D);
        }
        }

        //function StopFlyCam() global native
        void StopFlyCam(RE::StaticFunctionTag*)
        {
        #ifdef _DEBUG
        logs::info("func:%s start",__FUNCTION__);
        #endif
        bool bStop = g_timeController->unk0A == 1;
        PlayerCameraEx* camera = PlayerCameraEx::GetSingleton();
        if (camera->cameraState == camera->cameraStates[camera->kCameraState_Free]){
        CALL_MEMBER_FN(camera, ToggleFlyCam)(false);
        InputManagerEx* im = InputManagerEx::GetSingleton();
        CALL_MEMBER_FN(im, EnableControls)(0x0D);
        }else{
        return;
        }
        }

        //function ToggleFlyCam() global native
        void ToggleFlyCam(RE::StaticFunctionTag*)
        {
        #ifdef _DEBUG
        logs::info("func:%s start",__FUNCTION__);
        #endif
        bool bStop = g_timeController->unk0A == 1;
        PlayerCameraEx* camera = PlayerCameraEx::GetSingleton();
        CALL_MEMBER_FN(camera, ToggleFlyCam)(bStop);

        InputManagerEx* im = InputManagerEx::GetSingleton();
        if (camera->cameraState == camera->cameraStates[camera->kCameraState_Free])
        {
        CALL_MEMBER_FN(im, DisableControls)(0x0D);
        }
        else
        {
        CALL_MEMBER_FN(im, EnableControls)(0x0D);
        }
        }

        //function TogglePause() global native
        void TogglePause(RE::StaticFunctionTag*)
        {
        #ifdef _DEBUG
        logs::info("func:%s start",__FUNCTION__);
        #endif
        g_timeController->unk0A = (g_timeController->unk0A == 0 ? 1 : 0);
        }

        //function SetUFOCamSpeedMult(Float ratio) global native
        void SetUFOCamSpeedMult(RE::StaticFunctionTag*, float fMult)
        {
        #ifdef _DEBUG
        logs::info("func:%s start",__FUNCTION__);
        #endif
        g_fUFOCamSpeedMult = fMult;
        }
        */

    //}

    // ƒpƒsƒ‹ƒXŠÖ”‚ð“o˜^‚·‚é‚Æ‚«‚ÉŒÄ‚Î‚ê‚é
    //void
    //Papyrus::RegisterFuncs(VMClassRegistry* registry) {
    //    registry->RegisterFunction(new NativeFunction1<RE::StaticFunctionTag, TESObjectREFR*, RE::Actor*>(
    //        "GetFurnitureReference", s_className, GetFurnitureReference, registry));

    //    registry->RegisterFunction(new NativeFunction1<RE::StaticFunctionTag, TESIdleForm*, RE::Actor*>(
    //        "GetCurrentIdle", s_className, GetCurrentIdle, registry));

    //    registry->RegisterFunction(new NativeFunction2<RE::StaticFunctionTag, BSFixedString, RE::Actor*, uint32_t>(
    //        "GetTriFilePath", s_className, GetTriFilePath, registry));

    //    registry->RegisterFunction(
    //        new NativeFunction4<RE::StaticFunctionTag, void, RE::Actor*, BSFixedString, BSFixedString, BSFixedString>(
    //            "SwitchTriFiles", s_className, SwitchTriFiles, registry));

    //    registry->RegisterFunction(new NativeFunction1<RE::StaticFunctionTag, bool, TESForm*>(
    //        "IsDangerousWater", s_className, IsDangerousWater, registry));

    //    registry->RegisterFunction(new NativeFunction2<RE::StaticFunctionTag, void, TESForm*, bool>(
    //        "SetDangerousWater", s_className, SetDangerousWater, registry));

    //    registry->RegisterFunction(
    //        new NativeFunction0<RE::StaticFunctionTag, void>("Debug1", s_className, Debug1, registry));

    //    registry->RegisterFunction(new NativeFunction1<RE::StaticFunctionTag, RE::Actor*, bool>(
    //        "GetRandomFollower", s_className, getRandomFollower, registry));

    //    registry->RegisterFunction(new NativeFunction1<RE::StaticFunctionTag, uint32_t, bool>(
    //        "GetFollowersCount", s_className, getFollowersCount, registry));

    //    registry->RegisterFunction(new NativeFunction2<RE::StaticFunctionTag, RE::Actor*, bool, uint32_t>(
    //        "GetFollower", s_className, getFollower, registry));

    //    registry->RegisterFunction(new NativeFunction1<RE::StaticFunctionTag, BSFixedString, TESForm*>(
    //        "GetAnimObjectPath", s_className, GetAnimObjectPath, registry));

    //    registry->RegisterFunction(new NativeFunction2<RE::StaticFunctionTag, void, TESForm*, BSFixedString>(
    //        "SetAnimObjectPath", s_className, SetAnimObjectPath, registry));



        /*
        registry->RegisterFunction(
            new NativeFunction0<RE::StaticFunctionTag, void>("StartFlyCam", s_className, StartFlyCam, registry));

        registry->RegisterFunction(
            new NativeFunction0<RE::StaticFunctionTag, void>("StopFlyCam", s_className, StopFlyCam, registry));

        registry->RegisterFunction(
            new NativeFunction0<RE::StaticFunctionTag, void>("ToggleFlyCam", s_className, ToggleFlyCam, registry));

        registry->RegisterFunction(
            new NativeFunction1<RE::StaticFunctionTag, void, float>("SetUFOCamSpeedMult", s_className, SetUFOCamSpeedMult,
        registry));
        */

        /*
        registry->RegisterFunction(
        new NativeFunction0<RE::StaticFunctionTag, void>("TogglePause", s_className, TogglePause, registry));
        registry->RegisterFunction(
        new NativeFunction1<RE::StaticFunctionTag, void, float>("SetGlobalTimeMultiplier", s_className,
        SetGlobalTimeMultiplier, registry));
        */

        /*
        registry->RegisterFunction(
        new NativeFunction0<RE::StaticFunctionTag, void>("forDebug", s_className, forDebug, registry));
        registry->RegisterFunction(
        new NativeFunction4<RE::StaticFunctionTag, void, RE::Actor*, UInt32, UInt32, float>("EstrusMFG", s_className, EstrusMFG,
        registry)); registry->RegisterFunction( new NativeFunction0<RE::StaticFunctionTag, float>("GetDefaultFOV",
        s_className, GetDefaultFOV, registry)); registry->RegisterFunction( new NativeFunction0<RE::StaticFunctionTag,
        float>("GetCurrentFOV", s_className, GetCurrentFOV, registry)); registry->RegisterFunction( new
        NativeFunction1<RE::StaticFunctionTag, void, float>("SetCurrentFOV", s_className, SetCurrentFOV, registry));
        registry->RegisterFunction(
        new NativeFunction2<RE::StaticFunctionTag,  BGSTextureSet*, TESObjectARMA*, bool>("GetDefaultSkinTextureSet",
        s_className, GetDefaultSkinTextureSet, registry));
        */
    //}



    //};
}